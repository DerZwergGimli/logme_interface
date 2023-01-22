#ifndef REST_POST_HANDLER_H
#define REST_POST_HANDLER_H

#include "esp_flash.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "rest_types.h"
#include "cJSON.h"

static const char *RESTSERVER_PUT = "RESTSERVER_PUT";

/**
 * POST-Request
 * -> '/wifiConfig'
 */
static esp_err_t rest_post_wifiConfig_handler(httpd_req_t *req)
{
    int total_len = req->content_len;
    int cur_len = 0;
    char *buf = ((rest_server_context_t *)(req->user_ctx))->scratch;
    int received = 0;
    if (total_len >= SCRATCH_BUFSIZE)
    {
        /* Respond with 500 Internal Server Error */
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "content too long");
        return ESP_FAIL;
    }
    while (cur_len < total_len)
    {
        received = httpd_req_recv(req, buf + cur_len, total_len);
        if (received <= 0)
        {
            /* Respond with 500 Internal Server Error */
            httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to post control value");
            return ESP_FAIL;
        }
        cur_len += received;
    }
    buf[total_len] = '\0';

    cJSON *root = cJSON_Parse(buf);

    if (root == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
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
    if (network_ssid_json)
    {
        snprintf(network_ssid, "%s", network_ssid_json->valuestring);
    }

    if (network_password_json)
    {
        snprintf(network_password, "%s", network_password_json->valuestring);
    }

    ESP_LOGI(RESTSERVER_PUT, "ssid: %s", network_ssid);
    ESP_LOGI(RESTSERVER_PUT, "passwd: %s", network_password);

    cJSON_Delete(root);
    httpd_resp_sendstr(req, "Post wifiConfig successfully");

    return ESP_OK;
}

#endif