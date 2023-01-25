#include "sensor_manager.h"
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
#include "esp_vfs.h"
#include <fcntl.h>

#define JSON_SENSOR_MANGER_SIZE 250
static const char *SENSOR_MANAGER_TAG = "sensor-manager";
#define SCRATCH_BUFSIZE (10240)
char scratch[SCRATCH_BUFSIZE];

QueueHandle_t sensor_manager_queue;
SemaphoreHandle_t sensor_manager_json_mutex = NULL;
static EventGroupHandle_t sensor_manager_event_group;
static TaskHandle_t sensor_manager_task = NULL;

char *sensor_manager_json = NULL;
sml_smart_meter_sensor_t sensors[4];

char *base_path = NULL;

void sensor_manager(void *pvParameters) {
    queue_sensor_manager msg;
    BaseType_t xStatus;
    EventBits_t uxBits;
    uint8_t retries = 0;

    sensor_manager_send_message(SM_INIT, NULL);
    ESP_LOGI(SENSOR_MANAGER_TAG, "Creating Task...");
    for (;;) {
        xStatus = xQueueReceive(sensor_manager_queue, &msg, pdMS_TO_TICKS(1000));

        if (xStatus == pdPASS) {
            switch (msg.code) {
                case SM_IDLE: {
                    ESP_LOGI(SENSOR_MANAGER_TAG, "SM_IDLE");
                    sensor_manager_send_message(SM_LOOP, NULL);
                }
                    break;
                case SM_INIT: {
                    ESP_LOGI(SENSOR_MANAGER_TAG, "SM_INIT");

                    // Open renamed file for reading
                    ESP_LOGI(SENSOR_MANAGER_TAG, "Reading file");

                    char filepath[ESP_VFS_PATH_MAX + 128];
                    strlcpy(filepath, CONFIG_EXAMPLE_WEB_MOUNT_POINT, sizeof(filepath));
                    strlcat(filepath, "/sensorstore.json", sizeof(filepath));
                    int fd = open(filepath, O_RDONLY, 0);
                    if (fd == -1) {
                        ESP_LOGE(SENSOR_MANAGER_TAG, "Failed to open file for reading: %s", filepath);
                        /* Respond with 500 Internal Server Error */

                        //return ESP_FAIL;
                    } else {
                        ESP_LOGI(SENSOR_MANAGER_TAG, "Opened storage file");
                    }

                    char *chunk = scratch;
                    ssize_t read_bytes;
                    do {
                        /* Read file in chunks into the scratch buffer */
                        read_bytes = read(fd, chunk, SCRATCH_BUFSIZE);
                        if (read_bytes == -1) {
                            ESP_LOGE(SENSOR_MANAGER_TAG, "Failed to read file : %s", filepath);
                        } else if (read_bytes > 0) {
                            ESP_LOGI(SENSOR_MANAGER_TAG, "%s", chunk);
                        }
                    } while (read_bytes > 0);
                    /* Close file after sending complete */
                    close(fd);


                    parse_sml_smart_meter_sensors(sensors, chunk);

                    ESP_LOGI(SENSOR_MANAGER_TAG, "%s", sensors[0].name);

                    sensor_manager_send_message(SM_IDLE, NULL);

                }
                    break;
                case SM_LOOP: {
                    ESP_LOGI(SENSOR_MANAGER_TAG, "SM_LOOP");
                    vTaskDelay(pdMS_TO_TICKS(2000));
                    sensor_manager_send_message(SM_LOOP, NULL);
                }
                    break;
                case SM_SAVE: {
                    ESP_LOGI(SENSOR_MANAGER_TAG, "SM_SAVE");
                    sensor_manager_send_message(SM_IDLE, NULL);

                }
                    break;
                default: {
                    ESP_LOGI(SENSOR_MANAGER_TAG, "SI_DEFAULT");
                    ESP_LOGE(SENSOR_MANAGER_TAG, "Destroy");
                    sensor_manager_destroy();
                }
                    break;
            }
        }
    }

    vTaskDelete(NULL);


};

void sensor_manager_start(bool log_enable) {
    if (!log_enable) {
        esp_log_level_set(SENSOR_MANAGER_TAG, ESP_LOG_NONE);
    }

    ESP_LOGI(SENSOR_MANAGER_TAG, "Starting up Sensor-Manager task...");
    sensor_manager_queue = xQueueCreate(3, sizeof(queue_sensor_manager));

    sensor_manager_json = (char *) malloc(sizeof(char) * JSON_SENSOR_MANGER_SIZE);
    sensor_manager_clear_info_json();

    sensor_manager_json_mutex = xSemaphoreCreateMutex();
    sensor_manager_event_group = xEventGroupCreate();

    xTaskCreate(&sensor_manager, "sensor_manager", 4096, NULL, 5, &sensor_manager_task);


};

void sensor_manager_clear_info_json() {
    strcpy(sensor_manager_json, "{}\n");
};

void sensor_manager_destroy() {
    vTaskDelete(sensor_manager_task);
    sensor_manager_task = NULL;

    free(sensor_manager_json);

    vSemaphoreDelete(sensor_manager_json_mutex);
    sensor_manager_json_mutex = NULL;

    vEventGroupDelete(sensor_manager_event_group);
    sensor_manager_event_group = NULL;
    vQueueDelete(sensor_manager_queue);
    sensor_manager_queue = NULL;
};

bool sensor_store_lock_json_buffer(TickType_t xTicksToWait) {
    if (sensor_manager_json_mutex) {
        if (xSemaphoreTake(sensor_manager_json_mutex, xTicksToWait) == pdTRUE) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
};

BaseType_t sensor_manager_send_message(message_sensor_manager_t code, void *param) {
    queue_sensor_manager msg;
    msg.code = code;
    msg.param = param;
    return xQueueSend(sensor_manager_queue, &msg, portMAX_DELAY);
};
