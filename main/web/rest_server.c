/* HTTP Restful API Server

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include "web/rest_server.h"
#include "web/rest_server_helper.h"
#include "web/rest_types.h"

static httpd_handle_t server_handle = NULL;

/* strings holding the URLs of the wifi manager */
static char *http_root_url = NULL;
static char *http_redirect_url = NULL;

httpd_handle_t *get_rest_server() {
    return &server_handle;
};

static esp_err_t base_get_handler(httpd_req_t *req) {
    char *host = NULL;
    size_t buf_len;
    esp_err_t ret = ESP_OK;

    ESP_LOGI(HTTP_SERVER_TAG, "GET %s", req->uri);
    ESP_LOGI(HTTP_SERVER_TAG, "ROOTURL %s", http_root_url);
    ESP_LOGI(HTTP_SERVER_TAG, "REDIRECT %s", http_redirect_url);

    /* determine if Host is from the STA IP address */
    wifi_manager_lock_sta_ip_string(portMAX_DELAY);
    bool access_from_sta_ip = host != NULL ? strstr(host, wifi_manager_get_sta_ip_string()) : false;
    wifi_manager_unlock_sta_ip_string();

    size_t req_hdr_host_len = httpd_req_get_hdr_value_len(req, "Host");
    char req_hdr_host_val[req_hdr_host_len + 1];

    esp_err_t res = httpd_req_get_hdr_value_str(req, "Host", (char *) &req_hdr_host_val,
                                                sizeof(char) * req_hdr_host_len + 1);
    if (res != ESP_OK) {
        ESP_LOGE(HTTP_SERVER_TAG, "failed getting HOST header value: %d", res);

        switch (res) {
            case ESP_ERR_NOT_FOUND:
                ESP_LOGE(HTTP_SERVER_TAG, "failed getting HOST header value: ESP_ERR_NOT_FOUND: Key not found: %d",
                         res);
                break;

            case ESP_ERR_INVALID_ARG:
                ESP_LOGE(HTTP_SERVER_TAG, "failed getting HOST header value: ESP_ERR_INVALID_ARG: Null arguments: %d",
                         res);
                break;

            case ESP_ERR_HTTPD_INVALID_REQ:
                ESP_LOGE(HTTP_SERVER_TAG,
                         "failed getting HOST header value: ESP_ERR_HTTPD_INVALID_REQ: Invalid HTTP request pointer: %d",
                         res);
                break;

            case ESP_ERR_HTTPD_RESULT_TRUNC:
                ESP_LOGE(HTTP_SERVER_TAG,
                         "failed getting HOST header value: ESP_ERR_HTTPD_RESULT_TRUNC: Value string truncated: %d",
                         res);
                break;

            default:
                break;
        }
    }

    ESP_LOGI(HTTP_SERVER_TAG, "Got HOST header value: %s", req_hdr_host_val);

    const char redir_trigger_host[] = "connectivitycheck.gstatic.com";

    if (strncmp(req_hdr_host_val, redir_trigger_host, strlen(redir_trigger_host)) == 0) {
        const char resp[] = "302 Found";

        ESP_LOGI(HTTP_SERVER_TAG, "Detected redirect trigger HOST: %s", redir_trigger_host);

        httpd_resp_set_status(req, resp);

        /** NOTE: This is where you redirect to whatever DNS address you prefer to open the
            captive portal page. This DNS address will be displayed at the top of the
            page, so maybe you want to choose a nice name to use (it can be any legal
            DNS name that you prefer. */
        httpd_resp_set_hdr(req, http_location_hdr, http_redirect_url);

        httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
        return ESP_OK;
    } else {

        if (host != NULL && !strstr(host, DEFAULT_AP_IP) && !access_from_sta_ip) {

            /* Captive Portal functionality */
            /* 302 Redirect to IP of the access point */
            httpd_resp_set_status(req, http_302_hdr);
            httpd_resp_set_hdr(req, http_location_hdr, http_redirect_url);
            httpd_resp_send(req, NULL, 0);
            return ESP_OK;


        } else if (strcmp(req->uri, "/system") == 0) {
            ESP_ERROR_CHECK(rest_get_system_handler(req));
            return ESP_OK;

        } else if (strcmp(req->uri, "/wifi") == 0) {
            ESP_ERROR_CHECK(rest_get_wifi_handler(req));
            return ESP_OK;

        } else if (strcmp(req->uri, "/ap") == 0) {
            ESP_ERROR_CHECK(rest_get_ap_handler(req));
            return ESP_OK;

        } else if (strcmp(req->uri, "/sensors") == 0) {
            ESP_ERROR_CHECK(rest_get_sensors_handler(req));
            return ESP_OK;

        } else {

            char filepath[FILE_PATH_MAX];

            rest_server_context_t *rest_context = (rest_server_context_t *) req->user_ctx;
            strlcpy(filepath, rest_context->base_path, sizeof(filepath));
            if (strcmp(req->uri, http_root_url) == 0) {
                strlcat(filepath, "/index.html", sizeof(filepath));
            } else {
                strlcat(filepath, req->uri, sizeof(filepath));
            }
            int fd = open(filepath, O_RDONLY, 0);
            if (fd == -1) {
                ESP_LOGE(HTTP_SERVER_TAG, "Failed to open file : %s", filepath);
                /* Respond with 500 Internal Server Error */
                httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to read existing file");
                return ESP_FAIL;
            }

            set_content_type_from_file(req, filepath);

            char *chunk = rest_context->scratch;
            ssize_t read_bytes;
            do {
                /* Read file in chunks into the scratch buffer */
                read_bytes = read(fd, chunk, SCRATCH_BUFSIZE);
                if (read_bytes == -1) {
                    ESP_LOGE(HTTP_SERVER_TAG, "Failed to read file : %s", filepath);
                } else if (read_bytes > 0) {
                    /* Send the buffer contents as HTTP response chunk */
                    if (httpd_resp_send_chunk(req, chunk, read_bytes) != ESP_OK) {
                        close(fd);
                        ESP_LOGE(HTTP_SERVER_TAG, "File sending failed!");
                        /* Abort sending file */
                        httpd_resp_sendstr_chunk(req, NULL);
                        /* Respond with 500 Internal Server Error */
                        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to send file");
                        return ESP_FAIL;
                    }
                }
            } while (read_bytes > 0);
            /* Close file after sending complete */
            close(fd);
            ESP_LOGI(HTTP_SERVER_TAG, "File sending complete");
            /* Respond with an empty chunk to signal HTTP response completion */
            httpd_resp_send_chunk(req, NULL, 0);
            return ESP_OK;
        }
    }
}

