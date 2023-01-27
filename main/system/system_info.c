//
//  Request Status of the System Implementation
//

#include "system_info.h"
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>
#include <freertos/event_groups.h>
#include <string.h>
#include <esp_log.h>
#include <esp_chip_info.h>
#include <esp_timer.h>
#include <esp_flash.h>
#include "cJSON.h"

#define JSON_SYSTEM_INFO_SIZE 250
static const char *SYSTEM_INFO_TAG = "system-info";

QueueHandle_t system_info_queue;
SemaphoreHandle_t system_info_json_mutex = NULL;
static EventGroupHandle_t system_info_event_group;
static TaskHandle_t system_info_task = NULL;

char *system_info_json = NULL;


void system_info_start(bool log_enable) {

    if (!log_enable) {
        esp_log_level_set(SYSTEM_INFO_TAG, ESP_LOG_NONE);
    }
    ESP_LOGI(SYSTEM_INFO_TAG, "Starting up System-Info task...");
    system_info_queue = xQueueCreate(3, sizeof(queue_system_message));

    system_info_json = (char *) malloc(sizeof(char) * JSON_SYSTEM_INFO_SIZE);
    system_info_clear_info_json();

    system_info_json_mutex = xSemaphoreCreateMutex();
    system_info_event_group = xEventGroupCreate();

    xTaskCreate(&system_info, "system_info", 4096, NULL, 1, &system_info_task);

}

void system_info_destroy() {
    vTaskDelete(system_info_task);
    system_info_task = NULL;

    free(system_info_json);

    vSemaphoreDelete(system_info_json_mutex);
    system_info_json_mutex = NULL;

    vEventGroupDelete(system_info_event_group);
    system_info_event_group = NULL;
    vQueueDelete(system_info_queue);
    system_info_queue = NULL;
}


void system_info(void *pvParameters) {
    queue_system_message msg;
    BaseType_t xStatus;
    EventBits_t uxBits;
    uint8_t retries = 0;

    system_info_send_message(SI_INIT, NULL);

    ESP_LOGI(SYSTEM_INFO_TAG, "Creating Task...");
    for (;;) {
        xStatus = xQueueReceive(system_info_queue, &msg, pdMS_TO_TICKS(100));

        if (xStatus == pdPASS) {
            switch (msg.code) {
                case SI_INIT: {
                    ESP_LOGI(SYSTEM_INFO_TAG, "SI_INIT");
                    system_info_send_message(SI_UPDATE, NULL);
                }
                    break;
                case SI_IDLE: {
                    ESP_LOGI(SYSTEM_INFO_TAG, "SI_IDLE");
                }
                    break;
                case SI_UPDATE: {
                    ESP_LOGI(SYSTEM_INFO_TAG, "SI_UPDATE");
                    if (system_info_lock_info_json_buffer(pdMS_TO_TICKS(1000))) {
                        ESP_ERROR_CHECK(system_info_generate_info_json());
                        system_info_unlock_info_json_buffer();
                    } else {
                        ESP_LOGE(SYSTEM_INFO_TAG, "could not get access to json mutex in system_info");
                    }
                    system_info_send_message(SI_IDLE, NULL);
                }
                    break;

                case SI_KILL: {
                    ESP_LOGI(SYSTEM_INFO_TAG, "SI_KILL");
                    system_info_destroy();
                }
                default: {
                    ESP_LOGI(SYSTEM_INFO_TAG, "SI_DEFAULT");
                    ESP_LOGE(SYSTEM_INFO_TAG, "Destroy");
                    system_info_destroy();
                }
                    break;
            }
        }
    }

    vTaskDelete(NULL);
};


bool system_info_lock_info_json_buffer(TickType_t xTicksToWait) {
    if (system_info_json_mutex) {
        if (xSemaphoreTake(system_info_json_mutex, xTicksToWait) == pdTRUE) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
};

void system_info_unlock_info_json_buffer() {
    xSemaphoreGive(system_info_json_mutex);
}

esp_err_t system_info_generate_info_json() {
    memset(system_info_json, 0x00, JSON_SYSTEM_INFO_SIZE);
    // Request data for system info
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    uint32_t flash_size;
    ESP_ERROR_CHECK(esp_flash_get_size(NULL, &flash_size));

    //Build JSON
    cJSON *root = cJSON_CreateObject();
    cJSON *info = cJSON_CreateObject();
    cJSON_AddStringToObject(info, "target", CONFIG_IDF_TARGET);
    cJSON_AddNumberToObject(info, "cores", chip_info.cores);
    cJSON_AddNumberToObject(info, "major_rev", (uint32_t) (chip_info.revision / 100));
    cJSON_AddNumberToObject(info, "minor_rev", chip_info.revision % 100);
    cJSON_AddNumberToObject(info, "flash_size_MB", (uint32_t) (flash_size / (uint32_t) (1024 * 1024)));
    cJSON_AddNumberToObject(info, "total_heap", heap_caps_get_total_size(MALLOC_CAP_8BIT));
    cJSON_AddNumberToObject(info, "free_heap", esp_get_minimum_free_heap_size());
    cJSON_AddNumberToObject(info, "uptime_ms", (uint32_t) (esp_timer_get_time() / 1000));
    cJSON_AddItemToObject(root, "info", info);

    const char *json_object = cJSON_Print(root);
    strcat(system_info_json, json_object);
    free((void *) json_object);

    cJSON_Delete(root);
    return ESP_OK;
}

void system_info_clear_info_json() {
    strcpy(system_info_json, "{}\n");
};

char *system_info_get_info_json() {
    return system_info_json;
};

BaseType_t system_info_send_message(message_system_code_t code, void *param) {
    queue_system_message msg;
    msg.code = code;
    msg.param = param;
    return xQueueSend(system_info_queue, &msg, portMAX_DELAY);
}

//esp_err_t *system_info_save_info_json() {};

