//
// Created by yannick on 25.01.23.
//

#ifndef LOGME_INTERFACE_SML_SENSOR_T_H
#define LOGME_INTERFACE_SML_SENSOR_T_H


#include <cJSON.h>
#include <string.h>

typedef struct sml_smart_meter_history_t {
    int16_t day_24_kw[24];
    int16_t week_7_kw[7];
    int16_t month_30_kw[30];
} sml_smart_meter_history_t;

typedef struct sml_smart_meter_sensor_t {
    int16_t id;
    char name[20];
    int16_t count;
    int16_t power;
    sml_smart_meter_history_t history;
} sml_smart_meter_sensor_t;


typedef struct sml_smart_meter_ws_t {
    int16_t id;
    int32_t timestamp;
    int16_t value;
} sml_smart_meter_ws_t;

static int parse_sml_smart_meter_sensors(sml_smart_meter_sensor_t *sensor_array, const char *json_data) {
    const cJSON *sensor = NULL;
    cJSON *sensors = cJSON_Parse(json_data);
    int status = 0;
    if (sensors == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            ESP_LOGE("SML-Parser", "Error before: %s\n", error_ptr);
        }
        status = 0;

        cJSON_Delete(sensors);
        return status;
    }

    int index = 0;
    cJSON_ArrayForEach(sensor, sensors) {
        //cJSON *id = cJSON_GetObjectItem(sensors, "id");

        //Mapping JSON
        if (cJSON_IsNumber(cJSON_GetObjectItem(sensor, "id")))
            sensor_array[index].id = (int16_t) cJSON_GetObjectItem(sensor, "id")->valueint;
        if (cJSON_IsString(cJSON_GetObjectItem(sensor, "name")))
            strcpy(sensor_array[index].name, cJSON_GetObjectItem(sensor, "name")->valuestring);
        if (cJSON_IsNumber(cJSON_GetObjectItem(sensor, "count")))
            sensor_array[index].count = (int16_t) cJSON_GetObjectItem(sensor, "count")->valueint;
        if (cJSON_IsNumber(cJSON_GetObjectItem(sensor, "power")))
            sensor_array[index].power = (int16_t) cJSON_GetObjectItem(sensor, "power")->valueint;
        index++;
    }

    cJSON_Delete(sensors);
    return status;
}

#endif //LOGME_INTERFACE_SML_SENSOR_T_H