static esp_err_t base_post_handler(httpd_req_t *req) {
    if (strcmp(req->uri, "/restart") == 0) {
        ESP_ERROR_CHECK(rest_post_restart_handler(req));
        return ESP_OK;
    } else if (strcmp(req->uri, "/wifireset") == 0) {
        ESP_ERROR_CHECK(rest_post_wifi_reset_handler(req));
        return ESP_OK;
    } else if (strcmp(req->uri, "/wificonfig") == 0) {
        ESP_ERROR_CHECK(rest_post_wifiConfig_handler(req));
        return ESP_OK;
    } else if (strcmp(req->uri, "/sensorhistoryclear") == 0) {
        ESP_ERROR_CHECK(rest_post_sensor_history_clear_handler(req));
        return ESP_OK;
    } else if (strcmp(req->uri, "/sensorsave") == 0) {
        ESP_ERROR_CHECK(rest_post_sensor_save_handler(req));
        return ESP_OK;
    } else if (strcmp(req->uri, "/sensoredit") > 0) {
        char *pEnd;
        const char *number_string = req->uri + sizeof("/sensoredit/") - 1;
        int number = strtol(number_string, &pEnd, 10);
        ESP_ERROR_CHECK(rest_post_sensor_edit_handler(req, number));
        return ESP_OK;

    } else {
        httpd_resp_set_status(req, http_400_hdr);
        httpd_resp_set_hdr(req, http_cache_control_hdr, http_cache_control_no_cache);
        httpd_resp_set_hdr(req, http_pragma_hdr, http_pragma_no_cache);

        cJSON *response_json = cJSON_CreateObject();
        json_status_response_create(response_json, STATUS_ERROR, "No route found!");
        httpd_resp_set_type(req, http_content_type_json);
        const char *json_buff = cJSON_Print(response_json);
        httpd_resp_sendstr(req, json_buff);
        free((void *) json_buff);
        cJSON_Delete(response_json);

        return 1;
    }

}

