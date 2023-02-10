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
#include "fs/config.h"

static const char *RESTSERVER_PUT = "RESTSERVER_PUT";

void rest_send_json_response_ok(httpd_req_t *req, char *message);


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
 * -> '/mqtt'
 */
esp_err_t rest_post_mqtt_config_handler(httpd_req_t *req);

/**
 * POST-Request
 * -> '/cronjobs'
 */
esp_err_t rest_post_cron_jobs_config_handler(httpd_req_t *req);

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