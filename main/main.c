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
#include "protocol_examples_common.h"

#if CONFIG_EXAMPLE_WEB_DEPLOY_SD
#include "driver/sdmmc_host.h"
#endif

#include "web/mdns.h"
#include "fs/fs.h"
#include "sml_sensor/smart_meter_t.h"
#include "sensor_tasks.h"

#include <esp_http_server.h>

#include "wifi/wifi_manager.h"
#include "web/rest_types.h"
#include "web/rest_server_helper.h"

// GLOBALS
static const char TAG_MAIN[] = "main";
// WiFi-Config
esp_netif_t *netif_config;
esp_netif_ip_info_t ip_info;

// SmartMeter
smart_meter_sensor_t sensors[CONFIG_SMART_METER_SENSOR_LENGTH];
//
// esp_netif_t *wifi_init_sta(void);
// esp_err_t start_rest_server(const char *base_path, esp_netif_t *netif_config, smart_meter_sensor_t *sensors);

void cb_connection_ok(void *pvParameter)
{
    ip_event_got_ip_t *param = (ip_event_got_ip_t *)pvParameter;

    /* transform IP to human readable string */
    char str_ip[16];
    esp_ip4addr_ntoa(&param->ip_info.ip, str_ip, 16);

    ESP_LOGI(TAG_MAIN, "I have a connection and my IP is %s!", str_ip);
}

void monitoring_task(void *pvParameter)
{
    for (;;)
    {
        ESP_LOGI(TAG_MAIN, "free heap: %lu", esp_get_free_heap_size());
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}

void app_main(void)
{

    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    // ESP_ERROR_CHECK(nvs_sync_create()); /* semaphore for thread synchronization on NVS memory */

    ESP_ERROR_CHECK(ret);
    ESP_ERROR_CHECK(init_fs());

    wifi_manager_start();

    wifi_manager_set_callback(WM_EVENT_STA_GOT_IP, &cb_connection_ok);

    xTaskCreatePinnedToCore(&monitoring_task, "monitoring_task", 2048, NULL, 1, NULL, 1);

    // ESP_LOGI("WiFI-Config", "ESP_WIFI_MODE_STA");

    // netif_config = wifi_init_sta();
    // ESP_ERROR_CHECK(esp_netif_get_ip_info(netif_config, &ip_info));

    snprintf(sensors[0].name, "%s", "Sensor_1");
    snprintf(sensors[1].name, "%s", "Sensor_2");
    snprintf(sensors[2].name, "%s", "Sensor_3");
    snprintf(sensors[3].name, "%s", "Sensor_4");

    // ESP_LOGI("TEST", "IP is:" IPSTR,
    //          IP2STR(&ip_info->ip));

    // ESP_ERROR_CHECK(esp_netif_init());
    // ESP_ERROR_CHECK(esp_event_loop_create_default());
    // initialise_mdns();
    // netbiosns_init();
    //    netbiosns_set_name(CONFIG_EXAMPLE_MDNS_HOST_NAME);

    // ESP_ERROR_CHECK(example_connect());
    // ESP_ERROR_CHECK(init_fs());

    ESP_LOGI("main", "starting tasks");
    sensor_task_paramater_t sensorTaskParamater;
    sensorTaskParamater.sensors = &sensors;
    // xTaskCreate(sensor_task, "Sensor Task", 2048, (void *)&sensorTaskParamater, 1, NULL);
    //    vTaskStartScheduler();

    ESP_LOGI("main", "starting webserver");
    // ESP_ERROR_CHECK(start_rest_server(CONFIG_EXAMPLE_WEB_MOUNT_POINT, NULL, NULL));
}