static esp_err_t rest_common_get_handler(httpd_req_t *req) {
    char *host = NULL;
    size_t buf_len;
    esp_err_t ret = ESP_OK;

    ESP_LOGI(HTTP_SERVER_TAG, "GET %s", req->uri);

    /* determine if Host is from the STA IP address */
    wifi_manager_lock_sta_ip_string(portMAX_DELAY);
    bool access_from_sta_ip = host != NULL ? strstr(host, wifi_manager_get_sta_ip_string()) : false;
    wifi_manager_unlock_sta_ip_string();

    if (host != NULL && !strstr(host, DEFAULT_AP_IP) && !access_from_sta_ip) {

        /* Captive Portal functionality */
        /* 302 Redirect to IP of the access point */
        httpd_resp_set_status(req, http_302_hdr);
        httpd_resp_set_hdr(req, http_location_hdr, http_redirect_url);
        httpd_resp_send(req, NULL, 0);
        return ESP_OK;
    } else {

        char filepath[FILE_PATH_MAX];

        rest_server_context_t *rest_context = (rest_server_context_t *) req->user_ctx;
        strlcpy(filepath, rest_context->base_path, sizeof(filepath));
        if (strcmp(req->uri, http_root_url) == 0) {
            strlcat(filepath, "/index.html", sizeof(filepath));
        } else {
            strlcat(filepath, req->uri, sizeof(filepath));
        }
        int fd = open(filepath, O_RDONLY, 0);
        if (fd == -1) {
            ESP_LOGE(HTTP_SERVER_TAG, "Failed to open file : %s", filepath);
            /* Respond with 500 Internal Server Error */
            httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to read existing file");
            return ESP_FAIL;
        }

        set_content_type_from_file(req, filepath);

        char *chunk = rest_context->scratch;
        ssize_t read_bytes;
        do {
            /* Read file in chunks into the scratch buffer */
            read_bytes = read(fd, chunk, SCRATCH_BUFSIZE);
            if (read_bytes == -1) {
                ESP_LOGE(HTTP_SERVER_TAG, "Failed to read file : %s", filepath);
            } else if (read_bytes > 0) {
                /* Send the buffer contents as HTTP response chunk */
                if (httpd_resp_send_chunk(req, chunk, read_bytes) != ESP_OK) {
                    close(fd);
                    ESP_LOGE(HTTP_SERVER_TAG, "File sending failed!");
                    /* Abort sending file */
                    httpd_resp_sendstr_chunk(req, NULL);
                    /* Respond with 500 Internal Server Error */
                    httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to send file");
                    return ESP_FAIL;
                }
            }
        } while (read_bytes > 0);
        /* Close file after sending complete */
        close(fd);
        ESP_LOGI(HTTP_SERVER_TAG, "File sending complete");
        /* Respond with an empty chunk to signal HTTP response completion */
        httpd_resp_send_chunk(req, NULL, 0);
        return ESP_OK;
    }
}


