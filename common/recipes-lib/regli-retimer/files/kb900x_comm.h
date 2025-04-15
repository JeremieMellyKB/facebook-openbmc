#ifndef __KB900X_COMM_H__
#define __KB900X_COMM_H__

#include <stdint.h>

// Constants
// Command code as per supplemental spec Table 6-3.
// (Slave SMBus Command Code Fields)
#define CCODE_START_READ_FUNC0 (0x82)
#define CCODE_END_READ_FUNC0 (0x81)

#define CCODE_START_END_WRITE_FUNC1 (0x87)

#define CCODE_START_READ_FUNC2 (0x8A)
#define CCODE_END_READ_FUNC2 (0x89)

#define CCODE_START_END_WRITE_FUNC3 (0x8F)

#define KB900X_SMBUS_COMMAND_SIZE (4)
#define KB900X_SMBUS_COMMAND_ADDR_SIZE (2)
#define KB900X_REGLI_REGISTER_SIZE (4)
#define KB900X_REGLI_REGISTER_ADDR_SIZE (4)

// Configuration structure
typedef struct kb900x_config {
  uint8_t intf;  // Used only for BIC communication
  uint8_t slot_id;  // Used only for BIC communication
  uint8_t bus_id;  // Used only for BIC communication
  uint8_t retimer_addr;
  uint8_t handle;  // Used only for direct I2C connection
} kb900x_config;

/** \brief open an i2c connection and return the handle
 *
 * \note This function is only needed when communicating with the retimer
 * directly through I2C
 *
 * \param[in] i2c_bus the i2c device bus id (/dev/i2c-{i2c_id})
 * \param[in] slave_address the slave address of the device
 *
 * \return the handler of the I2C connection
 */
int kb900x_i2c_init(int i2c_bus, uint8_t slave_address);

/** \brief open an i2c connection
 *
 * \param[in] i2c_bus the i2c device bus id (/dev/i2c-{i2c_id})
 * \param[in] slave_address the slave address of the device
 *
 * \return the handler of the I2C connection
 */
int kb900x_i2c_open(int i2c_id, uint8_t slave_address);

/** \brief select the slave address
 *
 * \param[in] handle the handler of the I2C connection
 * \param[in] slave_address the slave address of the device
 *
 * \return 0 if no error, else the error code
 */
int kb900x_i2c_select_slave_addr(int handle, uint8_t slave_address);

/** \brief Write a four-bytes address register.
 *
 * \param[in] config the driver configuration
 * \param[in] address the register address
 * \param[out] payload the payload to write in the register
 *
 * \return 0 if no error, else the error code
 */
int kb900x_write_register(const kb900x_config config, const uint32_t address,
                          const uint32_t payload);

/** \brief Read a four-bytes address register.
 *
 * \param[in] config the driver configuration
 * \param[in] address the register address
 * \param[out] result pointer to an uint32_t to store the result
 *
 * \return 0 if no error, else the error code
 */
int kb900x_read_register(const kb900x_config config, const uint32_t address, uint32_t *result);

/** \brief Read a two-bytes address register using SMBus.
 *
 * \param[in] config the driver configuration
 * \param[in] offset the SMBus command offset
 * \param[out] result pointer to an uint32_t to store the result
 */
int kb900x_read_smbus_command(const kb900x_config config, uint16_t offsets, uint32_t *result);

/** \brief Write an SMBus block through BIC.
 *
 * \param[in] config the driver configuration
 * \param[in] address pointer to the address (big-endian)
 * \param[in] address_size the size of the address in bytes
 * \param[in] payload pointer to the payload (big-endian)
 * \param[in] payload_size the size of the payload
 *
 * \return the result code
 */
int kb900x_write(const kb900x_config config, const uint8_t *address, const uint8_t address_size,
                 const uint8_t *payload, const uint8_t payload_size);

/** \brief Read an SMBus block through BIC.
 *
 * \param[in] config the driver configuration
 * \param[in] address pointer to the address (big-endian)
 * \param[in] address_size the size of the address in bytes
 * \param[out] result pointer to the array that will store the data read
 * (big-endian) \param[in] result_size the size of the result array
 *
 * \return the result code
 */
int kb900x_read(const kb900x_config config, const uint8_t *address, const uint8_t address_size,
                uint8_t *result, uint8_t result_size);

#endif // __KB900X_COMM_H__