
//
// Request Status of the System
//

#ifndef LOGME_INTERFACE_TIME_MANAGER_H
#define LOGME_INTERFACE_TIME_MANAGER_H

#include <stddef.h>
#include <esp_err.h>
#include <esp_log.h>
#include <stdbool.h>
#include <freertos/portmacro.h>
#include "cron.h"

#ifdef __cplusplus
extern "C"
{
#endif


typedef enum message_time_manager_code_t {
    TM_NONE = 0,
    TM_INIT = 1,
    CREATE_TASKS = 2
} message_time_manager_code_t;

typedef struct {
    message_time_manager_code_t code;
    void *param;
} queue_time_manager;

void cron_job_history_callback(cron_job *job);

void cron_job_pull_sensors();

void time_manager(void *pvParameters);

void time_manager_start(bool log_enable);

int check_time(int time_value);

void time_manager_destroy();

BaseType_t time_manager_send_message(message_time_manager_code_t code, void *param);

#ifdef __cplusplus
}
#endif

#endif
