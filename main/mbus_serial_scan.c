#include "mbus_serial_scan.h"

#include "driver/uart.h"

#define ECHO_TEST_TXD  (4)
#define ECHO_TEST_RXD  (5)
#define BUF_SIZE (1024)

static int
init_slaves(mbus_handle *handle) {
    if (debug)
        printf("%s: debug: sending init frame #1\n", __PRETTY_FUNCTION__);

    if (mbus_send_ping_frame(handle, MBUS_ADDRESS_NETWORK_LAYER, 1) == -1) {
        return 0;
    }

    //
    // resend SND_NKE, maybe the first get lost
    //

    if (debug)
        printf("%s: debug: sending init frame #2\n", __PRETTY_FUNCTION__);

    if (mbus_send_ping_frame(handle, MBUS_ADDRESS_NETWORK_LAYER, 1) == -1) {
        return 0;
    }

    return 1;
}

int ping_address(mbus_handle *handle, mbus_frame *reply, int address) {
    int i, ret = MBUS_RECV_RESULT_ERROR;

    memset((void *) reply, 0, sizeof(mbus_frame));

    for (i = 0; i <= handle->max_search_retry; i++) {
        if (debug) {
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

//------------------------------------------------------------------------------
// Primary addressing scanning of mbus devices.
//------------------------------------------------------------------------------
int
mbus_scan() {
    mbus_frame reply;
    mbus_frame_data reply_data;
    memset((void *) &reply_data, 0, sizeof(mbus_frame_data));

    mbus_handle *handle;
    char *addr_str, *xml_result;
    char *device = "/dev/usb1";
    int address = 1, retries = 0;
    long baudrate = 2400;
    int ret;
    debug = 1;

    memset((void *) &reply, 0, sizeof(mbus_frame));

    if ((handle = mbus_context_serial(device)) == NULL) {
        fprintf(stderr, "Scan failed: Could not initialize M-Bus context: %s\n", mbus_error_str());
        return 1;
    }

    if (debug) {
        mbus_register_send_event(handle, &mbus_dump_send_event);
        mbus_register_recv_event(handle, &mbus_dump_recv_event);
    }

    if (mbus_connect(handle) == -1) {
        fprintf(stderr, "Scan failed: Could not setup connection to M-bus gateway: %s\n", mbus_error_str());
        return 1;
    }


    if (mbus_context_set_option(handle, MBUS_OPTION_MAX_SEARCH_RETRY, retries) == -1) {
        fprintf(stderr, "Failed to set retry count\n");
        return 1;
    }

    if (mbus_serial_set_baudrate(handle, baudrate) == -1) {
        fprintf(stderr, "Failed to set baud rate.\n");
        return 1;
    }

    mbus_serial_wakeup(handle);

    //ESP_LOGI("SERIAL_WAKEUP", "Sending serial wakeup!");
    //const char wakeup[] = {0x10, 0x40, 0x01, 0x41, 0x16};
    //const size_t wakeup_len = sizeof(wakeup);
    //uart_write_bytes(UART_NUM_1, wakeup, wakeup_len);

    if (mbus_send_request_frame(handle, address) == -1) {
        fprintf(stderr, "Failed to send M-Bus request frame.\n");
        mbus_disconnect(handle);
        mbus_context_free(handle);
        return 1;
    }

    if (mbus_recv_frame(handle, &reply) != MBUS_RECV_RESULT_OK) {
        fprintf(stderr, "Failed to receive M-Bus response frame.\n");
        return 1;
    }

    //
    // dump hex data if debug is true
    //
    if (debug) {
        mbus_frame_print(&reply);
    }

    //
    // parse data
    //
    if (mbus_frame_data_parse(&reply, &reply_data) == -1) {
        fprintf(stderr, "M-bus data parse error: %s\n", mbus_error_str());
        mbus_disconnect(handle);
        mbus_context_free(handle);
        return 1;
    }

    //
    // generate XML and print to standard output
    //
    if ((xml_result = mbus_frame_data_xml(&reply_data)) == NULL) {
        fprintf(stderr, "Failed to generate XML representation of MBUS frame: %s\n", mbus_error_str());
        mbus_disconnect(handle);
        mbus_context_free(handle);
        return 1;
    }

    printf("%s", xml_result);
    free(xml_result);

    // manual free
    if (reply_data.data_var.record) {
        mbus_data_record_free(reply_data.data_var.record); // free's up the whole list
    }


    if (debug)
        printf("Scanning primary addresses:\n");

    for (address = 1; address <= 1; address++) {
        mbus_frame reply;

        ret = ping_address(handle, &reply, address);

        if (ret == MBUS_RECV_RESULT_TIMEOUT) {
            continue;
        }

        if (ret == MBUS_RECV_RESULT_INVALID) {
            /* check for more data (collision) */
            mbus_purge_frames(handle);
            printf("Collision at address %d\n", address);
            continue;
        }

        if (mbus_frame_type(&reply) == MBUS_FRAME_TYPE_ACK) {
            /* check for more data (collision) */
            if (mbus_purge_frames(handle)) {
                printf("Collision at address %d\n", address);
                continue;
            }

            printf("Found a M-Bus device at address %d\n", address);
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    mbus_disconnect(handle);
    mbus_context_free(handle);
    return 0;
}
