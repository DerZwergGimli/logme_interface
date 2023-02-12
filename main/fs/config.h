//
// Interface for loading/saving the config
//

#ifndef LOGME_INTERFACE_CONFIG_H
#define LOGME_INTERFACE_CONFIG_H

#include "esp_vfs.h"
#include "cJSON.h"
#include "esp_log.h"
#include <sys/fcntl.h>


typedef struct config_mqtt_t {
    bool enabled;
    char protocol[10];
    char ip[16];
    char port[5];
    char username[50];
    char password[100];
} config_mqtt_t;

typedef struct config_cron_jobs_t {
    bool enabled;
    char name[20];
    char description[100];
    char schedule[20];
} config_cron_jobs_t;


void config_init();

void config_destroy();

bool config_lock(TickType_t xTicksToWait);

void config_unlock();

config_mqtt_t *config_get_mqtt();

config_cron_jobs_t *config_get_cron_jobs();

cJSON *config_get_mqtt_json();

cJSON *config_get_cron_jobs_json();


esp_err_t config_write_to_flash();

esp_err_t config_parse_file(char *json_data);

esp_err_t config_parse_mqtt(const cJSON *json_mqtt);

esp_err_t config_parse_cron_jobs(const cJSON *json_cron_jobs);

void print_config_mqtt();

void print_config_cron_jobs();

void config_read_file();

#endif //LOGME_INTERFACE_CONFIG_H
