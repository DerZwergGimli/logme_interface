#ifndef REST_POST_HANDLER_H
#define REST_POST_HANDLER_H

#include <esp_http_server.h>
#include "esp_flash.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "cJSON.h"
#include "wifi/wifi_manager.h"
#include "mqtt/mqtt_manager.h"
#include "web/rest_server_helper.h"
#include "web/status_reponse.h"
#include "system/system_info.h"
#include "sensors/sensor_manager.h"
#include "web/rest_helper.h"

static const char *RESTSERVER_PUT = "RESTSERVER_PUT";

/**
 * POST-Request
 * -> '/restart'
 */
esp_err_t rest_post_restart_handler(httpd_req_t *req);

/**
 * POST-Request
 * -> '/restart'
 */
esp_err_t rest_post_mqtt_ping_handler(httpd_req_t *req);

/**
 * POST-Request
 * -> '/wifireset'
 */
esp_err_t rest_post_wifi_reset_handler(httpd_req_t *req);

/**
 * POST-Request
 * -> '/sensorhistoryclear'
 */
esp_err_t rest_post_sensor_history_clear_handler(httpd_req_t *req);

/**
 * POST-Request
 * -> '/sensorsave'
 */
esp_err_t rest_post_sensor_save_handler(httpd_req_t *req);


/**
 * POST-Request
 * -> '/sesnoredit/$index'
 */
esp_err_t rest_post_sensor_edit_handler(httpd_req_t *req, int index);

/**
 * POST-Request
 * -> '/wifiConfig'
 */
esp_err_t rest_post_wifiConfig_handler(httpd_req_t *req);

#endif