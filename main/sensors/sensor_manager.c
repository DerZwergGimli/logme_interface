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
#include "sensor_helper.h"
#include "mbus_sensor_t.h"
#include <fcntl.h>
#include <esp_heap_trace.h>

#define JSON_SENSOR_MANGER_SIZE 10240
static const char *SENSOR_MANAGER_TAG = "sensor-manager";
#define SENSORS_LENGTH 4
#define SCRATCH_BUFSIZE (10240)
char scratch[SCRATCH_BUFSIZE];

QueueHandle_t sensor_manager_queue;
SemaphoreHandle_t sensor_manager_json_mutex = NULL;
static EventGroupHandle_t sensor_manager_event_group;
static TaskHandle_t sensor_manager_task = NULL;


//old
char *sensor_manager_json = NULL;
sml_smart_meter_sensor_t sensors[SENSORS_LENGTH];

//new sensors

char *sensor_manager_mbus_devices = NULL;
mbus_device_t mbus_devices[CONFIG_LOGME_MBUS_DEVICES];


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
                case SM_INIT: {
                    ESP_LOGI(SENSOR_MANAGER_TAG, "SM_INIT");

                    // Open renamed file for reading
                    ESP_LOGI(SENSOR_MANAGER_TAG, "Reading file");

//                    char filepath[ESP_VFS_PATH_MAX + 128];
//                    strlcpy(filepath, CONFIG_EXAMPLE_WEB_MOUNT_POINT, sizeof(filepath));
//                    strlcat(filepath, "/sensorstore.json", sizeof(filepath));
//                    int fd = open(filepath, O_RDONLY, 0);
//                    if (fd == -1) {
//                        ESP_LOGE(SENSOR_MANAGER_TAG, "Failed to open file for reading: %s", filepath);
//                    } else {
//                        ESP_LOGI(SENSOR_MANAGER_TAG, "Opened storage file");
//                    }
//
//                    char *chunk = scratch;
//                    ssize_t read_bytes;
//                    do {
//                        /* Read file in chunks into the scratch buffer */
//                        read_bytes = read(fd, chunk, SCRATCH_BUFSIZE);
//                        if (read_bytes == -1) {
//                            ESP_LOGE(SENSOR_MANAGER_TAG, "Failed to read file : %s", filepath);
//                        } else if (read_bytes > 0) {
//                            ESP_LOGI(SENSOR_MANAGER_TAG, "Loading json data...");
//                            if (sensor_manager_lock_json_buffer(pdMS_TO_TICKS(portMAX_DELAY))) {
//                                sensor_manager_json_parse(chunk);
//                                ESP_ERROR_CHECK(sensor_manager_generate_json());
//                                sensor_manager_unlock_json_buffer();
//                            } else {
//                                ESP_LOGE(SENSOR_MANAGER_TAG, "could not get access to json mutex in system_info");
//                            }
//
//                        }
//                    } while (read_bytes > 0);
//                    /* Close file after sending complete */
//                    close(fd);



                    char filepath[ESP_VFS_PATH_MAX + 128];
                    strlcpy(filepath, CONFIG_EXAMPLE_WEB_MOUNT_POINT, sizeof(filepath));
                    strlcat(filepath, "/default_sensor_config.json", sizeof(filepath));
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
                                sensor_manager_json_parse_mbus(chunk);
                                ESP_ERROR_CHECK(sensor_manager_generate_json());
                                sensor_manager_unlock_json_buffer();
                            } else {
                                ESP_LOGE(SENSOR_MANAGER_TAG, "could not get access to json mutex in system_info");
                            }

                        }
                    } while (read_bytes > 0);
                    /* Close file after sending complete */
                    close(fd);

                    sensor_manager_send_message(SM_MBUS_PULL, NULL);
                }
                    break;
                case SM_SAVE_CONFIG: {
                    ESP_LOGI(SENSOR_MANAGER_TAG, "SM_SAVE_CONFIG");
                    char filepath[ESP_VFS_PATH_MAX + 128];
                    strlcpy(filepath, CONFIG_EXAMPLE_WEB_MOUNT_POINT, sizeof(filepath));
                    strlcat(filepath, "/sensorstore.json", sizeof(filepath));
                    FILE *f = fopen(filepath, "w");
                    if (f == NULL) {
                        ESP_LOGE(SENSOR_MANAGER_TAG, "Failed to open file for reading: %s", filepath);
                    } else {
                        ESP_LOGI(SENSOR_MANAGER_TAG, "Opened storage file");
                    }

                    if (sensor_manager_lock_json_buffer(pdMS_TO_TICKS(portMAX_DELAY))) {
                        const char *json_sensor_config = sensor_manager_get_json();
                        fprintf(f, "%s", json_sensor_config);
                        sensor_manager_unlock_json_buffer();
                    } else {
                        ESP_LOGE(SENSOR_MANAGER_TAG, "could not get access to json mutex in system_info");
                    }
                    fclose(f);
                    sensor_manager_send_message(SM_IDLE, NULL);

                }
                    break;
                case SM_CLEAR_HISTORY: {
                    ESP_LOGI(SENSOR_MANAGER_TAG, "SM_CLEAR_HISTORY");
                    for (int i = 0; i < sizeof(sensors) / sizeof(sensors[0]); i++) {
                        memset(sensors[i].history.day_24_kw, 0, sizeof sensors[i].history.day_24_kw);
                        memset(sensors[i].history.week_7_kw, 0, sizeof sensors[i].history.week_7_kw);
                        memset(sensors[i].history.month_30_kw, 0, sizeof sensors[i].history.month_30_kw);
                    }

                    if (sensor_manager_lock_json_buffer(pdMS_TO_TICKS(portMAX_DELAY))) {
                        ESP_ERROR_CHECK(sensor_manager_generate_json());
                        sensor_manager_unlock_json_buffer();
                    } else {
                        ESP_LOGE(SENSOR_MANAGER_TAG, "could not get access to json mutex in system_info");
                    }
                    ESP_LOGI(SENSOR_MANAGER_TAG, "...history cleared!");
                    sensor_manager_send_message(SM_IDLE, NULL);

                }
                    break;
                case SM_KILL: {
                    ESP_LOGI(SENSOR_MANAGER_TAG, "SM_KILL");
                    sensor_manager_destroy();
                }
                    break;
                case SM_UPDATE_HISTORY: {
                    ESP_LOGI(SENSOR_MANAGER_TAG, "SM_UPDATE_HISTORY");
                    sensor_manager_history_t *evt_history = (sensor_manager_history_t *) msg.param;
                    sensor_manager_update_history_save(*evt_history);

                }
                    break;
                case SM_MBUS_PULL: {
                    ESP_LOGI(SENSOR_MANAGER_TAG, "SM_MBUS_PULL");

                    for (int i = 0; i < NELEMS(mbus_devices); i++) {
                        mbus_devices[i].status = MBUS_READING;
                        if (sensor_manager_lock_json_buffer(pdMS_TO_TICKS(portMAX_DELAY))) {
                            ESP_ERROR_CHECK(sensor_manager_generate_json());
                            sensor_manager_unlock_json_buffer();
                        }

                        if (mbus_request_short(&mbus_devices[i].data, mbus_devices[i].pin_rx, mbus_devices[i].pin_tx,
                                               mbus_devices[i].baudrate, mbus_devices[i].primary_address) > 0) {
                            ESP_LOGE(SENSOR_MANAGER_TAG, "Error while pulling data from MBus.");
                        }


                        mbus_devices[i].status = MBUS_IDLE;
                        if (sensor_manager_lock_json_buffer(pdMS_TO_TICKS(portMAX_DELAY))) {
                            ESP_ERROR_CHECK(sensor_manager_generate_json());
                            sensor_manager_unlock_json_buffer();
                        }
                    }

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
    sensor_manager_mbus_devices = (char *) malloc(sizeof(char) * JSON_SENSOR_MANGER_SIZE);
    sensor_manager_clear_info_json();

    sensor_manager_json_mutex = xSemaphoreCreateMutex();
    sensor_manager_event_group = xEventGroupCreate();

    xTaskCreate(&sensor_manager, "sensor_manager", 4096, NULL, 5, &sensor_manager_task);


};


