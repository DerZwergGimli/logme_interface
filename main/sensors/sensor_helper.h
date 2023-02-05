//
// Interface for Sensor-Helper
//

#ifndef LOGME_INTERFACE_SENSOR_HELPER_C_H
#define LOGME_INTERFACE_SENSOR_HELPER_C_H

#include <string.h>
#include "esp_err.h"
#include "mbus_serial_scan.h"

static esp_err_t mbus_init_slaves(mbus_handle *handle, int address) {
    if (CONFIG_LOGME_MBUS_DEBUG)
        printf("%s: debug: sending init frame #1\n", __PRETTY_FUNCTION__);

    if (mbus_send_ping_frame(handle, address, 1) == -1) {
        return ESP_OK;
    }

    //
    // resend SND_NKE, maybe the first get lost
    //

    if (CONFIG_LOGME_MBUS_DEBUG)
        printf("%s: debug: sending init frame #2\n", __PRETTY_FUNCTION__);

    if (mbus_send_ping_frame(handle, address, 1) == -1) {
        return ESP_OK;
    }

    return 1;
}

static esp_err_t mbus_ping_address(mbus_handle *handle, mbus_frame *reply, int address) {
    int i, ret = MBUS_RECV_RESULT_ERROR;

    memset((void *) reply, 0, sizeof(mbus_frame));

    for (i = 0; i <= handle->max_search_retry; i++) {
        if (CONFIG_LOGME_MBUS_DEBUG) {
            printf("%d ", address);
            fflush(stdout);
        }

        if (mbus_send_ping_frame(handle, address, 0) == -1) {
            fprintf(stderr, "Scan failed. Could not send ping frame: %s\n", mbus_error_str());
            return MBUS_RECV_RESULT_ERROR;
        }

        ret = mbus_recv_frame(handle, reply);

        if (ret != MBUS_RECV_RESULT_TIMEOUT) {
            return ret;
        }
    }

    return ret;
}

int mbus_scan_full(int rx_pin, int tx_pin, long baudrate);


int mbus_request_short(char **json_result, int rx_pin, int tx_pin, long baudrate, int address);

int mbus_request_full(char **json_result, int rx_pin, int tx_pin, long baudrate, int address);


#endif //LOGME_INTERFACE_SENSOR_HELPER_C_H
