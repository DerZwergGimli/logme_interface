#ifndef REST_POST_HANDLER_H
#define REST_POST_HANDLER_H

#include "esp_flash.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "cJSON.h"
#include "wifi/wifi_manager.h"
#include "rest_server.h"
#include "status_reponse.h"
#include "rest_helper.h"

static const char *RESTSERVER_PUT = "RESTSERVER_PUT";

/**
 * POST-Request
 * -> '/restart'
 */
static esp_err_t rest_post_restart_handler(httpd_req_t *req) {
    ESP_LOGI(HTTP_SERVER_TAG, "POST %s", req->uri);

    httpd_resp_set_status(req, http_200_hdr);
    httpd_resp_set_hdr(req, http_cache_control_hdr, http_cache_control_no_cache);
    httpd_resp_set_hdr(req, http_pragma_hdr, http_pragma_no_cache);

    cJSON *response_json = cJSON_CreateObject();
    json_status_response_create(response_json, STATUS_OK, "Restarting in 3s...");
    httpd_resp_set_type(req, http_content_type_json);
    const char *json_buff = cJSON_Print(response_json);
    httpd_resp_sendstr(req, json_buff);
    free((void *) json_buff);
    cJSON_Delete(response_json);

    vTaskDelay(pdMS_TO_TICKS(3000));

    system_info_send_message(SI_KILL, NULL);
    sensor_manager_send_message(SM_KILL, NULL);
    wifi_manager_send_message(WM_ORDER_KILL, NULL);

    esp_restart();
    return ESP_OK;
}


/**
 * POST-Request
 * -> '/wifireset'
 */
static esp_err_t rest_post_wifi_reset_handler(httpd_req_t *req) {
    ESP_LOGI(HTTP_SERVER_TAG, "POST %s", req->uri);

    httpd_resp_set_status(req, http_200_hdr);
    httpd_resp_set_hdr(req, http_cache_control_hdr, http_cache_control_no_cache);
    httpd_resp_set_hdr(req, http_pragma_hdr, http_pragma_no_cache);

    cJSON *response_json = cJSON_CreateObject();
    json_status_response_create(response_json, STATUS_OK, "WiFi set to: AP-Mode!");
    httpd_resp_set_type(req, http_content_type_json);
    const char *json_buff = cJSON_Print(response_json);
    httpd_resp_sendstr(req, json_buff);
    free((void *) json_buff);
    cJSON_Delete(response_json);


    wifi_manager_send_message(WM_ORDER_START_AP, NULL);

    return ESP_OK;
}

/**
 * POST-Request
 * -> '/sensorhistoryclear'
 */
static esp_err_t rest_post_sensor_history_clear_handler(httpd_req_t *req) {
    ESP_LOGI(HTTP_SERVER_TAG, "POST %s", req->uri);

    httpd_resp_set_status(req, http_200_hdr);
    httpd_resp_set_hdr(req, http_cache_control_hdr, http_cache_control_no_cache);
    httpd_resp_set_hdr(req, http_pragma_hdr, http_pragma_no_cache);

    cJSON *response_json = cJSON_CreateObject();
    json_status_response_create(response_json, STATUS_OK, "Clearing sensor history");
    httpd_resp_set_type(req, http_content_type_json);
    const char *json_buff = cJSON_Print(response_json);
    httpd_resp_sendstr(req, json_buff);
    free((void *) json_buff);
    cJSON_Delete(response_json);

    sensor_manager_send_message(SM_CLEAR_HISTORY, NULL);

    return ESP_OK;
}

/**
 * POST-Request
 * -> '/sensorsave'
 */
static esp_err_t rest_post_sensor_save_handler(httpd_req_t *req) {
    ESP_LOGI(HTTP_SERVER_TAG, "POST %s", req->uri);

    httpd_resp_set_status(req, http_200_hdr);
    httpd_resp_set_hdr(req, http_cache_control_hdr, http_cache_control_no_cache);
    httpd_resp_set_hdr(req, http_pragma_hdr, http_pragma_no_cache);

    cJSON *response_json = cJSON_CreateObject();
    json_status_response_create(response_json, STATUS_OK, "Saved sensor history");
    httpd_resp_set_type(req, http_content_type_json);
    const char *json_buff = cJSON_Print(response_json);
    httpd_resp_sendstr(req, json_buff);
    free((void *) json_buff);
    cJSON_Delete(response_json);

    sensor_manager_send_message(SM_SAVE_CONFIG, NULL);

    return ESP_OK;
}


/**
 * POST-Request
 * -> '/sesnoredit/$index'
 */
