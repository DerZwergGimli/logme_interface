#ifndef REST_POST_HANDLER_H
#define REST_POST_HANDLER_H

#include "esp_flash.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "cJSON.h"
#include "wifi/wifi_manager.h"
#include "rest_server.h"

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
    httpd_resp_set_type(req, http_content_type_plain);
    httpd_resp_sendstr(req, "Restart triggerd...(3s)!");
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
    httpd_resp_set_type(req, http_content_type_plain);
    httpd_resp_sendstr(req, "WiFi set to: AP-Mode!");
    vTaskDelay(pdMS_TO_TICKS(3000));

    wifi_manager_send_message(WM_ORDER_START_AP, NULL);

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
    if (total_len >= SCRATCH_BUFSIZE) {
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