esp_err_t start_rest_server(const char *base_path, bool lru_purge_enable) {

    if (server_handle == NULL) {
        ESP_LOGI(REST_TAG, "Starting HTTP Server");

        // esp_netif_t *netif_config;
        // smart_meter_sensor_t *sensors;
        //  Setup ase config for rest requests
        REST_CHECK(base_path, "wrong base path", err);
        rest_server_context_t *rest_context = calloc(1, sizeof(rest_server_context_t));
        REST_CHECK(rest_context, "No memory for rest context", err);
        strlcpy(rest_context->base_path, base_path, sizeof(rest_context->base_path));
        // rest_context->netif_config = netif_config;
        // rest_context->sensors = sensors;

        /* generate the URLs */
        if (http_root_url == NULL) {
            int root_len = strlen(CONFIG_WEBAPP_LOCATION);

            /* root url, eg "/"   */
            const size_t http_root_url_sz = sizeof(char) * (root_len + 1);
            http_root_url = malloc(http_root_url_sz);
            memset(http_root_url, 0x00, http_root_url_sz);
            strcpy(http_root_url, CONFIG_WEBAPP_LOCATION);

            /* redirect url */
            size_t redirect_sz = 22 + root_len + 1; /* strlen(http://255.255.255.255) + strlen("/") + 1 for \0 */
            http_redirect_url = malloc(sizeof(char) * redirect_sz);
            *http_redirect_url = '\0';

            if (root_len == 1) {
                snprintf(http_redirect_url, redirect_sz, "http://%s", DEFAULT_AP_IP);
            } else {
                snprintf(http_redirect_url, redirect_sz, "http://%s%s", DEFAULT_AP_IP, CONFIG_WEBAPP_LOCATION);
            }
        }

        httpd_config_t config = HTTPD_DEFAULT_CONFIG();
        //config.stack_size = config.stack_size * 4;
        config.uri_match_fn = httpd_uri_match_wildcard;
        config.lru_purge_enable = lru_purge_enable;

        REST_CHECK(httpd_start(&server_handle, &config) == ESP_OK, "Start server failed", err_start);

        ESP_LOGI(REST_TAG, "Started HTTP Server");

        // Add Endpoints to REST-Server
        /* URI handler for fetching SYSTEM-info */

        /* URI handler for Restart  */

        // httpd_uri_t wss_sensor_uri = {
        //     .uri = "/websocket",
        //     .method = HTTP_GET,
        //     .handler = echo_handler,
        //     .user_ctx = NULL,
        //     .is_websocket = true};
        // httpd_register_uri_handler(server, &wss_sensor_uri);

        // httpd_uri_t wss_subscribe_uri = {
        //     .uri = "/websocket",
        //     .method = HTTP_SUBSCRIBE,
        //     .handler = wss_subscribe_handler,
        //     .user_ctx = NULL,
        //     .is_websocket = true};
        // httpd_register_uri_handler(server, &wss_subscribe_uri);

        httpd_uri_t base_get_uri = {
                .uri = "*",
                .method = HTTP_GET,
                .handler = base_get_handler,
                .user_ctx = rest_context};
        httpd_register_uri_handler(server_handle, &base_get_uri);


        httpd_uri_t base_post_uri = {
                .uri = "*",
                .method = HTTP_POST,
                .handler = base_post_handler,
                .user_ctx = rest_context};
        httpd_register_uri_handler(server_handle, &base_post_uri);
        /* URI handler for getting web server files */
        // httpd_uri_t common_get_uri = {
        //     .uri = "/*",
        //     .method = HTTP_GET,
        //     .handler = rest_common_get_handler,
        //     .user_ctx = rest_context};
        // httpd_register_uri_handler(server_handle, &common_get_uri);

        // wss_server_send_messages(&server, sensors);

        return ESP_OK;
        err_start:
        free(rest_context);
        err:
        return ESP_FAIL;
    }
    return ESP_OK;
}

esp_err_t stop_rest_server() {
    if (server_handle != NULL) {

        /* dealloc URLs */
        if (http_root_url) {
            free(http_root_url);
            http_root_url = NULL;
        }
        if (http_redirect_url) {
            free(http_redirect_url);
            http_redirect_url = NULL;
        }

        httpd_stop(server_handle);
        server_handle = NULL;

        ESP_LOGI(HTTP_SERVER_TAG, "Stopped reset server!");
    }

    return ESP_OK;
}