static esp_err_t rest_post_sensor_edit_handler(httpd_req_t *req, int index) {
    ESP_LOGI(HTTP_SERVER_TAG, "POST %s", req->uri);

    httpd_resp_set_status(req, http_200_hdr);
    httpd_resp_set_hdr(req, http_cache_control_hdr, http_cache_control_no_cache);
    httpd_resp_set_hdr(req, http_pragma_hdr, http_pragma_no_cache);
    httpd_resp_set_type(req, http_content_type_json);

    cJSON *response_json = cJSON_CreateObject();

    if (index >= CONFIG_LOGME_MBUS_DEVICES || index < 0) {
        json_status_response_create(response_json, STATUS_ERROR, "Sensor editing failed wrong index!");

    } else {
        cJSON *root = cJSON_CreateObject();
        ESP_ERROR_CHECK(rest_helper_get_json_from_request(req, &root));
        if (root != NULL) {
            char *sensor_name = "none";
            if (cJSON_IsString(cJSON_GetObjectItemCaseSensitive(root, "name"))) {
                sensor_name = cJSON_GetObjectItemCaseSensitive(root, "name")->valuestring;
            }

            char *sensor_description = "none";
            if (cJSON_IsString(cJSON_GetObjectItemCaseSensitive(root, "description"))) {
                sensor_description = cJSON_GetObjectItemCaseSensitive(root, "description")->valuestring;
            }

            int sensor_id = 0;
            if (cJSON_IsString(cJSON_GetObjectItemCaseSensitive(root, "id"))) {
                sensor_id = cJSON_GetObjectItemCaseSensitive(root, "id")->valueint;
            }

            int sensor_pin_rx = 0;
            if (cJSON_IsString(cJSON_GetObjectItemCaseSensitive(root, "pin_rx"))) {
                sensor_pin_rx = cJSON_GetObjectItemCaseSensitive(root, "pin_rx")->valueint;
            }

            int sensor_pin_tx = 0;
            if (cJSON_IsString(cJSON_GetObjectItemCaseSensitive(root, "pin_tx"))) {
                sensor_pin_rx = cJSON_GetObjectItemCaseSensitive(root, "pin_tx")->valueint;
            }

            int sensor_baudrate = 0;
            if (cJSON_IsString(cJSON_GetObjectItemCaseSensitive(root, "baudrate"))) {
                sensor_baudrate = cJSON_GetObjectItemCaseSensitive(root, "baudrate")->valueint;
            }

            int sensor_primary_address = 0;
            if (cJSON_IsString(cJSON_GetObjectItemCaseSensitive(root, "primary_address"))) {
                sensor_primary_address = cJSON_GetObjectItemCaseSensitive(root, "primary_address")->valueint;
            }

            int sensor_secondary_address = 0;
            if (cJSON_IsString(cJSON_GetObjectItemCaseSensitive(root, "secondary_address"))) {
                sensor_secondary_address = cJSON_GetObjectItemCaseSensitive(root, "secondary_address")->valueint;
            }

            ESP_ERROR_CHECK(sensor_manager_edit_params_save(index,
                                                            sensor_name,
                                                            sensor_id,
                                                            sensor_description,
                                                            sensor_pin_rx,
                                                            sensor_pin_tx,
                                                            sensor_baudrate,
                                                            sensor_primary_address,
                                                            sensor_secondary_address
            ));

            json_status_response_create(response_json, STATUS_OK, "Sensor edited!");
        } else { json_status_response_create(response_json, STATUS_ERROR, "Error parsing sensor data!"); }

        cJSON_Delete(root);
    }

    const char *json_buff = cJSON_Print(response_json);
    httpd_resp_sendstr(req, json_buff);
    free((void *) json_buff);
    cJSON_Delete(response_json);


    return ESP_OK;
}


/**
 * POST-Request
 * -> '/wifiConfig'
 */
static esp_err_t rest_post_wifiConfig_handler(httpd_req_t *req) {
    ESP_LOGI(HTTP_SERVER_TAG, "POST %s", req->uri);

    int total_len = req->content_len;
    int cur_len = 0;
    char *buf = ((rest_server_context_t *) (req->user_ctx))->scratch;
    int received = 0;
    if (total_len >= SCRATCH_BUFFER_SIZE) {
        /* Respond with 500 Internal Server Error */
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "content too long");
        return ESP_FAIL;
    }
    while (cur_len < total_len) {
        received = httpd_req_recv(req, buf + cur_len, total_len);
        if (received <= 0) {
            /* Respond with 500 Internal Server Error */
            httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to post control value");
            return ESP_FAIL;
        }
        cur_len += received;
    }
    buf[total_len] = '\0';

    cJSON *root = cJSON_Parse(buf);

    if (root == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            ESP_LOGE(RESTSERVER_PUT, "Error before: %s\n", error_ptr);
        }
        cJSON_Delete(root);
        httpd_resp_sendstr(req, "Unable to parse message!");
        return ESP_OK;
    }

    const cJSON *network_ssid_json = cJSON_GetObjectItemCaseSensitive(root, "ssid");
    const cJSON *network_password_json = cJSON_GetObjectItemCaseSensitive(root, "password");


    char network_ssid[30] = "none";
    char network_password[30] = "none";
    if (network_ssid_json) {
        snprintf(network_ssid, "%s", network_ssid_json->valuestring);
    }

    if (network_password_json) {
        snprintf(network_password, "%s", network_password_json->valuestring);
    }

    ESP_LOGI(RESTSERVER_PUT, "ssid: %s", network_ssid);
    ESP_LOGI(RESTSERVER_PUT, "passwd: %s", network_password);

    if (network_ssid_json && network_password_json) {
        ESP_LOGI(RESTSERVER_PUT, "Starting wifi new config...");

        wifi_config_t *config = wifi_manager_get_wifi_sta_config();
        memset(config, 0x00, sizeof(wifi_config_t));
        memcpy(config->sta.ssid, network_ssid, 30);
        memcpy(config->sta.password, network_password, 30);
        ESP_LOGD(RESTSERVER_PUT, "http_server_post_handler: wifi_manager_connect_async() call");
        wifi_manager_connect_async();

        /* free memory */
        //free(network_ssid);
        //free(network_password);

        httpd_resp_sendstr(req, "Post wifiConfig initialized!");

    } else {
        httpd_resp_sendstr(req, "Post wifiConfig unsuccessful!");

    }

    cJSON_Delete(root);

    return ESP_OK;
}

#endif