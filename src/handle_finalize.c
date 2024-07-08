#include "plugin.h"

void handle_finalize(ethPluginFinalize_t *msg) {
    context_t *context = (context_t *) msg->pluginContext;

    msg->uiType = ETH_UI_TYPE_GENERIC;

    // Number of screens for multicall is the number of calls.
    msg->numScreens = context->n_calls;

    msg->result = ETH_PLUGIN_RESULT_OK;
}
