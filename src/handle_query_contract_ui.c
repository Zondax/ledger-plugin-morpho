#include "plugin.h"

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
        THROW(EXCEPTION_OVERFLOW);
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

/**
 * @brief UI for depositIntoStrategy selector
 *
 * @param msg: message containing the parameter
 * @param ctx: context with provide_parameter data
 * @param screenIndex: index of the screen to display
 *
 */
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
        default:
            PRINTF("Selector index: %d not supported\n", context->selectorIndex);
            ret = false;
    }
    msg->result = ret ? ETH_PLUGIN_RESULT_OK : ETH_PLUGIN_RESULT_ERROR;
}
