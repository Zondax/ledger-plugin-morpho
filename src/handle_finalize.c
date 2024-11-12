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
            msg->numScreens = 2;
            break;
        case MINT:
            msg->numScreens = 2;
            break;
        case REDEEM:
            msg->numScreens = 3;
            break;
        case WITHDRAW:
            msg->numScreens = 3;
            break;
        case APPROVE:
            msg->numScreens = 2;
            break;
        case SET_AUTHORIZATION:
            msg->numScreens = 2;
            break;
        case FLASH_LOAN:
            msg->numScreens = 3;
            break;
        case BORROW:
            msg->numScreens = 3;
            break;
        default:
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }

    msg->result = ETH_PLUGIN_RESULT_OK;
}
