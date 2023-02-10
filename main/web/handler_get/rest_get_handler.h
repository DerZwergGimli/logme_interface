#ifndef REST_GET_HANDLER_H
#define REST_GET_HANDLER_H

#include <esp_http_server.h>
#include "esp_flash.h"
#include "esp_wifi.h"
#include "cJSON.h"

#include "system/system_info.h"
#include "web/rest_server_helper.h"
#include "sensors/sensor_manager.h"

/**
 * GET-Request
 * -> '/system'
 */
esp_err_t rest_get_system_handler(httpd_req_t *req);

/**
 * GET-Request
 * -> '/wifi'
 */
esp_err_t rest_get_wifi_handler(httpd_req_t *req);

/**
 * GET-Request
 * -> '/ap'
 */
esp_err_t rest_get_ap_handler(httpd_req_t *req);

/**
 * GET-Request
 * -> '/sensors'
 */
esp_err_t rest_get_sensors_handler(httpd_req_t *req);

/**
 * GET-Request
 * -> '/sensors/id'
 */
esp_err_t rest_get_sensor_handler(httpd_req_t *req, int index);

#endif