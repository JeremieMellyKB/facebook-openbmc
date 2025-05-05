#include "../kb900x_comm.h"
#include "../kb900x_utils.h"
#include "../kb900x_log.h"

#include <facebook/bic_xfer.h>

static uint8_t cal_crc8(uint8_t *data, size_t len)
{
  uint8_t crc = 0x00;
  for (size_t i = 0; i < len; i++)
  {
    crc ^= data[i];
    for (int j = 0; j < 8; j++)
    {                                                      // NOLINT
      crc = (crc & 0x80) ? (crc << 1) ^ 0x07 : (crc << 1); // NOLINT
    }
  }
  return crc;
}

static int get_smbus_command_code(uint8_t address_size, uint8_t *command_code_start,
                                  uint8_t *command_code_stop)
{
  if (address_size == 4)
  {
    *command_code_start = CCODE_START_READ_FUNC2;
    *command_code_stop = CCODE_END_READ_FUNC2;
  }
  else if (address_size == 2)
  {
    *command_code_start = CCODE_START_READ_FUNC0;
    *command_code_stop = CCODE_END_READ_FUNC0;
  }
  else
  {
    KANDOU_ERR("Address size not supported. Must be 2 or 4 bytes.");
    return -EINVAL;
  }
  return KB900X_E_OK;
}

int kb900x_write(const kb900x_config config, const uint8_t *address, const uint8_t address_size,
                                      const uint8_t *payload, const uint8_t payload_size)
{
    // We only use 4 bytes addresses with vendor defined SMBus write register
    if (address_size != 4)
    {
      KANDOU_ERR("Address size must be 4 bytes");
      return -EINVAL;
    }
    uint8_t tbuf[64] = {0x00};
    uint8_t rbuf[64] = {0x00};
    uint8_t tlen = 0;
    uint8_t rlen = 0;
  
    const uint8_t smbus_tx_length = payload_size + address_size + 3; // Write length = ByteCount + command code + payload size + address size + PEC
    tbuf[0] = (config.bus_id << 1) + 1;
    tbuf[1] = config.retimer_addr;
    tbuf[2] = 0x00; // Read count = 0
    tbuf[3] = CCODE_START_END_WRITE_FUNC3;
    tbuf[4] = smbus_tx_length;
  
    // Copy address to the beginning of the buffer
    for (size_t i = 0; i < address_size; i++)
    {
      // As SMBus expect address in little endian
      // We reverse the address to match the expected format
      tbuf[i + 5] = address[address_size - 1 - i];
    }
    // Copy the payload after the address
    for (size_t i = 0; i < payload_size; i++)
    {
      // As SMBus expect payload in little endian
      // We reverse the payload to match the expected format
      tbuf[i + 5 + address_size] = payload[payload_size - i - 1];
    }
    // Add the PEC
    uint8_t data_to_sign[smbus_tx_length];
    data_to_sign[0] = config.retimer_addr << 1; // Write
    memcpy(&(data_to_sign[1]), &(tbuf[3]), smbus_tx_length - 1);
    tbuf[smbus_tx_length + 2] = cal_crc8(data_to_sign, smbus_tx_length);
    tlen = smbus_tx_length + 3; // bus_id + retimer_addr + tx_len + I2C data
    int ret = bic_data_send(config.slot_id, NETFN_APP_REQ, CMD_APP_MASTER_WRITE_READ, tbuf, tlen, rbuf, &rlen, config.intf);
    if(ret != 0) {
      KANDOU_ERR("bic_data_wrapper failed with error code: %d", ret);
    }
    return ret;
}

int kb900x_read(const kb900x_config config, const uint8_t *address, const uint8_t address_size,
                uint8_t *result, uint8_t result_size) {
  uint8_t tlen = 0;
  uint8_t rlen = 0;
  uint8_t tbuf[64] = {0x00};
  uint8_t rbuf[64] = {0x00};

  // Get SMBus Command Code
  uint8_t command_code_start;
  uint8_t command_code_stop;
  int ret = get_smbus_command_code(address_size, &command_code_start,
                                   &command_code_stop);
  CHECK_SUCCESS(ret);
  // First we need to write the address to the retimer
  const uint8_t smbus_tx_length = address_size + 3; // Write length = Command Code + ByteCount + address size + PEC
  tbuf[0] = (config.bus_id << 1) + 1;
  tbuf[1] = config.retimer_addr;
  tbuf[2] = 0x00;
  tbuf[3] = command_code_start;
  tbuf[4] = address_size;
  
  // Copy address to the beginning of the buffer
  for (size_t i = 0; i < address_size; i++)
  {
    // As SMBus expect address in little endian
    // We reverse the address to match the expected format
    tbuf[i + 5] = address[address_size - 1 - i];
  }
  // Add the PEC
  uint8_t data_to_sign[smbus_tx_length];
  data_to_sign[0] = config.retimer_addr << 1; // Write
  memcpy(&(data_to_sign[1]), &(tbuf[3]), smbus_tx_length - 1);
  tbuf[smbus_tx_length + 2] = cal_crc8(data_to_sign, smbus_tx_length);
  // FIXME remove debug logs
  kandou_log_set_quiet(false);
  kandou_log_set_level(LOG_DEBUG);

  for(size_t i = 0; i < smbus_tx_length; i++)
  {
    KANDOU_DEBUG("data_to_sign[%d] = 0x%02x", i, data_to_sign[i]);
  }
  KANDOU_DEBUG("PEC: 0x%02x", tbuf[smbus_tx_length + 2]);

  KANDOU_DEBUG("Tbuf:");
  for (size_t i = 0; i < smbus_tx_length + 3; i++)
  {
    KANDOU_DEBUG("tbuf[%d] = 0x%02x", i, tbuf[i]);
  }

  kandou_log_set_quiet(true);

  tlen = smbus_tx_length + 3;  // bus_id + retimer_addr + tx_len + I2C data
  ret = bic_data_send(config.slot_id, NETFN_APP_REQ, CMD_APP_MASTER_WRITE_READ, tbuf, tlen, rbuf, &rlen, config.intf);
  if (ret != 0)
  {
    KANDOU_ERR("bic_data_wrapper failed with error code: %d", ret);
  }
  // Then we read the result from the retimer
  memset(tbuf, 0, sizeof(tbuf));
  memset(rbuf, 0, sizeof(rbuf));
  tlen = 0;
  rlen = 0;
  
  tbuf[0] = (config.bus_id << 1) + 1;
  tbuf[1] = config.retimer_addr;
  tbuf[2] = KB900X_REGLI_REGISTER_SIZE; // A read operation will always return 4 bytes
  tbuf[3] = command_code_stop;
  tlen = 4;
  ret = bic_data_send(config.slot_id, NETFN_APP_REQ, CMD_APP_MASTER_WRITE_READ, tbuf, tlen, rbuf, &rlen, config.intf);
  if (ret != 0 || rlen == 0)
  {
    KANDOU_ERR("bic_data_wrapper failed with error code: %d", ret);
  }else{
    // Copy the result to the result buffer
    const uint8_t bytecnt = rbuf[0];
    if(bytecnt < 4){

    }
    for (size_t i = 0; i < result_size; i++)
    {
      // As SMBus expect payload in little endian
      // We reverse the payload to match the expected format
      result[i] = rbuf[bytecnt - i];
    }
  }

  return ret;
}
