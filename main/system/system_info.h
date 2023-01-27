
//
// Request Status of the System
//

#ifndef SYSTEM_INFO_H_INCLUDED
#define SYSTEM_INFO_H_INCLUDED

#include <stddef.h>
#include <esp_err.h>
#include <esp_log.h>
#include <stdbool.h>
#include <freertos/portmacro.h>


#ifdef __cplusplus
extern "C"
{
#endif


typedef enum message_system_code_t {
    SI_NONE = 0,
    SI_INIT = 1,
    SI_UPDATE = 2,
    SI_SLEEP = 3,
    SI_KILL = 4,
} message_system_code_t;

typedef struct {
    message_system_code_t code;
    void *param;
} queue_system_message;


void system_info(void *pvParameters);

void system_info_start(bool log_enable);

void system_info_destroy();

bool system_info_lock_info_json_buffer(TickType_t xTicksToWait);

void system_info_unlock_info_json_buffer();

void system_info_clear_info_json();

esp_err_t system_info_generate_info_json();

char *system_info_get_info_json();

esp_err_t *system_info_save_info_json();

BaseType_t system_info_send_message(message_system_code_t code, void *param);

#ifdef __cplusplus
}
#endif

#endif
