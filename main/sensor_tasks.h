#ifndef SENSOR_TASKS_H
#define SENSOR_TASKS_H

#include "esp_log.h"
#include "sml_sensor/smart_meter_t.h"

typedef struct sensor_task_paramater
{
    smart_meter_sensor_t *sensors;
} sensor_task_paramater_t;

void sensor_task(void *pvParameters)
{
    sensor_task_paramater_t *params = (sensor_task_paramater_t *)pvParameters;
    smart_meter_sensor_t *sensors = params->sensors;

    for (;;)
    {
        sensors[0].count += 1;
        ESP_LOGI("SENSOR-TASK", "%s", sensors[0].name);
        ESP_LOGI("SENSOR-TASK", "I'm sensor task!");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    vTaskDelete(NULL);
}

#endif