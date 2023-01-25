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

QueueHandle_t system_info_queue;
SemaphoreHandle_t system_info_json_mutex = NULL;
static EventGroupHandle_t system_info_event_group;
static TaskHandle_t system_info_task = NULL;

char *system_info_json = NULL;


void system_info_start() {
    esp_log_level_set(SYSTEM_INFO_TAG, ESP_LOG_NONE);
    ESP_LOGI(SYSTEM_INFO_TAG, "Starting System-Info task...");
    system_info_queue = xQueueCreate(3, sizeof(queue_system_message));
    system_info_json_mutex = xSemaphoreCreateMutex();

    system_info_json = (char *) malloc(sizeof(char) * JSON_SYSTEM_INFO_SIZE);
    system_info_clear_info_json();

    system_info_json_mutex = xSemaphoreCreateMutex();
    system_info_event_group = xEventGroupCreate();

    xTaskCreate(&system_info, "system_info", 4096, NULL, 5, &system_info_task);

    ESP_LOGI(SYSTEM_INFO_TAG, "...started System-Info task");
}

void system_info_destroy() {
    vTaskDelete(system_info_task);
    system_info_task = NULL;

    free(system_info_json);

    vSemaphoreDelete(system_info_json_mutex);
    system_info_json_mutex = NULL;

    vEventGroupDelete(system_info_event_group);
    system_info_event_group = NULL;
}


void system_info(void *pvParameters) {
    queue_system_message msg;
    BaseType_t xStatus;
    EventBits_t uxBits;
    uint8_t retries = 0;
    
}

void system_info_clear_info_json() {
    strcpy(system_info_json, "{}\n");
};

char *system_info_get_info_json() {
    return system_info_json;
};

//esp_err_t *system_info_save_info_json() {};

