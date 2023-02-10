#include "web/rest_server_helper.h"

#define REST_CHECK(a, str, goto_tag, ...)                                                     \
    do                                                                                        \
    {                                                                                         \
        if (!(a))                                                                             \
        {                                                                                     \
            ESP_LOGE(HTTP_SERVER_TAG, "%s(%d): " str, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
            goto goto_tag;                                                                    \
        }                                                                                     \
    } while (0)

#define CHECK_FILE_EXTENSION(filename, ext) (strcasecmp(&filename[strlen(filename) - strlen(ext)], ext) == 0)
#define FILE_PATH_MAX (ESP_VFS_PATH_MAX + 128)

/* Set HTTP response content type according to file extension */
esp_err_t set_content_type_from_file(httpd_req_t *req, const char *filepath) {
    const char *type = "text/plain";
    if (CHECK_FILE_EXTENSION(filepath, ".html")) {
        type = "text/html";
    } else if (CHECK_FILE_EXTENSION(filepath, ".js")) {
        type = "application/javascript";
    } else if (CHECK_FILE_EXTENSION(filepath, ".css")) {
        type = "text/css";
    } else if (CHECK_FILE_EXTENSION(filepath, ".png")) {
        type = "image/png";
    } else if (CHECK_FILE_EXTENSION(filepath, ".ico")) {
        type = "image/x-icon";
    } else if (CHECK_FILE_EXTENSION(filepath, ".svg")) {
        type = "text/xml";
    } else if (CHECK_FILE_EXTENSION(filepath, ".json")) {
        type = "text/json";
    }
    return httpd_resp_set_type(req, type);
}