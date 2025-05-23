#include "plugin_utils.h"
#include "plugin.h"

// Called once to init.
void handle_init_contract(ethPluginInitContract_t *msg) {
    // Make sure we are running a compatible version.
    if (msg->interfaceVersion != ETH_PLUGIN_INTERFACE_VERSION_LATEST) {
        // If not the case, return the `UNAVAILABLE` status.
        msg->result = ETH_PLUGIN_RESULT_UNAVAILABLE;
        return;
    }

    // Double check that the `context_t` struct is not bigger than the maximum size (defined by
    // `msg->pluginContextLength`).
    if (msg->pluginContextLength < sizeof(context_t)) {
        PRINTF("Plugin parameters structure is bigger than allowed size\n");
        msg->result = ETH_PLUGIN_RESULT_ERROR;
        return;
    }

    context_t *context = (context_t *) msg->pluginContext;

    // Initialize the context (to 0).
    memset(context, 0, sizeof(*context));

    size_t index;
    if (!find_selector(U4BE(msg->selector, 0), SELECTORS, SELECTOR_COUNT, &index)) {
        PRINTF("Error: selector not found!\n");
        msg->result = ETH_PLUGIN_RESULT_UNAVAILABLE;
        return;
    }
    context->selectorIndex = index;
    // check for overflow
    if ((size_t) context->selectorIndex != index) {
        PRINTF("Error: overflow detected on selector index!\n");
        msg->result = ETH_PLUGIN_RESULT_ERROR;
        return;
    }

    // Set `next_param` to be the first field we expect to parse.
    switch (context->selectorIndex) {
        case DEPOSIT:
            context->next_param = AMOUNT;
            break;
        case APPROVE:
            context->next_param = SPENDER;
            break;
        case REDEEM:
            context->next_param = SHARES;
            break;
        case WITHDRAW:
            context->next_param = AMOUNT;
            break;
        case MINT:
            context->next_param = SHARES;
            break;
        case SET_AUTHORIZATION:
            context->next_param = ADDRESS;
            break;
        case FLASH_LOAN:
            context->next_param = TOKEN;
            break;
        case BORROW:
        case REPAY:
        case WITHDRAW_BLUE:
        case SUPPLY:
            context->next_param = LOAN_TOKEN;
            break;
        case SUPPLY_COLLATERAL:
        case WITHDRAW_COLLATERAL:
            context->next_param = TUPPLE_1;
            break;
        case CREATE_MARKET:
            context->next_param = LOAN_TOKEN;
            break;
        case SET_AUTHORIZATION_WITH_SIG:
            context->next_param = AUTHORIZER;
            break;
        case REALLOCATE:
            context->next_param = VAULT;
            break;
        default:
            PRINTF("Missing selectorIndex: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }

    // Return valid status.
    msg->result = ETH_PLUGIN_RESULT_OK;
}