esp_err_t sensor_manager_generate_json() {
    //ESP_ERROR_CHECK(heap_trace_start(HEAP_TRACE_LEAKS));
    strcpy(sensor_manager_json, "[");
    for (int i = 0; i < NELEMS(mbus_devices); i++) {
        char buffer[10240];

        sprintf(buffer, "{ \"name\": \"%s\","
                        "\"id\": %u,"
                        "\"description\": \"%s\","
                        "\"status\": %u,"
                        "\"pin_rx\": %u,"
                        "\"pin_tx\": %u,"
                        "\"baudrate\": %u,"
                        "\"primary_address\": %u,"
                        "\"secondary_address\": %u",
                mbus_devices[i].name,
                mbus_devices[i].id,
                mbus_devices[i].description,
                mbus_devices[i].status,
                mbus_devices[i].pin_rx,
                mbus_devices[i].pin_tx,
                mbus_devices[i].baudrate,
                mbus_devices[i].primary_address,
                mbus_devices[i].secondary_address
        );

        if (mbus_devices[i].data != NULL) {
            strcat(sensor_manager_json, buffer);
            sprintf(buffer, ",\"data\": %s", mbus_devices[i].data);
        }
        strcat(buffer, "}");

        if (i < NELEMS(mbus_devices) - 1)
            strcat(buffer, ",");
        strcat(sensor_manager_json, buffer);
    }
    strcat(sensor_manager_json, "]");

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
            sensors[index].id = (uint16_t) cJSON_GetObjectItem(sensor, "id")->valueint;
        if (cJSON_IsString(cJSON_GetObjectItem(sensor, "name")))
            strcpy(sensors[index].name, cJSON_GetObjectItem(sensor, "name")->valuestring);
        if (cJSON_IsNumber(cJSON_GetObjectItem(sensor, "count")))
            sensors[index].count = cJSON_GetObjectItem(sensor, "count")->valueint;
        if (cJSON_IsNumber(cJSON_GetObjectItem(sensor, "power")))
            sensors[index].power = cJSON_GetObjectItem(sensor, "power")->valueint;


        if (cJSON_IsObject(cJSON_GetObjectItem(sensor, "history"))) {
            cJSON *history = cJSON_GetObjectItem(sensor, "history");

            if (cJSON_IsArray(cJSON_GetObjectItem(history, "day_24_kw"))) {
                cJSON *element = NULL;
                int i = 0;
                cJSON_ArrayForEach(element, cJSON_GetObjectItem(history, "day_24_kw")) {
                    sensors[index].history.day_24_kw[i] = (uint32_t) cJSON_GetNumberValue(element);
                    i++;
                }
            }
            if (cJSON_IsArray(cJSON_GetObjectItem(history, "week_7_kw"))) {
                cJSON *element = NULL;
                int i = 0;
                cJSON_ArrayForEach(element, cJSON_GetObjectItem(history, "week_7_kw")) {
                    sensors[index].history.week_7_kw[i] = (uint32_t) cJSON_GetNumberValue(element);
                    i++;
                }
            }
            if (cJSON_IsArray(cJSON_GetObjectItem(history, "month_30_kw"))) {
                cJSON *element = NULL;
                int i = 0;
                cJSON_ArrayForEach(element, cJSON_GetObjectItem(history, "month_30_kw")) {
                    sensors[index].history.month_30_kw[i] = (uint32_t) cJSON_GetNumberValue(element);
                    i++;
                }
            }
        }
        index++;
    }

    cJSON_Delete(sensors_json);
    return ESP_OK;
}

