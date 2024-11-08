#include "plugin.h"

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
        default:
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }

    msg->result = ETH_PLUGIN_RESULT_OK;
}
