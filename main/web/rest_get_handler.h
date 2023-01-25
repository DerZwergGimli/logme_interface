#ifndef REST_GET_HANDLER_H
#define REST_GET_HANDLER_H

#include "esp_flash.h"
#include "esp_wifi.h"
#include "rest_types.h"
#include "cJSON.h"

#include "system/system_info.h"

/**
 * GET-Request
 * -> '/system'
 */
static esp_err_t rest_get_system_handler(httpd_req_t *req) {

    if (system_info_lock_info_json_buffer((TickType_t) 10)) {
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
static esp_err_t rest_get_wifi_handler(httpd_req_t *req) {

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
static esp_err_t rest_get_ap_handler(httpd_req_t *req) {
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
static esp_err_t rest_get_sensors_handler(httpd_req_t *req) {
    rest_server_context_t *rest_context = (rest_server_context_t *) req->user_ctx;
    smart_meter_sensor_t *sensors = rest_context->sensors;

    httpd_resp_set_type(req, "application/json");

    cJSON *sensors_array = cJSON_CreateArray();

    for (size_t i = 0; i < CONFIG_SMART_METER_SENSOR_LENGTH; i++) {
        cJSON *sensor_item = cJSON_CreateObject();
        cJSON_AddNumberToObject(sensor_item, "id", sensors[i].id);
        cJSON_AddStringToObject(sensor_item, "name", sensors[i].name);
        cJSON_AddNumberToObject(sensor_item, "count", sensors[i].count);
        cJSON_AddNumberToObject(sensor_item, "power", sensors[i].power);

        cJSON *history = cJSON_CreateObject();
        cJSON *history_day = cJSON_CreateArray();
        cJSON *history_week = cJSON_CreateArray();
        cJSON *history_month = cJSON_CreateArray();

        cJSON_AddArrayToObject(history_day, sensors[i].history.day_24_kw);
        cJSON_AddArrayToObject(history_week, sensors[i].history.week_7_kw);
        cJSON_AddArrayToObject(history_month, sensors[i].history.month_30_kw);

        cJSON_AddItemToObject(history, "day_24_kw", history_day);
        cJSON_AddItemToObject(history, "week_7_kw", history_week);
        cJSON_AddItemToObject(history, "month_30_kw", history_month);

        cJSON_AddItemToObject(sensor_item, "history", history);

        cJSON_AddItemToArray(sensors_array, sensor_item);
    }

    const char *response = cJSON_Print(sensors_array);
    httpd_resp_sendstr(req, response);
    free((void *) response);
    cJSON_Delete(sensors_array);
    return ESP_OK;
}

#endif