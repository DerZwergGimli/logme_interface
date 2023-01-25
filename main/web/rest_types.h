#ifndef REST_TYPES_H
#define REST_TYPES_H

#include "esp_netif.h"
#include "esp_vfs.h"


#define SCRATCH_BUFSIZE (10240)
#define FILE_PATH_MAX (ESP_VFS_PATH_MAX + 128)

typedef struct rest_server_context {
    char base_path[ESP_VFS_PATH_MAX + 1];
    char scratch[SCRATCH_BUFSIZE];
    esp_netif_t *netif_config;
} rest_server_context_t;

#endif