esp_err_t sensor_manager_json_parse_mbus(const char *json_data) {
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
            mbus_devices[index].id = (uint16_t) cJSON_GetObjectItem(sensor, "id")->valueint;
        if (cJSON_IsString(cJSON_GetObjectItem(sensor, "name")))
            strcpy(mbus_devices[index].name, cJSON_GetObjectItem(sensor, "name")->valuestring);
        if (cJSON_IsString(cJSON_GetObjectItem(sensor, "description")))
            strcpy(mbus_devices[index].description, cJSON_GetObjectItem(sensor, "description")->valuestring);
        if (cJSON_IsNumber(cJSON_GetObjectItem(sensor, "pin_rx")))
            mbus_devices[index].pin_rx = cJSON_GetObjectItem(sensor, "pin_rx")->valueint;
        if (cJSON_IsNumber(cJSON_GetObjectItem(sensor, "pin_tx")))
            mbus_devices[index].pin_tx = cJSON_GetObjectItem(sensor, "pin_tx")->valueint;
        if (cJSON_IsNumber(cJSON_GetObjectItem(sensor, "baudrate")))
            mbus_devices[index].baudrate = cJSON_GetObjectItem(sensor, "baudrate")->valueint;
        if (cJSON_IsNumber(cJSON_GetObjectItem(sensor, "primary_address")))
            mbus_devices[index].primary_address = cJSON_GetObjectItem(sensor, "primary_address")->valueint;
        if (cJSON_IsNumber(cJSON_GetObjectItem(sensor, "secondary_address")))
            mbus_devices[index].secondary_address = cJSON_GetObjectItem(sensor, "secondary_address")->valueint;



//        if (cJSON_IsObject(cJSON_GetObjectItem(sensor, "history"))) {
//            cJSON *history = cJSON_GetObjectItem(sensor, "history");
//
//            if (cJSON_IsArray(cJSON_GetObjectItem(history, "day_24_kw"))) {
//                cJSON *element = NULL;
//                int i = 0;
//                cJSON_ArrayForEach(element, cJSON_GetObjectItem(history, "day_24_kw")) {
//                    mbus_devices[index].history.day_24_kw[i] = (uint32_t) cJSON_GetNumberValue(element);
//                    i++;
//                }
//            }
//            if (cJSON_IsArray(cJSON_GetObjectItem(history, "week_7_kw"))) {
//                cJSON *element = NULL;
//                int i = 0;
//                cJSON_ArrayForEach(element, cJSON_GetObjectItem(history, "week_7_kw")) {
//                    mbus_devices[index].history.week_7_kw[i] = (uint32_t) cJSON_GetNumberValue(element);
//                    i++;
//                }
//            }
//            if (cJSON_IsArray(cJSON_GetObjectItem(history, "month_30_kw"))) {
//                cJSON *element = NULL;
//                int i = 0;
//                cJSON_ArrayForEach(element, cJSON_GetObjectItem(history, "month_30_kw")) {
//                    mbus_devices[index].history.month_30_kw[i] = (uint32_t) cJSON_GetNumberValue(element);
//                    i++;
//                }
//            }
//        }
        index++;
    }

    cJSON_Delete(sensors_json);
    return ESP_OK;
}


