/*
 * Copyright (c) Kandou-AI.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _KB_REG_DUMP_H
#define _KB_REG_DUMP_H

#include "kb900x_utils.h"

// Addresses to dump, number of registers are calculated by dividing the
// byte-length of each range, by the length of each register (4 bytes)
#define KB900X_RPCS_CORE_BASE_ADDR (0xE04C0000)
#define KB900X_RPCS_CORE_NUM_REG (0x000012B4 / BYTES_IN_U32)
#define KB900X_RPCS_AON_BASE_ADDR (0xE04F0000)
#define KB900X_RPCS_AON_NUM_REG (0x00000018 / BYTES_IN_U32)
#define KB900X_PHC0_BASE_ADDR (0xE0100000)
#define KB900X_PHC0_NUM_REG (0x0001C584 / BYTES_IN_U32)
#define KB900X_PHC1_BASE_ADDR (0xE0140000)
#define KB900X_PHC1_NUM_REG (0x0001C584 / BYTES_IN_U32)
#define KB900X_PHC2_BASE_ADDR (0xE0180000)
#define KB900X_PHC2_NUM_REG (0x0001C584 / BYTES_IN_U32)
#define KB900X_PHC3_BASE_ADDR (0xE01C0000)
#define KB900X_PHC3_NUM_REG (0x0001C584 / BYTES_IN_U32)
#define KB900X_PHT0_BASE_ADDR (0xE0200000)
#define KB900X_PHT0_NUM_REG (0x00000EAC / BYTES_IN_U32)
#define KB900X_PHT1_BASE_ADDR (0xE0240000)
#define KB900X_PHT1_NUM_REG (0x00000EAC / BYTES_IN_U32)

// NOTE: KB900X_DUMP_NUM_RANGES **MUST** match the length of reg_dump_ranges defined in regli.c
#define KB900X_DUMP_NUM_RANGES (8)
/** The number of register records in a full dump. */
#define KB900X_DUMP_NUM_REG                                                                        \
    ((size_t)(KB900X_RPCS_CORE_NUM_REG + KB900X_RPCS_AON_NUM_REG + KB900X_PHC0_NUM_REG +           \
              KB900X_PHC1_NUM_REG + KB900X_PHC2_NUM_REG + KB900X_PHC3_NUM_REG +                    \
              KB900X_PHT0_NUM_REG + KB900X_PHT1_NUM_REG))

#endif // _KB_REG_DUMP_H
