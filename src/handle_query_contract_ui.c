#include "plugin.h"

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
    if (msg->msgLength < 42) {
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
        default:
            PRINTF("Selector index: %d not supported\n", context->selectorIndex);
            ret = false;
    }
    msg->result = ret ? ETH_PLUGIN_RESULT_OK : ETH_PLUGIN_RESULT_ERROR;
}
