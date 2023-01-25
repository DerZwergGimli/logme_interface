//
// Request Status of the System
//

#ifndef SYSTEM_INFO_H_INCLUDED
#define SYSTEM_INFO_H_INCLUDED

#include <stddef.h>
#include <esp_err.h>
#include <esp_log.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum message_system_code_t {
    SI_NONE = 0,
    SI_INIT = 1,
    SI_SYNCING = 2,
    SI_READY = 3
} message_system_code_t;

typedef struct {
    message_system_code_t code;
    void *param;
} queue_system_message;

#define JSON_SYSTEM_INFO_SIZE 250

static const char *SYSTEM_INFO_TAG = "SYSTEM_INFO";


void system_info(void *pvParameters);

void system_info_start();

void system_info_destroy();

void system_info_clear_info_json();

char *system_info_get_info_json();

esp_err_t *system_info_save_info_json();


#ifdef __cplusplus
}
#endif

#endif
