#include "plugin.h"

uint8_t bytes_missing = 0;

static void copy_text(uint8_t *dst, uint16_t dst_len, uint16_t max_len, const uint8_t *src) {
    size_t len = MIN(dst_len, max_len);
    memcpy(dst, src, len);
}

static void handle_multicall(ethPluginProvideParameter_t *msg, context_t *context) {
    uint16_t containers = 0;  // group of 32 bytes needed to hold name
    uint16_t tmp_offset = 0;

    if (context->go_to_offset) {
        if (msg->parameterOffset != context->offset) {
            return;
        }
        context->go_to_offset = false;
    }
    switch (context->next_param) {
        case OFFSET:
            // save temporarily the offset to check on the next parameter we are indeed in the right
            // offset
            if (!U2BE_from_parameter(msg->parameter, &context->offsets_start)) {
                msg->result = ETH_PLUGIN_RESULT_ERROR;
                return;
            }
            context->next_param = N_CALL;
            break;
        case N_CALL:
            if (msg->parameterOffset != context->offsets_start + SELECTOR_SIZE) {
                msg->result = ETH_PLUGIN_RESULT_ERROR;
                return;
            }

            if (!U2BE_from_parameter(msg->parameter, &context->n_calls) || context->n_calls > 3 ||
                context->n_calls == 0) {
                msg->result = ETH_PLUGIN_RESULT_ERROR;
            }
            context->next_param = OFFSETS;
            break;
        case OFFSETS:
            if (!U2BE_from_parameter(msg->parameter, &tmp_offset)) {
                msg->result = ETH_PLUGIN_RESULT_ERROR;
                return;
            }

            // save offsets start point
            if (context->id == 0) {
                context->offsets_start = msg->parameterOffset;
            }
            // save offset and if not last one save next
            context->offsets[context->id] = tmp_offset;
            context->id++;
            context->next_param = OFFSETS;
            if (context->id == context->n_calls) {
                context->id = 0;
                context->offset = context->offsets_start + context->offsets[0];
                context->go_to_offset = true;
                context->next_param = CALL_LEN;
            }
            break;
        case CALL_LEN:
            if (!U2BE_from_parameter(msg->parameter, &context->call_len[context->id])) {
                msg->result = ETH_PLUGIN_RESULT_ERROR;
                return;
            }
            context->next_param = CALL;
            break;
        case CALL:
            // Name has less then 32
            if (context->call_len[context->id] <= PARAMETER_LENGTH) {
                copy_text(context->call[context->id].value,
                          sizeof(context->call[context->id].value),
                          PARAMETER_LENGTH,
                          msg->parameter);
                context->call[context->id].ellipsis = false;
                if (context->id == context->n_calls - 1) {
                    context->id = 0;
                    context->next_param = NONE;
                } else {
                    context->id++;
                    context->offset = context->offsets_start + context->offsets[context->id];
                    context->go_to_offset = true;
                    context->next_param = CALL_LEN;
                }
            } else {  // Name has more then 32 bytes
                containers = context->call_len[context->id] / PARAMETER_LENGTH;
                bytes_missing = context->call_len[context->id] % PARAMETER_LENGTH;
                context->call[context->id].ellipsis = true;
                // copy first 16 bytes
                copy_text(context->call[context->id].value,
                          sizeof(context->call[context->id].value),
                          HALF_PARAMETER_LENGTH,
                          msg->parameter);

                if (bytes_missing < HALF_PARAMETER_LENGTH) {
                    if (containers < 2) {  // only one container we still need bytes from this one
                                           // to complete the last 16. then go to last container
                        copy_text(context->call[context->id].value + HALF_PARAMETER_LENGTH,
                                  sizeof(context->call[context->id].value) - HALF_PARAMETER_LENGTH,
                                  HALF_PARAMETER_LENGTH - bytes_missing,
                                  msg->parameter + HALF_PARAMETER_LENGTH + bytes_missing);

                        context->offset = msg->parameterOffset + PARAMETER_LENGTH;
                        context->go_to_offset = true;
                        context->next_param = CALL_2;
                    } else {  // more then 1 container go to second-last and get missing bytes to
                              // complete the last 16
                        context->offset =
                            msg->parameterOffset + (containers - 1) * PARAMETER_LENGTH;
                        context->go_to_offset = true;
                        context->next_param = CALL_1;
                    }
                } else {  // last container has the last 16 bytes we need
                    context->offset = msg->parameterOffset + containers * PARAMETER_LENGTH;
                    context->go_to_offset = true;
                    context->next_param = CALL_2;
                }
            }
            break;
        case CALL_1:  // second last container
            copy_text(context->call[context->id].value + HALF_PARAMETER_LENGTH,
                      sizeof(context->call[context->id].value) - HALF_PARAMETER_LENGTH,
                      HALF_PARAMETER_LENGTH - bytes_missing,
                      msg->parameter + HALF_PARAMETER_LENGTH + bytes_missing);
            context->next_param = CALL_2;
            break;
        case CALL_2:                                       // last container
            if (bytes_missing <= HALF_PARAMETER_LENGTH) {  // copy missing bytes
                copy_text(context->call[context->id].value + HALF_PARAMETER_LENGTH +
                              (HALF_PARAMETER_LENGTH - bytes_missing),
                          sizeof(context->call[context->id].value) - HALF_PARAMETER_LENGTH +
                              (HALF_PARAMETER_LENGTH - bytes_missing),
                          bytes_missing,
                          msg->parameter);
            } else {  // last container has 16 or more bytes, move the need offset to copy the last
                      // 16 bytes
                copy_text(context->call[context->id].value + HALF_PARAMETER_LENGTH,
                          sizeof(context->call[context->id].value) - HALF_PARAMETER_LENGTH,
                          HALF_PARAMETER_LENGTH,
                          msg->parameter + (bytes_missing - HALF_PARAMETER_LENGTH));
            }

            if (context->id == context->n_calls - 1) {
                context->id = 0;
                context->next_param = NONE;
            } else {
                context->id++;
                context->offset = context->offsets_start + context->offsets[context->id];
                context->go_to_offset = true;
                context->next_param = CALL_LEN;
            }
            break;
        case NONE:
            break;
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

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
        case MULTICALL:
            handle_multicall(msg, context);
            break;
        default:
            PRINTF("Selector Index not supported: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}
