#include "plugin.h"

#define ELLIPSIS_LENGTH 3
/**
 * @brief Convert byte array into a hexadecimal string
 *
 * @param dst
 * @param dstLen
 * @param src
 * @param count
 * @param addEllipsis
 * @return true
 * @return false
 */
static bool array_to_hexstr(char *dst,
                            size_t dstLen,
                            const uint8_t *src,
                            uint16_t count,
                            bool addEllipsis) {
    memset(dst, 0, dstLen);
    size_t requiredLen = count * 2 + 1 + (addEllipsis ? ELLIPSIS_LENGTH : 0);
    if (dstLen < requiredLen) {
        return false;
    }
    const char hexchars[] = "0123456789abcdef";
    int halfCount = count / 2;  // Calculate the middle point

    for (int i = 0; i < halfCount; i++, src++) {
        *dst++ = hexchars[*src >> 4u];
        *dst++ = hexchars[*src & 0x0Fu];
    }

    if (addEllipsis) {
        // Add "..." in the middle
        *dst++ = '.';
        *dst++ = '.';
        *dst++ = '.';
    }

    for (int i = halfCount; i < count; i++, src++) {
        *dst++ = hexchars[*src >> 4u];
        *dst++ = hexchars[*src & 0x0Fu];
    }

    *dst = 0;  // terminate string

    return true;
}

/**
 * @brief Create ui for byte array
 *
 * @param msg
 * @param array
 * @param title
 * @return true
 * @return false
 */
static bool set_bytes32_ui(ethQueryContractUI_t *msg, bytes32_t *array, const char *title) {
    strlcpy(msg->title, title, msg->titleLength);
    return array_to_hexstr(msg->msg, msg->msgLength, array->value, 32, array->ellipsis);
}

/**
 * @brief Set the bool ui object
 *
 * @param msg msg context
 * @param val value to printed
 * @param title
 * @return true
 * @return false
 */
static bool set_bool_ui(ethQueryContractUI_t *msg, uint16_t val, const char *title) {
    strlcpy(msg->title, title, msg->titleLength);

    if (val == 0) {
        snprintf(msg->msg, msg->msgLength, "%s", "False");
        return true;
    } else {
        snprintf(msg->msg, msg->msgLength, "%s", "True");
        return true;
    }
    return false;
}

/**
 * @brief Set the address ui object
 *
 * @param msg message containing the parameter
 * @param value addres to be printed
 * @return true
 * @return false
 */
static bool set_address_ui(ethQueryContractUI_t *msg, address_t *value) {
    if (msg->msgLength <= ADDRESS_LENGTH * 2 + 2) {
        return false;
    }
    // Prefix the address with `0x`.
    msg->msg[0] = '0';
    msg->msg[1] = 'x';

    // We need a random chainID for legacy reasons with `getEthAddressStringFromBinary`.
    // Setting it to `0` will make it work with every chainID :)
    uint64_t chainid = 0;

    // Get the string representation of the address stored in `context->beneficiary`. Put it in
    // `msg->msg`.
    return getEthAddressStringFromBinary(
        value->value,
        msg->msg + 2,  // +2 here because we've already prefixed with '0x'.
        chainid);
}

static bool handle_deposit(ethQueryContractUI_t *msg, context_t *ctx, uint8_t screenIndex) {
    switch (screenIndex) {
        case 0:
            strlcpy(msg->title, "Assets", msg->titleLength);
            return uint256_to_decimal(ctx->tx.deposit.assets.value,
                                      sizeof(ctx->tx.deposit.assets.value),
                                      msg->msg,
                                      msg->msgLength);
        case 1:
            strlcpy(msg->title, "Receiver", msg->titleLength);
            return set_address_ui(msg, &ctx->tx.deposit.receiver);
        default:
            PRINTF("Received an invalid screenIndex\n");
            return false;
    }
}

static bool handle_approve(ethQueryContractUI_t *msg, context_t *ctx, uint8_t screenIndex) {
    switch (screenIndex) {
        case 0:
            strlcpy(msg->title, "Shares", msg->titleLength);
            return uint256_to_decimal(ctx->tx.approve.shares.value,
                                      sizeof(ctx->tx.approve.shares.value),
                                      msg->msg,
                                      msg->msgLength);
        case 1:
            strlcpy(msg->title, "Spender", msg->titleLength);
            return set_address_ui(msg, &ctx->tx.approve.spender);
        default:
            PRINTF("Received an invalid screenIndex\n");
            return false;
    }
}

