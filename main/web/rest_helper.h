//
// Defines helper functions for http/rest server
//

#ifndef LOGME_INTERFACE_REST_HELPER_H
#define LOGME_INTERFACE_REST_HELPER_H

static const char *REST_HELPER = "REST_HELPER";


static esp_err_t rest_helper_get_json_from_request(httpd_req_t *req, cJSON **json_object) {
    size_t total_len = req->content_len;
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
    *json_object = cJSON_Parse(buf);

    if (json_object == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            ESP_LOGE(REST_HELPER, "Error before: %s\n", error_ptr);
        }
        cJSON_Delete(*json_object);
        httpd_resp_sendstr(req, "Unable to parse message!");
        return ESP_FAIL;
    }
    return ESP_OK;

}

#endif //LOGME_INTERFACE_REST_HELPER_H
