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

#define JSON_SENSOR_MANGER_SIZE 10240
static const char *SENSOR_MANAGER_TAG = "sensor-manager";
#define SENSORS_LENGTH 4
#define SCRATCH_BUFSIZE (10240)
char scratch[SCRATCH_BUFSIZE];

QueueHandle_t sensor_manager_queue;
SemaphoreHandle_t sensor_manager_json_mutex = NULL;
static EventGroupHandle_t sensor_manager_event_group;
static TaskHandle_t sensor_manager_task = NULL;

char *sensor_manager_json = NULL;
sml_smart_meter_sensor_t sensors[SENSORS_LENGTH];

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
                            ESP_LOGI(SENSOR_MANAGER_TAG, "Loading json data...");
                            if (sensor_manager_lock_json_buffer(pdMS_TO_TICKS(portMAX_DELAY))) {
                                sensor_manager_json_parse(chunk);
                                ESP_ERROR_CHECK(sensor_manager_generate_json());
                                sensor_manager_unlock_json_buffer();
                            } else {
                                ESP_LOGE(SENSOR_MANAGER_TAG, "could not get access to json mutex in system_info");
                            }

                        }
                    } while (read_bytes > 0);
                    /* Close file after sending complete */
                    close(fd);


                    ESP_LOGI(SENSOR_MANAGER_TAG, "%s", sensors[0].name);
                    sensor_manager_send_message(SM_IDLE, NULL);

                }
                    break;
                case SM_SAVE: {
                    ESP_LOGI(SENSOR_MANAGER_TAG, "SM_SAVE");
                    sensor_manager_send_message(SM_IDLE, NULL);

                }
                    break;
                case SM_KILL: {
                    ESP_LOGI(SENSOR_MANAGER_TAG, "SM_KILL");
                    sensor_manager_destroy();
                }
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


esp_err_t sensor_manager_generate_json() {
    memset(sensor_manager_json, 0x00, JSON_SENSOR_MANGER_SIZE);

    cJSON *root = cJSON_CreateArray();

    for (int index = 0; index < 4; index++) {
        cJSON *element = cJSON_CreateObject();

        cJSON_AddNumberToObject(element, "id", sensors[index].id);
        cJSON_AddStringToObject(element, "name", sensors[index].name);
        cJSON_AddNumberToObject(element, "count", sensors[index].count);
        cJSON_AddNumberToObject(element, "power", sensors[index].power);

        cJSON *history = cJSON_CreateObject();
        cJSON_AddItemToObject(element, "history", history);

        cJSON *day_24_kw = cJSON_AddArrayToObject(history, "day_24_kw");
        for (int i = 0; i < NELEMS(sensors[index].history.day_24_kw); i++) {
            if (cJSON_IsNumber(cJSON_CreateNumber(sensors[index].history.day_24_kw[i]))) {
                cJSON *value = cJSON_CreateNumber(sensors[index].history.day_24_kw[i]);
                cJSON_AddItemToArray(day_24_kw, value);
            }
        }

        cJSON *week_7_kw = cJSON_AddArrayToObject(history, "week_7_kw");
        for (int i = 0; i < NELEMS(sensors[index].history.week_7_kw); i++) {
            if (cJSON_IsNumber(cJSON_CreateNumber(sensors[index].history.week_7_kw[i]))) {
                cJSON *value = cJSON_CreateNumber(sensors[index].history.week_7_kw[i]);
                cJSON_AddItemToArray(week_7_kw, value);
            }
        }

        cJSON *month_30_kw = cJSON_AddArrayToObject(history, "month_30_kw");
        for (int i = 0; i < NELEMS(sensors[index].history.month_30_kw); i++) {
            if (cJSON_IsNumber(cJSON_CreateNumber(sensors[index].history.month_30_kw[i]))) {
                cJSON *value = cJSON_CreateNumber(sensors[index].history.month_30_kw[i]);
                cJSON_AddItemToArray(month_30_kw, value);
            }
        }

        cJSON_AddItemToArray(root, element);
    }

    const char *json_object = cJSON_Print(root);
    strcat(sensor_manager_json, json_object);
    free((void *) json_object);

    cJSON_Delete(root);
    return ESP_OK;
}


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

bool sensor_manager_lock_json_buffer(TickType_t xTicksToWait) {
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

void sensor_manager_unlock_json_buffer() {
    xSemaphoreGive(sensor_manager_json_mutex);
}

char *sensor_manager_get_json() {
    return sensor_manager_json;
}

BaseType_t sensor_manager_send_message(message_sensor_manager_t code, void *param) {
    queue_sensor_manager msg;
    msg.code = code;
    msg.param = param;
    return xQueueSend(sensor_manager_queue, &msg, portMAX_DELAY);
};

esp_err_t sensor_manager_json_parse(const char *json_data) {
    const cJSON *sensor = NULL;
    cJSON *sensors_json = cJSON_Parse(json_data);
    int status = 0;
    if (sensors_json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            ESP_LOGE("SML-Parser", "Error before: %s\n", error_ptr);
        }
        cJSON_Delete(sensors_json);
        return ESP_OK;
    }

    int index = 0;
    cJSON_ArrayForEach(sensor, sensors_json) {
        //cJSON *id = cJSON_GetObjectItem(sensors, "id");

        //Mapping JSON
        if (cJSON_IsNumber(cJSON_GetObjectItem(sensor, "id")))
            sensors[index].id = (int16_t) cJSON_GetObjectItem(sensor, "id")->valueint;
        if (cJSON_IsString(cJSON_GetObjectItem(sensor, "name")))
            strcpy(sensors[index].name, cJSON_GetObjectItem(sensor, "name")->valuestring);
        if (cJSON_IsNumber(cJSON_GetObjectItem(sensor, "count")))
            sensors[index].count = (int32_t) cJSON_GetObjectItem(sensor, "count")->valueint;
        if (cJSON_IsNumber(cJSON_GetObjectItem(sensor, "power")))
            sensors[index].power = (int16_t) cJSON_GetObjectItem(sensor, "power")->valueint;


        if (cJSON_IsObject(cJSON_GetObjectItem(sensor, "history"))) {
            cJSON *history = cJSON_GetObjectItem(sensor, "history");

            if (cJSON_IsArray(cJSON_GetObjectItem(history, "day_24_kw"))) {
                cJSON *element = NULL;
                int i = 0;
                cJSON_ArrayForEach(element, cJSON_GetObjectItem(history, "day_24_kw")) {
                    sensors[index].history.day_24_kw[i] = (int16_t) cJSON_GetNumberValue(element);
                    i++;
                }
            }
            if (cJSON_IsArray(cJSON_GetObjectItem(history, "week_7_kw"))) {
                cJSON *element = NULL;
                int i = 0;
                cJSON_ArrayForEach(element, cJSON_GetObjectItem(history, "week_7_kw")) {
                    sensors[index].history.week_7_kw[i] = (int16_t) cJSON_GetNumberValue(element);
                    i++;
                }
            }
            if (cJSON_IsArray(cJSON_GetObjectItem(history, "month_30_kw"))) {
                cJSON *element = NULL;
                int i = 0;
                cJSON_ArrayForEach(element, cJSON_GetObjectItem(history, "month_30_kw")) {
                    sensors[index].history.month_30_kw[i] = (int16_t) cJSON_GetNumberValue(element);
                    i++;
                }
            }
        }
        index++;
    }

    cJSON_Delete(sensors_json);
    return ESP_OK;
}

