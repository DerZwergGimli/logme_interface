//
// Implementation for Sensor-Helper
//

#include "sensor_helper.h"
#include "mbus-json.h"
#include "mqtt_manager.h"


int mbus_scan_full(int rx_pin, int tx_pin, long baudrate) {

    mbus_frame reply;
    mbus_frame_data reply_data;
    memset((void *) &reply_data, 0, sizeof(mbus_frame_data));

    mbus_handle *handle;
    int address, retries = 0;

    int ret;

    memset((void *) &reply, 0, sizeof(mbus_frame));


    if ((handle = mbus_context_serial(rx_pin, tx_pin)) == NULL) {
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
    if (CONFIG_LOGME_MBUS_DEBUG)
        printf("Scanning primary addresses:\n");

    int found_slaves = 0;
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
            found_slaves++;
            printf("Found a M-Bus device at address %d\n", address);
        }
    }


    // Clean up
    mbus_disconnect(handle);
    mbus_context_free(handle);
    mbus_frame_free(reply.next);

    return found_slaves;

}

int mbus_request_full(char **json_result, int rx_pin, int tx_pin, long baudrate, int address) {
    mbus_frame reply;
    mbus_frame_data reply_data;
    memset((void *) &reply_data, 0, sizeof(mbus_frame_data));

    mbus_handle *handle;
    int retries = 0;


    memset((void *) &reply, 0, sizeof(mbus_frame));

    mbus_handle *mbus_init = (mbus_handle *) malloc(sizeof(mbus_handle));
    mbus_serial_wakeup(mbus_init);

    if ((handle = mbus_context_serial(rx_pin, tx_pin)) == NULL) {
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


    if (mbus_init_slaves(handle, address) == 0) {
        mbus_disconnect(handle);
        mbus_context_free(handle);
        return 1;
    }


    if (mbus_sendrecv_request(handle, address, &reply, CONFIG_LOGME_MBUS_MAXFRAMES) != 0) {
        fprintf(stderr, "Failed to send/receive M-Bus request.\n");
        mbus_disconnect(handle);
        mbus_context_free(handle);
        mbus_frame_free(reply.next);
        return 1;
    }

    // dump hex data if debug is true
    if (CONFIG_LOGME_MBUS_DEBUG) {
        mbus_frame_print(&reply);
    }

    // generate JSON and print to standard output
    if ((*json_result = mbus_frame_json(&reply)) == NULL) {
        fprintf(stderr, "Failed to generate JSON representation of MBUS frames: %s\n", mbus_error_str());
        mbus_disconnect(handle);
        mbus_context_free(handle);
        mbus_frame_free(reply.next);
        return 1;
    }


    // Clean up
    mbus_disconnect(handle);
    mbus_context_free(handle);
    mbus_frame_free(reply.next);

    return 0;
}

int mbus_request_short(char **json_result, int rx_pin, int tx_pin, long baudrate, int address) {
    mbus_frame reply;
    mbus_frame_data reply_data;

    memset((void *) &reply, 0, sizeof(mbus_frame));
    memset((void *) &reply_data, 0, sizeof(mbus_frame_data));

    mbus_handle *handle;
    int retries = 0;


    mbus_handle *mbus_init = (mbus_handle *) malloc(sizeof(mbus_handle));
    mbus_serial_wakeup(mbus_init);

    if ((handle = mbus_context_serial(rx_pin, tx_pin)) == NULL) {
        fprintf(stderr, "Scan failed: Could not initialize M-Bus context: %s\n", mbus_error_str());

        mbus_disconnect(handle);
        mbus_context_free(handle);
        mbus_frame_free(reply.next);
        return 1;
    }

    if (CONFIG_LOGME_MBUS_DEBUG) {
        mbus_register_send_event(handle, &mbus_dump_send_event);
        mbus_register_recv_event(handle, &mbus_dump_recv_event);
    }

    if (mbus_connect(handle) == -1) {
        fprintf(stderr, "Could not setup connection to M-bus gateway: %s\n", mbus_error_str());

        mbus_disconnect(handle);
        mbus_context_free(handle);
        mbus_frame_free(reply.next);
        return 1;
    }


    if (mbus_context_set_option(handle, MBUS_OPTION_MAX_SEARCH_RETRY, retries) == -1) {
        fprintf(stderr, "Failed to set retry count\n");

        mbus_disconnect(handle);
        mbus_context_free(handle);
        mbus_frame_free(reply.next);
        return 1;
    }

    if (mbus_serial_set_baudrate(handle, baudrate) == -1) {
        fprintf(stderr, "Failed to set baud rate.\n");
    }


    if (mbus_init_slaves(handle, address) == 0) {
        mbus_disconnect(handle);
        mbus_context_free(handle);
        mbus_frame_free(reply.next);
        return 1;
    }

    if (mbus_send_request_frame(handle, address) == -1) {
        fprintf(stderr, "Failed to send M-Bus request frame.\n");
        mbus_disconnect(handle);
        mbus_context_free(handle);
        mbus_frame_free(reply.next);
        return 1;
    }

    if (mbus_recv_frame(handle, &reply) != MBUS_RECV_RESULT_OK) {
        fprintf(stderr, "Failed to receive M-Bus response frame.\n");
        mbus_disconnect(handle);
        mbus_context_free(handle);
        mbus_frame_free(reply.next);
        return 1;
    }

    // dump hex data if debug is true
    if (CONFIG_LOGME_MBUS_DEBUG) {
        mbus_frame_print(&reply);
    }

    // generate JSON and print to standard output
    if (mbus_frame_data_parse(&reply, &reply_data) == -1) {
        fprintf(stderr, "M-bus data parse error: %s\n", mbus_error_str());
        mbus_disconnect(handle);
        mbus_context_free(handle);
        mbus_frame_free(reply.next);
        return 1;
    }
    
    if (*json_result)
        free(*json_result);
    if ((*json_result = mbus_frame_data_json(&reply_data)) == NULL) {
        fprintf(stderr, "Failed to generate JSON representation of MBUS frames: %s\n", mbus_error_str());
        mbus_disconnect(handle);
        mbus_context_free(handle);
        mbus_frame_free(reply.next);
        return 1;
    }

    mbus_frame_data_mqtt(&reply_data);

    // manual free
    if (reply_data.data_var.record) {
        mbus_data_record_free(reply_data.data_var.record); // free's up the whole list
    }


    // Clean up
    mbus_disconnect(handle);
    mbus_context_free(handle);

    return 0;
}