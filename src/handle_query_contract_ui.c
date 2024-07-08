#include "plugin.h"

/**
 * @brief Format 32 bytes array to add ellipsis in the middle
 *
 * @param dst Destination array to save the result
 * @param dstLen Length of the destination array
 * @param src Source array to format
 * @param count Length of the source array
 * @param addEllipsis Add ellipsis in the middle boolean
 * @return true
 * @return false
 */
static bool array_to_hexstr(char *dst,
                            size_t dstLen,
                            const uint8_t *src,
                            uint16_t count,
                            bool addEllipsis) {
    memset(dst, 0, dstLen);
    if (dstLen < (count * 2 + 1)) {
        return false;
    }

    const char hexchars[] = "0123456789abcdef";
    int halfCount = count / 2;  // Calculate the middle point

    for (int i = 0; i < halfCount; i++, src++) {
        *dst++ = hexchars[*src >> 4u];
        *dst++ = hexchars[*src & 0x0Fu];
    }

    if (addEllipsis) {
        // Add "..." in the middle
        *dst++ = '.';
        *dst++ = '.';
        *dst++ = '.';
    }

    for (int i = halfCount; i < count; i++, src++) {
        *dst++ = hexchars[*src >> 4u];
        *dst++ = hexchars[*src & 0x0Fu];
    }

    *dst = 0;  // terminate string

    return true;
}

/**
 * @brief Set the bytes ui object
 *
 * @param msg msg context for intereact with eth app
 * @param array bytes to format
 * @param src_len length of the bytes
 * @param title title of the screen
 * @return true
 * @return false
 */
static bool set_bytes_ui(ethQueryContractUI_t *msg,
                         bytes32_t *array,
                         uint16_t src_len,
                         const char *title) {
    strlcpy(msg->title, title, msg->titleLength);

    return array_to_hexstr(msg->msg,
                           msg->msgLength,
                           array->value,
                           (src_len > PARAMETER_LENGTH) ? PARAMETER_LENGTH : src_len,
                           array->ellipsis);
}

void handle_query_contract_ui(ethQueryContractUI_t *msg) {
    context_t *context = (context_t *) msg->pluginContext;
    bool ret = false;

    // msg->title is the upper line displayed on the device.
    // msg->msg is the lower line displayed on the device.

    // Clean the display fields.
    memset(msg->title, 0, msg->titleLength);
    memset(msg->msg, 0, msg->msgLength);

    if (context->selectorIndex == MULTICALL) {
        if (msg->screenIndex < context->n_calls) {
            ret = set_bytes_ui(msg,
                               &context->call[msg->screenIndex],
                               context->call_len[msg->screenIndex],
                               "Call");
        } else {
            PRINTF("Received an invalid screenIndex\n");
            ret = false;
        }
    } else {
        PRINTF("Selector index: %d not supported\n", context->selectorIndex);
        ret = false;
    }
    msg->result = ret ? ETH_PLUGIN_RESULT_OK : ETH_PLUGIN_RESULT_ERROR;
}
