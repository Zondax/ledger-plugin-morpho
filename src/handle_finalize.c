#include "plugin.h"

/**
 * @brief Function to determine the number of screens needed for each operation
 *
 * @param msg  msg context
 */
void handle_finalize(ethPluginFinalize_t *msg) {
    context_t *context = (context_t *) msg->pluginContext;

    msg->uiType = ETH_UI_TYPE_GENERIC;

    // The total number of screen you will need.
    switch (context->selectorIndex) {
        case DEPOSIT:
        case MINT:
        case APPROVE:
        case SET_AUTHORIZATION:
        case SUPPLY_COLLATERAL:
        case WITHDRAW_COLLATERAL:
            msg->numScreens = 2;
            break;
        case REDEEM:
        case WITHDRAW:
        case FLASH_LOAN:
        case BORROW:
        case REPAY:
        case WITHDRAW_BLUE:
        case SUPPLY:
            msg->numScreens = 3;
            break;
        default:
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }

    msg->result = ETH_PLUGIN_RESULT_OK;
}
