//
// Stores and Manages the Sensors
//

#ifndef LOGME_INTERFACE_SENSOR_MANAGER_H
#define LOGME_INTERFACE_SENSOR_MANAGER_H

#include <stddef.h>
#include <esp_err.h>
#include <esp_log.h>
#include <stdbool.h>
#include <freertos/portmacro.h>

#include "sml_sensor_t.h"

#define USE_SENSOR SML
typedef enum message_sensor_manager_t {
    SM_NONE = 0,
    SM_IDLE = 1,
    SM_INIT = 2,
    SM_LOOP = 3,
    SM_SAVE = 4,

} message_sensor_manager_t;

typedef struct {
    message_sensor_manager_t code;
    void *param;
} queue_sensor_manager;


void sensor_manager(void *pvParameters);

void sensor_manager_start(bool log_enable);

void sensor_manager_destroy();

void sensor_manager_clear_info_json();

bool sensor_store_lock_json_buffer(TickType_t xTicksToWait);

BaseType_t sensor_manager_send_message(message_sensor_manager_t code, void *param);

#endif //LOGME_INTERFACE_SENSOR_MANAGER_H
