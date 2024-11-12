/*******************************************************************************
 *   Plugin Morpho
 *   (c) 2023 Ledger
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 ********************************************************************************/

#pragma once

#include <string.h>
#include "eth_plugin_interface.h"

// All possible selectors for plugin
#define SELECTORS_LIST(X)            \
    X(DEPOSIT, 0x6e553f65)           \
    X(MINT, 0x94bf804d)              \
    X(REDEEM, 0xba087652)            \
    X(WITHDRAW, 0xb460af94)          \
    X(APPROVE, 0x095ea7b3)           \
    X(SET_AUTHORIZATION, 0xeecea000) \
    X(FLASH_LOAN, 0xe0232b42)        \
    X(BORROW, 0x50d8cd4b)            \
    X(REPAY, 0x20b76e81)             \
    X(WITHDRAW_BLUE, 0x5c2bea49)     \
    X(SUPPLY, 0xa99aad89)

// Xmacro helpers to define the enum and map
// Do not modify !
#define TO_ENUM(selector_name, selector_id)  selector_name,
#define TO_VALUE(selector_name, selector_id) selector_id,

// This enum will be automatically expanded to hold all selector names.
// The value SELECTOR_COUNT can be used to get the number of defined selectors
// Do not modify !
typedef enum selector_e {
    SELECTORS_LIST(TO_ENUM) SELECTOR_COUNT,
} selector_t;

// This array will be automatically expanded to map all selector_t names with the correct value.
// Do not modify !
extern const uint32_t SELECTORS[SELECTOR_COUNT];

#define HALF_PARAMETER_LENGTH 16

// Enumeration used to parse the smart contract data.
typedef enum {
    NONE,
    AMOUNT,
    RECEIVER,
    SPENDER,
    SHARES,
    OWNER,
    ADDRESS,
    IS_AUTHORIZED,
    TOKEN,
    ASSETS,
    DATA,
    DATA_SIZE,
    DATA_OFFSET,
    DATA_CONTAINER_1,
    DATA_CONTAINER_2,
    TUPPLE_1,
    TUPPLE_2,
    TUPPLE_3,
    TUPPLE_4,
    TUPPLE_5,
    SENDER
} parameter;

typedef struct {
    uint8_t value[INT256_LENGTH];
    bool ellipsis;
} bytes32_t;

typedef struct {
    uint8_t value[ADDRESS_LENGTH];
} address_t;

// Tx struct for each operation
typedef struct {
    bytes32_t assets;
    address_t receiver;
} deposit_t;

typedef struct {
    bytes32_t shares;
    address_t receiver;
} mint_t;

typedef struct {
    bytes32_t shares;
    address_t receiver;
    address_t owner;
} redeem_t;

typedef struct {
    bytes32_t assets;
    address_t receiver;
    address_t owner;
} withdraw_t;

typedef struct {
    bytes32_t shares;
    address_t spender;
} approve_t;

typedef struct {
    address_t address;
    uint16_t isAuthorized;
} set_authorization_t;

typedef struct {
    bytes32_t assets;
    address_t token;
    bytes32_t data;
    uint16_t data_size;
    uint16_t data_offset;
} flash_loan_t;

typedef struct {
    bytes32_t assets;
    bytes32_t shares;
    address_t sender;
} morpho_blue_generic_t;

// Shared global memory with Ethereum app. Must be at most 5 * 32 bytes.
typedef struct context_s {
    // For parsing data.
    uint8_t next_param;  // Set to be the next param we expect to parse.
    uint16_t offset;     // Offset at which the array or struct starts.
    bool go_to_offset;   // If set, will force the parsing to iterate through parameters until
                         // `offset` is reached.

    union {
        // MetaMorpho
        deposit_t deposit;
        mint_t mint;
        redeem_t redeem;
        withdraw_t withdraw;
        approve_t approve;
        // MorphoBlue
        set_authorization_t set_authorization;
        flash_loan_t flash_loan;
        morpho_blue_generic_t generic;  // borrow, repay
    } tx;

    // For both parsing and display.
    selector_t selectorIndex;
} context_t;

// Check if the context structure will fit in the RAM section ETH will prepare for us
// Do not remove!
ASSERT_SIZEOF_PLUGIN_CONTEXT(context_t);