static bool handle_redeem(ethQueryContractUI_t *msg, context_t *ctx, uint8_t screenIndex) {
    switch (screenIndex) {
        case 0:
            strlcpy(msg->title, "Shares", msg->titleLength);
            return uint256_to_decimal(ctx->tx.redeem.shares.value,
                                      sizeof(ctx->tx.redeem.shares.value),
                                      msg->msg,
                                      msg->msgLength);
        case 1:
            strlcpy(msg->title, "Receiver", msg->titleLength);
            return set_address_ui(msg, &ctx->tx.redeem.receiver);
        case 2:
            strlcpy(msg->title, "Owner", msg->titleLength);
            return set_address_ui(msg, &ctx->tx.redeem.owner);
        default:
            PRINTF("Received an invalid screenIndex\n");
            return false;
    }
}

static bool handle_withdraw(ethQueryContractUI_t *msg, context_t *ctx, uint8_t screenIndex) {
    switch (screenIndex) {
        case 0:
            strlcpy(msg->title, "Assets", msg->titleLength);
            return uint256_to_decimal(ctx->tx.withdraw.assets.value,
                                      sizeof(ctx->tx.withdraw.assets.value),
                                      msg->msg,
                                      msg->msgLength);
        case 1:
            strlcpy(msg->title, "Receiver", msg->titleLength);
            return set_address_ui(msg, &ctx->tx.withdraw.receiver);
        case 2:
            strlcpy(msg->title, "Owner", msg->titleLength);
            return set_address_ui(msg, &ctx->tx.withdraw.owner);
        default:
            PRINTF("Received an invalid screenIndex\n");
            return false;
    }
}

static bool handle_mint(ethQueryContractUI_t *msg, context_t *ctx, uint8_t screenIndex) {
    switch (screenIndex) {
        case 0:
            strlcpy(msg->title, "Shares", msg->titleLength);
            return uint256_to_decimal(ctx->tx.mint.shares.value,
                                      sizeof(ctx->tx.mint.shares.value),
                                      msg->msg,
                                      msg->msgLength);
        case 1:
            strlcpy(msg->title, "Receiver", msg->titleLength);
            return set_address_ui(msg, &ctx->tx.mint.receiver);
        default:
            PRINTF("Received an invalid screenIndex\n");
            return false;
    }
}

static bool handle_set_authorization(ethQueryContractUI_t *msg,
                                     context_t *ctx,
                                     uint8_t screenIndex) {
    switch (screenIndex) {
        case 0:
            strlcpy(msg->title, "Authorized", msg->titleLength);
            return set_address_ui(msg, &ctx->tx.set_authorization.address);
        case 1:
            strlcpy(msg->title, "Is Authorized", msg->titleLength);
            return set_bool_ui(msg, ctx->tx.set_authorization.isAuthorized, "Is Authorized");
        default:
            PRINTF("Received an invalid screenIndex\n");
            return false;
    }
}

static bool handle_flash_loan(ethQueryContractUI_t *msg, context_t *ctx, uint8_t screenIndex) {
    switch (screenIndex) {
        case 0:
            strlcpy(msg->title, "Token", msg->titleLength);
            return set_address_ui(msg, &ctx->tx.flash_loan.token);
        case 1:
            strlcpy(msg->title, "Assets", msg->titleLength);
            return uint256_to_decimal(ctx->tx.flash_loan.assets.value,
                                      sizeof(ctx->tx.flash_loan.assets.value),
                                      msg->msg,
                                      msg->msgLength);
        case 2:
            return set_bytes32_ui(msg, &ctx->tx.flash_loan.data, "Data");
        default:
            PRINTF("Received an invalid screenIndex\n");
            return false;
    }
}

static bool handle_generic(ethQueryContractUI_t *msg, context_t *ctx, uint8_t screenIndex) {
    switch (screenIndex) {
        case 0:
            strlcpy(msg->title, "Assets", msg->titleLength);
            return uint256_to_decimal(ctx->tx.generic.assets.value,
                                      sizeof(ctx->tx.generic.assets.value),
                                      msg->msg,
                                      msg->msgLength);
        case 1:
            strlcpy(msg->title, "Shares", msg->titleLength);
            return uint256_to_decimal(ctx->tx.generic.shares.value,
                                      sizeof(ctx->tx.generic.shares.value),
                                      msg->msg,
                                      msg->msgLength);
        case 2:
            strlcpy(msg->title, "onBehalf", msg->titleLength);
            return set_address_ui(msg, &ctx->tx.generic.sender);
        default:
            PRINTF("Received an invalid screenIndex\n");
            return false;
    }
}

