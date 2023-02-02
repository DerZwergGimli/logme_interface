#include "mbus_serial_scan.h"

#include "driver/uart.h"

#define ECHO_TEST_TXD  (4)
#define ECHO_TEST_RXD  (5)
#define BUF_SIZE (1024)

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


    mbus_handle *handle;
    char *device = "/dev/usb1";
    int address, retries = 0;
    long baudrate = 9600;
    int ret;
    debug = 1;


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

    if (debug)
        printf("Scanning primary addresses:\n");

    for (address = 0; address <= MBUS_MAX_PRIMARY_SLAVES; address++) {
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
    }

    mbus_disconnect(handle);
    mbus_context_free(handle);
    return 0;
}
