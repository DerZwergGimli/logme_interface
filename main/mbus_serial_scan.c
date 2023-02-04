#include "mbus_serial_scan.h"

#include "driver/uart.h"
#include "mbus-json.h"

#define ECHO_TEST_TXD  (4)
#define ECHO_TEST_RXD  (5)
#define BUF_SIZE (1024)
#define MAXFRAMES 16

static int
init_slaves(mbus_handle *handle, int address) {
    if (CONFIG_LOGME_MBUS_DEBUG)
        printf("%s: debug: sending init frame #1\n", __PRETTY_FUNCTION__);

    if (mbus_send_ping_frame(handle, address, 1) == -1) {
        return 0;
    }

    //
    // resend SND_NKE, maybe the first get lost
    //

    if (CONFIG_LOGME_MBUS_DEBUG)
        printf("%s: debug: sending init frame #2\n", __PRETTY_FUNCTION__);

    if (mbus_send_ping_frame(handle, address, 1) == -1) {
        return 0;
    }

    return 1;
}

int ping_address(mbus_handle *handle, mbus_frame *reply, int address) {
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

//------------------------------------------------------------------------------
// Primary addressing scanning of mbus devices.
//------------------------------------------------------------------------------
int
mbus_scan_old() {
    mbus_frame reply;
    mbus_frame_data reply_data;
    memset((void *) &reply_data, 0, sizeof(mbus_frame_data));

    mbus_handle *handle;
    char *addr_str, *xml_result;
    char *device = "/dev/usb1";
    int address = 1, retries = 0;
    long baudrate = 2400;
    int ret;


    memset((void *) &reply, 0, sizeof(mbus_frame));

    mbus_handle *mbus_init = (mbus_handle *) malloc(sizeof(mbus_handle));
    mbus_serial_wakeup(mbus_init);

    if ((handle = mbus_context_serial(device)) == NULL) {
        fprintf(stderr, "Scan failed: Could not initialize M-Bus context: %s\n", mbus_error_str());
        return 1;
    }

    if (CONFIG_LOGME_MBUS_DEBUG) {
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


    if (init_slaves(handle, address) == 0) {
        mbus_disconnect(handle);
        mbus_context_free(handle);
        return 1;
    }


    //ESP_LOGI("SERIAL_WAKEUP", "Sending serial wakeup!");
    //const char wakeup[] = {0x10, 0x40, 0x01, 0x41, 0x16};
    //const size_t wakeup_len = sizeof(wakeup);
    //uart_write_bytes(UART_NUM_1, wakeup, wakeup_len);
//
//    if (mbus_send_request_frame(handle, address) == -1) {
//        fprintf(stderr, "Failed to send M-Bus request frame.\n");
//        mbus_disconnect(handle);
//        mbus_context_free(handle);
//        return 1;
//    }

//    if (mbus_send_request_frame(handle, address) == -1) {
//        fprintf(stderr, "Failed to send M-Bus request frame.\n");
//        mbus_disconnect(handle);
//        mbus_context_free(handle);
//        return 1;
//    }
//
//    if (mbus_recv_frame(handle, &reply) != MBUS_RECV_RESULT_OK) {
//        fprintf(stderr, "Failed to receive M-Bus response frame.\n");
//        return 1;
//    }
    int maxframes = MAXFRAMES;
    if (mbus_sendrecv_request(handle, address, &reply, maxframes) != 0) {
        fprintf(stderr, "Failed to send/receive M-Bus request.\n");
        mbus_disconnect(handle);
        mbus_context_free(handle);
        mbus_frame_free(reply.next);
        return 1;
    }

    //
    // dump hex data if debug is true
    //
    if (CONFIG_LOGME_MBUS_DEBUG) {
        mbus_frame_print(&reply);
    }

    //
    // generate JSON and print to standard output
    //

    if ((xml_result = mbus_frame_json(&reply)) == NULL) {
        fprintf(stderr, "Failed to generate JSON representation of MBUS frames: %s\n", mbus_error_str());
        mbus_disconnect(handle);
        mbus_context_free(handle);
        mbus_frame_free(reply.next);
        return 1;
    }


    printf("%s", xml_result);
    free(xml_result);

//
//    //
//    // generate XML and print to standard output
//    //
//
    if ((xml_result = mbus_frame_xml(&reply)) == NULL) {
        fprintf(stderr, "Failed to generate XML representation of MBUS frames: %s\n", mbus_error_str());
        mbus_disconnect(handle);
        mbus_context_free(handle);
        mbus_frame_free(reply.next);
        return 1;
    }
//
//
//    printf("%s", xml_result);
//    free(xml_result);

    mbus_disconnect(handle);
    mbus_context_free(handle);
    mbus_frame_free(reply.next);

    return 0;
}
