#ifndef REST_SERVER_HELPER_H
#define REST_SERVER_HELPER_H

#include <esp_http_server.h>
#include "web/rest_types.h"
#include "esp_http_server.h"
#include "wifi/wifi_manager.h"

static const char *HTTP_SERVER_TAG = "REST-Server";
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
esp_err_t set_content_type_from_file(httpd_req_t *req, const char *filepath);

/* const httpd related values stored in ROM */
const static char http_200_hdr[] = "200 OK";
const static char http_302_hdr[] = "302 Found";
const static char http_400_hdr[] = "400 Bad Request";
const static char http_404_hdr[] = "404 Not Found";
const static char http_503_hdr[] = "503 Service Unavailable";
const static char http_location_hdr[] = "Location";
const static char http_content_type_plain[] = "text/plain";
const static char http_content_type_html[] = "text/html";
const static char http_content_type_js[] = "text/javascript";
const static char http_content_type_css[] = "text/css";
const static char http_content_type_json[] = "application/json";
const static char http_cache_control_hdr[] = "Cache-Control";
const static char http_cache_control_no_cache[] = "no-store, no-cache, must-revalidate, max-age=0";
const static char http_cache_control_cache[] = "public, max-age=31536000";
const static char http_pragma_hdr[] = "Pragma";
const static char http_pragma_no_cache[] = "no-cache";

/* Send HTTP response with the contents of the requested file */

#endif