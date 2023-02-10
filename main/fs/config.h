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
    char protocol[5];
    char ip[16];
    char port[5];
    char username[20];
    char password[20];
} config_mqtt_t;

typedef struct config_cron_jobs_t {
    bool enabled;
    char name[20];
    char description[50];
    char schedule[10];
} config_cron_jobs_t;


config_mqtt_t *config_get_mqtt();

config_cron_jobs_t *config_get_cron_jobs();

void write_config();

esp_err_t config_parse_file(char *json_data);

void print_config_mqtt();

void print_config_cron_jobs();

void config_read_file(const char *filename);

#endif //LOGME_INTERFACE_CONFIG_H