static bool handle_generic_2(ethQueryContractUI_t *msg, context_t *ctx, uint8_t screenIndex) {
    switch (screenIndex) {
        case 0:
            strlcpy(msg->title, "Assets", msg->titleLength);
            return uint256_to_decimal(ctx->tx.generic.assets.value,
                                      sizeof(ctx->tx.generic.assets.value),
                                      msg->msg,
                                      msg->msgLength);
        case 1:
            strlcpy(msg->title, "onBehalf", msg->titleLength);
            return set_address_ui(msg, &ctx->tx.generic.sender);
        default:
            PRINTF("Received an invalid screenIndex\n");
            return false;
    }
}

static bool handle_create_market(ethQueryContractUI_t *msg, context_t *ctx, uint8_t screenIndex) {
    switch (screenIndex) {
        case 0:
            strlcpy(msg->title, "Loan Token", msg->titleLength);
            return set_address_ui(msg, &ctx->tx.create_market.loan_token);
        case 1:
            strlcpy(msg->title, "Collateral Token", msg->titleLength);
            return set_address_ui(msg, &ctx->tx.create_market.collateral_token);
        default:
            PRINTF("Received an invalid screenIndex\n");
            return false;
    }
}

static bool handle_set_authorization_with_sig(ethQueryContractUI_t *msg,
                                              context_t *ctx,
                                              uint8_t screenIndex) {
    switch (screenIndex) {
        case 0:
            strlcpy(msg->title, "Authorizer", msg->titleLength);
            return set_address_ui(msg, &ctx->tx.set_authorization_with_sig.authorizer);
        case 1:
            strlcpy(msg->title, "Authorized", msg->titleLength);
            return set_address_ui(msg, &ctx->tx.set_authorization_with_sig.authorized);
        case 2:
            strlcpy(msg->title, "Is Authorized", msg->titleLength);
            return set_bool_ui(msg,
                               ctx->tx.set_authorization_with_sig.isAuthorized,
                               "Is Authorized");
        default:
            PRINTF("Received an invalid screenIndex\n");
            return false;
    }
}

static bool handle_reallocate(ethQueryContractUI_t *msg, context_t *ctx, uint8_t screenIndex) {
    switch (screenIndex) {
        case 0:
            strlcpy(msg->title, "Vault", msg->titleLength);
            return set_address_ui(msg, &ctx->tx.reallocate.vault);
        default:
            PRINTF("Received an invalid screenIndex\n");
            return false;
    }
}

/**
 * @brief Fucntion for ui showing. Calls specific function for each method
 *
 * @param msg: message context
 *
 */
void handle_query_contract_ui(ethQueryContractUI_t *msg) {
    context_t *context = (context_t *) msg->pluginContext;
    bool ret = false;

    // msg->title is the upper line displayed on the device.
    // msg->msg is the lower line displayed on the device.

    // Clean the display fields.
    memset(msg->title, 0, msg->titleLength);
    memset(msg->msg, 0, msg->msgLength);

    switch (context->selectorIndex) {
        case DEPOSIT:
            ret = handle_deposit(msg, context, msg->screenIndex);
            break;
        case APPROVE:
            ret = handle_approve(msg, context, msg->screenIndex);
            break;
        case REDEEM:
            ret = handle_redeem(msg, context, msg->screenIndex);
            break;
        case WITHDRAW:
            ret = handle_withdraw(msg, context, msg->screenIndex);
            break;
        case MINT:
            ret = handle_mint(msg, context, msg->screenIndex);
            break;
        case SET_AUTHORIZATION:
            ret = handle_set_authorization(msg, context, msg->screenIndex);
            break;
        case FLASH_LOAN:
            ret = handle_flash_loan(msg, context, msg->screenIndex);
            break;
        case BORROW:
        case REPAY:
        case WITHDRAW_BLUE:
        case SUPPLY:
            ret = handle_generic(msg, context, msg->screenIndex);
            break;
        case SUPPLY_COLLATERAL:
        case WITHDRAW_COLLATERAL:
            ret = handle_generic_2(msg, context, msg->screenIndex);
            break;
        case CREATE_MARKET:
            ret = handle_create_market(msg, context, msg->screenIndex);
            break;
        case SET_AUTHORIZATION_WITH_SIG:
            ret = handle_set_authorization_with_sig(msg, context, msg->screenIndex);
            break;
        case REALLOCATE:
            ret = handle_reallocate(msg, context, msg->screenIndex);
            break;
        default:
            PRINTF("Selector index: %d not supported\n", context->selectorIndex);
            ret = false;
    }
    msg->result = ret ? ETH_PLUGIN_RESULT_OK : ETH_PLUGIN_RESULT_ERROR;
}
