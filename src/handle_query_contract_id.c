#include "plugin.h"

// Sets the first screen to display.
void handle_query_contract_id(ethQueryContractID_t *msg) {
    const context_t *context = (const context_t *) msg->pluginContext;
    // msg->name will be the upper sentence displayed on the screen.
    // msg->version will be the lower sentence displayed on the screen.

    // For the first screen, display the plugin name.
    strlcpy(msg->name, APPNAME, msg->nameLength);

    switch (context->selectorIndex) {
        case DEPOSIT:
            strlcpy(msg->version, "Deposit", msg->versionLength);
            break;
        case MINT:
            strlcpy(msg->version, "Mint", msg->versionLength);
            break;
        case REDEEM:
            strlcpy(msg->version, "Redeem", msg->versionLength);
            break;
        case WITHDRAW:
            strlcpy(msg->version, "Withdraw", msg->versionLength);
            break;
        case APPROVE:
            strlcpy(msg->version, "Approve", msg->versionLength);
            break;
        case SET_AUTHORIZATION:
            strlcpy(msg->version, "Set Authorization", msg->versionLength);
            break;
        case FLASH_LOAN:
            strlcpy(msg->version, "Flash Loan", msg->versionLength);
            break;
        default:
            PRINTF("Selector Index not supported: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
    msg->result = ETH_PLUGIN_RESULT_OK;
}
