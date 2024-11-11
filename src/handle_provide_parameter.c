#include "plugin.h"

static void handle_deposit(ethPluginProvideParameter_t *msg, context_t *context) {
    switch (context->next_param) {
        case AMOUNT:
            copy_parameter(context->tx.deposit.assets.value,
                           msg->parameter,
                           sizeof(context->tx.deposit.assets.value));
            context->next_param = RECEIVER;
            break;
        case RECEIVER:
            copy_address(context->tx.deposit.receiver.value,
                         msg->parameter,
                         sizeof(context->tx.deposit.receiver.value));
            context->next_param = NONE;
            break;
        case NONE:
            break;
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

static void handle_approve(ethPluginProvideParameter_t *msg, context_t *context) {
    switch (context->next_param) {
        case SPENDER:
            copy_address(context->tx.approve.spender.value,
                         msg->parameter,
                         sizeof(context->tx.approve.spender.value));

            context->next_param = SHARES;
            break;
        case SHARES:
            copy_parameter(context->tx.approve.shares.value,
                           msg->parameter,
                           sizeof(context->tx.approve.shares.value));
            context->next_param = NONE;
            break;
        case NONE:
            break;
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

static void handle_withdraw(ethPluginProvideParameter_t *msg, context_t *context) {
    switch (context->next_param) {
        case AMOUNT:
            copy_parameter(context->tx.withdraw.assets.value,
                           msg->parameter,
                           sizeof(context->tx.withdraw.assets.value));
            context->next_param = RECEIVER;
            break;
        case RECEIVER:
            copy_address(context->tx.withdraw.receiver.value,
                         msg->parameter,
                         sizeof(context->tx.withdraw.receiver.value));
            context->next_param = OWNER;
            break;
        case OWNER:
            copy_address(context->tx.withdraw.owner.value,
                         msg->parameter,
                         sizeof(context->tx.withdraw.owner.value));
            context->next_param = NONE;
            break;
        case NONE:
            break;
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

static void handle_redeem(ethPluginProvideParameter_t *msg, context_t *context) {
    switch (context->next_param) {
        case SHARES:
            copy_parameter(context->tx.redeem.shares.value,
                           msg->parameter,
                           sizeof(context->tx.redeem.shares.value));
            context->next_param = RECEIVER;
            break;
        case RECEIVER:
            copy_address(context->tx.redeem.receiver.value,
                         msg->parameter,
                         sizeof(context->tx.redeem.receiver.value));
            context->next_param = OWNER;
            break;
        case OWNER:
            copy_address(context->tx.redeem.owner.value,
                         msg->parameter,
                         sizeof(context->tx.redeem.owner.value));
            context->next_param = NONE;
            break;
        case NONE:
            break;
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

static void handle_mint(ethPluginProvideParameter_t *msg, context_t *context) {
    switch (context->next_param) {
        case SHARES:
            copy_parameter(context->tx.mint.shares.value,
                           msg->parameter,
                           sizeof(context->tx.mint.shares.value));
            context->next_param = RECEIVER;
            break;
        case RECEIVER:
            copy_address(context->tx.mint.receiver.value,
                         msg->parameter,
                         sizeof(context->tx.mint.receiver.value));
            context->next_param = NONE;
            break;
        case NONE:
            break;
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

/**
 * @brief Function to parse the important parameters of the call. Calls a specific function for each
 * method
 *
 * @param msg msg context
 */
void handle_provide_parameter(ethPluginProvideParameter_t *msg) {
    context_t *context = (context_t *) msg->pluginContext;
    // We use `%.*H`: it's a utility function to print bytes. You first give
    // the number of bytes you wish to print (in this case, `PARAMETER_LENGTH`) and then
    // the address (here `msg->parameter`).
    PRINTF("plugin provide parameter: offset %d\nBytes: %.*H\n",
           msg->parameterOffset,
           PARAMETER_LENGTH,
           msg->parameter);

    msg->result = ETH_PLUGIN_RESULT_OK;

    switch (context->selectorIndex) {
        case DEPOSIT:
            handle_deposit(msg, context);
            break;
        case APPROVE:
            handle_approve(msg, context);
            break;
        case REDEEM:
            handle_redeem(msg, context);
            break;
        case WITHDRAW:
            handle_withdraw(msg, context);
            break;
        case MINT:
            handle_mint(msg, context);
            break;
        default:
            PRINTF("Selector Index not supported: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}
