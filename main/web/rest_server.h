#ifndef REST_SERVER_H
#define REST_SERVER_H

#include <string.h>
#include <fcntl.h>
#include "esp_http_server.h"
#include "esp_chip_info.h"
#include "esp_random.h"
#include "esp_log.h"
#include "esp_vfs.h"
#include "cJSON.h"

#include "web/rest_server_helper.h"
#include "web/rest_get_handler.h"
#include "web/rest_post_handler.h"
#include "web/rest_types.h"

static const char *REST_TAG = "RESTSERVER_GET";

esp_err_t start_rest_server(const char *base_path, bool lru_purge_enable);
esp_err_t stop_rest_server();
#endif