esp_err_t sensor_manager_update_history_save(sensor_manager_history_t timeframe) {
    ESP_LOGI(SENSOR_MANAGER_TAG, "Updating history: %i", timeframe);

    //TODO: FIND id  and value in data.salveDATA and add this as history


    if (sensor_manager_lock_json_buffer(pdMS_TO_TICKS(portMAX_DELAY))) {
        switch (timeframe) {
            case SM_HISTORY_SECOUND: {
//                for (int i = 0; i < (NELEMS(sensors)); i++) {
//                    for (int k = 1;
//                         k < (NELEMS(sensors[i].history.day_24_kw)); k++) {
//                        sensors[i].history.day_24_kw[k - 1] = sensors[i].history.day_24_kw[k];
//                    }
//                    sensors[i].history.day_24_kw[NELEMS(sensors[i].history.day_24_kw) - 1] = sensors[i].count;
//                }
            }
                break;


            case SM_HISTORY_HOUR: {
                for (int i = 0; i < (NELEMS(sensors)); i++) {
                    for (int k = 1;
                         k < (NELEMS(sensors[i].history.day_24_kw)); k++) {
                        sensors[i].history.day_24_kw[k - 1] = sensors[i].history.day_24_kw[k];
                    }
                    sensors[i].history.day_24_kw[NELEMS(sensors[i].history.day_24_kw) - 1] = sensors[i].count;
                }
            }
                break;
            case SM_HISTORY_DAY: {
                for (int i = 0; i < (NELEMS(sensors)); i++) {
                    for (int k = 1;
                         k < (NELEMS(sensors[i].history.week_7_kw)); k++) {
                        sensors[i].history.week_7_kw[k - 1] = sensors[i].history.week_7_kw[k];
                    }
                    sensors[i].history.week_7_kw[NELEMS(sensors[i].history.week_7_kw) - 1] = sensors[i].count;
                }

                for (int i = 0; i < (NELEMS(sensors)); i++) {
                    for (int k = 1;
                         k < (NELEMS(sensors[i].history.month_30_kw)); k++) {
                        sensors[i].history.month_30_kw[k - 1] = sensors[i].history.month_30_kw[k];
                    }
                    sensors[i].history.month_30_kw[NELEMS(sensors[i].history.month_30_kw) - 1] = sensors[i].count;
                }
            }
                break;
            default:
                break;

        }
        ESP_ERROR_CHECK(sensor_manager_generate_json());
        sensor_manager_unlock_json_buffer();

    } else {
        ESP_LOGE(SENSOR_MANAGER_TAG, "could not get access to json mutex in sensor_manager");
    }
    return ESP_OK;
}
