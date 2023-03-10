#include "web/handler_get/rest_get_handler.h"

/**
 * GET-Request
 * -> '/system'
 */
esp_err_t rest_get_system_handler(httpd_req_t *req) {
    system_info_send_message(SI_UPDATE, NULL);

    if (system_info_lock_info_json_buffer(pdMS_TO_TICKS(1000))) {
        const char *json_buff = system_info_get_info_json();
        if (json_buff) {
            httpd_resp_set_status(req, http_200_hdr);
            httpd_resp_set_hdr(req, http_cache_control_hdr, http_cache_control_no_cache);
            httpd_resp_set_hdr(req, http_pragma_hdr, http_pragma_no_cache);
            httpd_resp_set_type(req, http_content_type_json);
            httpd_resp_sendstr(req, json_buff);

        } else {
            httpd_resp_set_status(req, http_503_hdr);
            httpd_resp_send(req, NULL, 0);
        }
        system_info_unlock_info_json_buffer();

    } else {
        httpd_resp_set_status(req, http_503_hdr);
        httpd_resp_send(req, NULL, 0);
        ESP_LOGE("REST_GET-System", "failed to obtain mutex");
    }
    return ESP_OK;
}


/**
 * GET-Request
 * -> '/wifi'
 */
esp_err_t rest_get_wifi_handler(httpd_req_t *req) {
    if (wifi_manager_lock_json_buffer((TickType_t) 10)) {
        char *buff = wifi_manager_get_ip_info_json();
        if (buff) {
            httpd_resp_set_status(req, http_200_hdr);
            httpd_resp_set_type(req, http_content_type_json);
            httpd_resp_set_hdr(req, http_cache_control_hdr, http_cache_control_no_cache);
            httpd_resp_set_hdr(req, http_pragma_hdr, http_pragma_no_cache);
            httpd_resp_sendstr(req, buff);
            wifi_manager_unlock_json_buffer();
        } else {
            httpd_resp_set_status(req, http_503_hdr);
            httpd_resp_send(req, NULL, 0);
        }
    } else {
        httpd_resp_set_status(req, http_503_hdr);
        httpd_resp_send(req, NULL, 0);
        ESP_LOGE("REST_GET-Wifi", "failed to obtain mutex");
    }
    return ESP_OK;
}

/**
 * GET-Request
 * -> '/ap'
 */
esp_err_t rest_get_ap_handler(httpd_req_t *req) {

    if (wifi_manager_lock_json_buffer((TickType_t) 10)) {
        httpd_resp_set_status(req, http_200_hdr);
        httpd_resp_set_type(req, http_content_type_json);
        httpd_resp_set_hdr(req, http_cache_control_hdr, http_cache_control_no_cache);
        httpd_resp_set_hdr(req, http_pragma_hdr, http_pragma_no_cache);
        char *ap_buf = wifi_manager_get_ap_list_json();
        httpd_resp_send(req, ap_buf, strlen(ap_buf));
        wifi_manager_unlock_json_buffer();
    } else {
        httpd_resp_set_status(req, http_503_hdr);
        httpd_resp_send(req, NULL, 0);
        ESP_LOGE("TAG", "http_server_netconn_serve: GET /ap.json failed to obtain mutex");
    }

    /* request a wifi scan */
    wifi_manager_scan_async();
    return ESP_OK;
}

/**
 * GET-Request
 * -> '/sensors'
 */
esp_err_t rest_get_sensors_handler(httpd_req_t *req) {

    if (sensor_manager_lock_json_buffer(portMAX_DELAY)) {
        const char *json_buff = sensor_manager_get_json();
        if (json_buff) {
            httpd_resp_set_status(req, http_200_hdr);
            httpd_resp_set_hdr(req, http_cache_control_hdr, http_cache_control_no_cache);
            httpd_resp_set_hdr(req, http_pragma_hdr, http_pragma_no_cache);
            httpd_resp_set_type(req, http_content_type_json);
            httpd_resp_sendstr(req, json_buff);

        } else {
            httpd_resp_set_status(req, http_503_hdr);
            httpd_resp_send(req, NULL, 0);
        }
        sensor_manager_unlock_json_buffer();

    } else {
        httpd_resp_set_status(req, http_503_hdr);
        httpd_resp_send(req, NULL, 0);
        ESP_LOGE("REST_GET-System", "failed to obtain mutex");
    }
    return ESP_OK;
}

/**
 * GET-Request
 * -> '/sensors/id'
 */
esp_err_t rest_get_sensor_handler(httpd_req_t *req, int index) {


    if (sensor_manager_lock_json_buffer(portMAX_DELAY)) {
        const char *json_buff = sensor_manager_get_json();
        if (json_buff) {
            httpd_resp_set_status(req, http_200_hdr);
            httpd_resp_set_hdr(req, http_cache_control_hdr, http_cache_control_no_cache);
            httpd_resp_set_hdr(req, http_pragma_hdr, http_pragma_no_cache);
            httpd_resp_set_type(req, http_content_type_json);
            httpd_resp_sendstr(req, json_buff);

        } else {
            httpd_resp_set_status(req, http_503_hdr);
            httpd_resp_send(req, NULL, 0);
        }
        sensor_manager_unlock_json_buffer();

    } else {
        httpd_resp_set_status(req, http_503_hdr);
        httpd_resp_send(req, NULL, 0);
        ESP_LOGE("REST_GET-System", "failed to obtain mutex");
    }
    return ESP_OK;
}
