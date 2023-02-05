#include "sdkconfig.h"
#include "driver/gpio.h"
#include "esp_vfs_semihost.h"
#include "esp_vfs_fat.h"
#include "esp_spiffs.h"
#include "sdmmc_cmd.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_log.h"
#include "mdns.h"
#include "lwip/apps/netbiosns.h"

#include "mbus_serial_scan.h"


#if CONFIG_EXAMPLE_WEB_DEPLOY_SD
#include "driver/sdmmc_host.h"
#endif

#include "web/mdns.h"
#include "fs/fs.h"

#include <esp_http_server.h>
#include <esp_heap_trace.h>

#include "wifi/wifi_manager.h"
#include "wifi/nvs_sync.h"
#include "web/rest_types.h"
#include "web/rest_server_helper.h"
#include "system/system_info.h"
#include "sensors/sensor_manager.h"
#include "web/rest_server.h"
#include "timer/timer_manager.h"

#include "timer/timer_manager.h"
#include "sensors/sensor_helper.h"

// GLOBALS
static const char TAG_MAIN[] = "main";
// WiFi-Config
esp_netif_t *netif_config;
esp_netif_ip_info_t ip_info;

//
// esp_netif_t *wifi_init_sta(void);
// esp_err_t start_rest_server(const char *base_path, esp_netif_t *netif_config, smart_meter_sensor_t *sensors);


void cb_restart_rest_server(void *pvParameter) {
    ESP_LOGI(TAG_MAIN, "Restarting WebServer...");
    ESP_ERROR_CHECK(stop_rest_server());
    ESP_ERROR_CHECK(start_rest_server(CONFIG_EXAMPLE_WEB_MOUNT_POINT, true));
}


//void monitoring_task(void *pvParameter) {
//    for (;;) {
//        ESP_LOGI(TAG_MAIN, "free heap: %lu", esp_get_free_heap_size());
//        vTaskDelay(pdMS_TO_TICKS(10000));
//    }
//}
#define NUM_RECORDS 100
static heap_trace_record_t trace_record[NUM_RECORDS]; // This buffer must be in internal RAM




void app_main(void) {
    printf("VERSION= %s\n", CONFIG_LOGME_VERSION);

    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_ERROR_CHECK(init_fs());
    ESP_ERROR_CHECK(nvs_sync_create()); /* semaphore for thread synchronization on NVS memory */



    // Initialize WIFI
    wifi_manager_start();

    //Initialize WebServer
    ESP_ERROR_CHECK(start_rest_server(CONFIG_EXAMPLE_WEB_MOUNT_POINT, true));
    wifi_manager_set_callback(WM_ORDER_START_AP, &cb_restart_rest_server);
    wifi_manager_set_callback(WM_ORDER_STOP_AP, &cb_restart_rest_server);


    // Initialize SystemInfo
    system_info_start(true);

    // Initialize Sensor Manager
    sensor_manager_start(true);

    //Time Manger for updating history
    //time_manager_start(false);

    ESP_LOGI("main", "--- DEVICE BOOTED ---");


}
