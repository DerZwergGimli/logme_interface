#include "sdkconfig.h"

#include "esp_spiffs.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_log.h"
#include "mbus_serial_scan.h"
#include "fs/fs.h"
#include <esp_heap_trace.h>
#include "wifi/nvs_sync.h"
#include "system/system_info.h"
#include "web/rest_server.h"

#include "timer/timer_manager.h"
#include "sensors/sensor_helper.h"
#include "mqtt/mqtt_manager.h"


// GLOBALS
static const char TAG_MAIN[] = "main";

esp_netif_ip_info_t ip_info;


void cb_restart_rest_server(void *pvParameter) {
    ESP_LOGI(TAG_MAIN, "Restarting WebServer...");
    ESP_ERROR_CHECK(stop_rest_server());
    ESP_ERROR_CHECK(start_rest_server(CONFIG_EXAMPLE_WEB_MOUNT_POINT, true));

}

void cb_start_mqtt_client(void *pvParameter) {
    ESP_LOGI(TAG_MAIN, "Staring MQTT...");
    start_mqtt5_app();
    for (int i = 0; i < 10; ++i) {

        mqtt_message_t message;
        strcpy(message.topic, "/test");
        strcpy(message.message, "Hello from LogME");
        send_message_async(message);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }

}


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
    wifi_manager_set_callback(WM_EVENT_STA_GOT_IP, &cb_start_mqtt_client);


    // Initialize SystemInfo
    system_info_start(true);

    // Initialize Sensor Manager
    sensor_manager_start(true);

    //Time Manger for updating history
    //time_manager_start(false);


    ESP_LOGI("main", "--- DEVICE BOOTED ---");


}
