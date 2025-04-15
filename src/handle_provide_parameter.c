#include "plugin.h"

uint8_t bytes_missing = 0;

/**
 * @brief Copy the parameter to the destination
 *
 * @param dst
 * @param dst_len
 * @param max_len
 * @param src
 */
static void copy_text(uint8_t *dst, uint16_t dst_len, uint16_t max_len, const uint8_t *src) {
    size_t len = MIN(dst_len, max_len);
    memcpy(dst, src, len);
}

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

static void handle_set_authorization(ethPluginProvideParameter_t *msg, context_t *context) {
    switch (context->next_param) {
        case ADDRESS:
            copy_address(context->tx.set_authorization.address.value,
                         msg->parameter,
                         sizeof(context->tx.set_authorization.address.value));
            context->next_param = IS_AUTHORIZED;
            break;
        case IS_AUTHORIZED:
            if (!U2BE_from_parameter(msg->parameter, &context->tx.set_authorization.isAuthorized)) {
                msg->result = ETH_PLUGIN_RESULT_ERROR;
            }
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

static void handle_flash_loan(ethPluginProvideParameter_t *msg, context_t *context) {
    uint16_t containers = 0;  // group of 32 bytes needed to hold data

    if (context->go_to_offset) {
        if (msg->parameterOffset != context->offset) {
            return;
        }
        context->go_to_offset = false;
    }
    switch (context->next_param) {
        case TOKEN:
            copy_address(context->tx.flash_loan.token.value,
                         msg->parameter,
                         sizeof(context->tx.flash_loan.token.value));
            context->next_param = ASSETS;
            break;
        case ASSETS:
            copy_parameter(context->tx.flash_loan.assets.value,
                           msg->parameter,
                           sizeof(context->tx.flash_loan.assets.value));
            context->next_param = DATA_OFFSET;
            break;
        case DATA_OFFSET:
            if (!U2BE_from_parameter(msg->parameter, &context->tx.flash_loan.data_offset)) {
                msg->result = ETH_PLUGIN_RESULT_ERROR;
                return;
            }
            context->next_param = DATA_SIZE;
            break;
        case DATA_SIZE:
            if (msg->parameterOffset != context->tx.flash_loan.data_offset + SELECTOR_SIZE) {
                msg->result = ETH_PLUGIN_RESULT_ERROR;
                return;
            }

            if (!U2BE_from_parameter(msg->parameter, &context->tx.flash_loan.data_size)) {
                msg->result = ETH_PLUGIN_RESULT_ERROR;
                return;
            }
            context->next_param = DATA;
            break;
        case DATA:
            // Name has less then 32
            if (context->tx.flash_loan.data_size <= PARAMETER_LENGTH) {
                copy_text(context->tx.flash_loan.data.value,
                          context->tx.flash_loan.data_size,
                          PARAMETER_LENGTH,
                          msg->parameter);
                context->tx.flash_loan.data.ellipsis = false;
                context->next_param = NONE;
            } else {  // Name has more then 32 bytes
                containers = context->tx.flash_loan.data_size / PARAMETER_LENGTH;
                bytes_missing = context->tx.flash_loan.data_size % PARAMETER_LENGTH;
                context->tx.flash_loan.data.ellipsis = true;

                // copy first 16 bytes
                copy_text(context->tx.flash_loan.data.value,
                          context->tx.flash_loan.data_size,
                          HALF_PARAMETER_LENGTH,
                          msg->parameter);

                if (bytes_missing < HALF_PARAMETER_LENGTH) {
                    if (containers < 2) {  // only one container we still need bytes from this one
                                           // to complete the last 16. then go to last container
                        copy_text(context->tx.flash_loan.data.value + HALF_PARAMETER_LENGTH,
                                  context->tx.flash_loan.data_size - HALF_PARAMETER_LENGTH,
                                  HALF_PARAMETER_LENGTH - bytes_missing,
                                  msg->parameter + HALF_PARAMETER_LENGTH + bytes_missing);

                        context->offset = msg->parameterOffset + PARAMETER_LENGTH;
                        context->go_to_offset = true;
                        context->next_param = DATA_CONTAINER_2;
                    } else {  // more then 1 container go to second-last and get missing bytes to
                              // complete the last 16
                        context->offset =
                            msg->parameterOffset + (containers - 1) * PARAMETER_LENGTH;
                        context->go_to_offset = true;
                        context->next_param = DATA_CONTAINER_1;
                    }
                } else {  // last container has the last 16 bytes we need
                    context->offset = msg->parameterOffset + containers * PARAMETER_LENGTH;
                    context->go_to_offset = true;
                    context->next_param = DATA_CONTAINER_2;
                }
            }
            break;
        case DATA_CONTAINER_1:  // second last container
            copy_text(context->tx.flash_loan.data.value + HALF_PARAMETER_LENGTH,
                      context->tx.flash_loan.data_size - HALF_PARAMETER_LENGTH,
                      HALF_PARAMETER_LENGTH - bytes_missing,
                      msg->parameter + HALF_PARAMETER_LENGTH + bytes_missing);
            context->next_param = DATA_CONTAINER_2;
            break;
        case DATA_CONTAINER_2:                             // last container
            if (bytes_missing <= HALF_PARAMETER_LENGTH) {  // copy missing bytes
                copy_text(context->tx.flash_loan.data.value + HALF_PARAMETER_LENGTH +
                              (HALF_PARAMETER_LENGTH - bytes_missing),
                          context->tx.flash_loan.data_size - HALF_PARAMETER_LENGTH -
                              (HALF_PARAMETER_LENGTH - bytes_missing),
                          bytes_missing,
                          msg->parameter);
            } else {  // last container has 16 or more bytes, move the need offset to copy the last
                      // 16 bytes
                copy_text(context->tx.flash_loan.data.value + HALF_PARAMETER_LENGTH,
                          context->tx.flash_loan.data_size - HALF_PARAMETER_LENGTH,
                          HALF_PARAMETER_LENGTH,
                          msg->parameter + (bytes_missing - HALF_PARAMETER_LENGTH));
            }
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

static void handle_generic(ethPluginProvideParameter_t *msg, context_t *context) {
    switch (context->next_param) {
        case LOAN_TOKEN:
            copy_address(context->tx.generic.loan_token.value,
                         msg->parameter,
                         sizeof(context->tx.generic.loan_token.value));
            context->next_param = TUPPLE_2;
            break;
        case TUPPLE_2:
            context->next_param = TUPPLE_3;
            break;
        case TUPPLE_3:
            context->next_param = TUPPLE_4;
            break;
        case TUPPLE_4:
            context->next_param = TUPPLE_5;
            break;
        case TUPPLE_5:
            context->next_param = ASSETS;
            break;
        case ASSETS:
            copy_parameter(context->tx.generic.assets.value,
                           msg->parameter,
                           sizeof(context->tx.generic.assets.value));
            context->next_param = SHARES;
            break;
        case SHARES:
            copy_parameter(context->tx.generic.shares.value,
                           msg->parameter,
                           sizeof(context->tx.generic.shares.value));
            context->next_param = SENDER;
            break;
        case SENDER:
            copy_address(context->tx.generic.sender.value,
                         msg->parameter,
                         sizeof(context->tx.generic.sender.value));
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

static void handle_generic_2(ethPluginProvideParameter_t *msg, context_t *context) {
    switch (context->next_param) {
        case TUPPLE_1:
            context->next_param = COLLATERAL_TOKEN;
            break;
        case COLLATERAL_TOKEN:
            copy_address(context->tx.generic.collateral_token.value,
                         msg->parameter,
                         sizeof(context->tx.generic.collateral_token.value));
            context->next_param = TUPPLE_3;
            break;
        case TUPPLE_3:
            context->next_param = TUPPLE_4;
            break;
        case TUPPLE_4:
            context->next_param = TUPPLE_5;
            break;
        case TUPPLE_5:
            context->next_param = ASSETS;
            break;
        case ASSETS:
            copy_parameter(context->tx.generic.assets.value,
                           msg->parameter,
                           sizeof(context->tx.generic.assets.value));
            context->next_param = SENDER;
            break;
        case SENDER:
            copy_address(context->tx.generic.sender.value,
                         msg->parameter,
                         sizeof(context->tx.generic.sender.value));
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

static void handle_create_market(ethPluginProvideParameter_t *msg, context_t *context) {
    switch (context->next_param) {
        case LOAN_TOKEN:
            copy_address(context->tx.create_market.loan_token.value,
                         msg->parameter,
                         sizeof(context->tx.create_market.loan_token.value));
            context->next_param = COLLATERAL_TOKEN;
            break;
        case COLLATERAL_TOKEN:
            copy_address(context->tx.create_market.collateral_token.value,
                         msg->parameter,
                         sizeof(context->tx.create_market.collateral_token.value));
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

static void handle_set_authorization_with_sig(ethPluginProvideParameter_t *msg,
                                              context_t *context) {
    switch (context->next_param) {
        case AUTHORIZER:
            copy_address(context->tx.set_authorization_with_sig.authorizer.value,
                         msg->parameter,
                         sizeof(context->tx.set_authorization_with_sig.authorizer.value));
            context->next_param = AUTHORIZED;
            break;
        case AUTHORIZED:
            copy_address(context->tx.set_authorization_with_sig.authorized.value,
                         msg->parameter,
                         sizeof(context->tx.set_authorization_with_sig.authorized.value));
            context->next_param = IS_AUTHORIZED;
            break;
        case IS_AUTHORIZED:
            if (!U2BE_from_parameter(msg->parameter,
                                     &context->tx.set_authorization_with_sig.isAuthorized)) {
                msg->result = ETH_PLUGIN_RESULT_ERROR;
            }
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

static void handle_reallocate(ethPluginProvideParameter_t *msg, context_t *context) {
    switch (context->next_param) {
        case VAULT:
            copy_address(context->tx.reallocate.vault.value,
                         msg->parameter,
                         sizeof(context->tx.reallocate.vault.value));
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
        case SET_AUTHORIZATION:
            handle_set_authorization(msg, context);
            break;
        case FLASH_LOAN:
            handle_flash_loan(msg, context);
            break;
        case BORROW:
        case REPAY:
        case WITHDRAW_BLUE:
        case SUPPLY:
            handle_generic(msg, context);
            break;
        case SUPPLY_COLLATERAL:
        case WITHDRAW_COLLATERAL:
            handle_generic_2(msg, context);
            break;
        case CREATE_MARKET:
            handle_create_market(msg, context);
            break;
        case SET_AUTHORIZATION_WITH_SIG:
            handle_set_authorization_with_sig(msg, context);
            break;
        case REALLOCATE:
            handle_reallocate(msg, context);
            break;
        default:
            PRINTF("Selector Index not supported: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}
