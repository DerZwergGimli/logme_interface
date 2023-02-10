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


static const char *REST_TAG = "REST-SERVER";

#include "esp_netif.h"
#include "esp_vfs.h"

httpd_handle_t *get_rest_server();

esp_err_t start_rest_server(const char *base_path, bool lru_purge_enable);

esp_err_t stop_rest_server();


#endif