//
// Interface of OTA
//


//
// Implementation of OTA
//

#include <string.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_ota_ops.h"
#include "esp_app_format.h"
#include "esp_http_client.h"
#include "esp_flash_partitions.h"
#include "esp_partition.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "errno.h"

#if CONFIG_EXAMPLE_CONNECT_WIFI
#include "esp_wifi.h"
#endif

static void http_cleanup(esp_http_client_handle_t client);

void __attribute__((noreturn)) task_fatal_error(void);

void print_sha256(const uint8_t *image_hash, const char *label);

void infinite_loop(void);

void ota_example_task(void *pvParameter);

bool diagnostic(void);

void ota_manager_init(void);

void ota_manager_start(void);