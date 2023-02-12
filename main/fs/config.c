//
// Implementation for loading/saving the config
//

#include "config.h"
#include "file_hanlder.h"

#define SCRATCH_BUFFER_SIZE_MAX 1024
#define CRON_JOBS_LENGTH 2
static const char *FS_CONFIG_TAG = "fs-config";
#define FILE_SYSTEM_CONFIG "/default_system_config.json"
#define NUM_ALPHA  26

config_mqtt_t *config_mqtt;
config_cron_jobs_t config_cron_jobs[CRON_JOBS_LENGTH];


config_mqtt_t *config_get_mqtt() {
    return config_mqtt;
}


config_cron_jobs_t *config_get_cron_jobs() {
    return config_cron_jobs;
}

cJSON *config_get_mqtt_json() {
    cJSON *json_mqtt = cJSON_CreateObject();
    cJSON_AddBoolToObject(json_mqtt, "enabled", config_mqtt->enabled);
    cJSON_AddStringToObject(json_mqtt, "protocol", config_mqtt->protocol);
    cJSON_AddStringToObject(json_mqtt, "ip", config_mqtt->ip);
    cJSON_AddStringToObject(json_mqtt, "port", config_mqtt->port);
    cJSON_AddStringToObject(json_mqtt, "username", config_mqtt->username);
    cJSON_AddStringToObject(json_mqtt, "password", config_mqtt->password);
    return json_mqtt;
}

cJSON *config_get_cron_jobs_json() {
    cJSON *json_cron_jobs = cJSON_CreateArray();
    for (int i = 0; i < CRON_JOBS_LENGTH; ++i) {
        cJSON *json_cron_job = cJSON_CreateObject();
        cJSON_AddBoolToObject(json_cron_job, "enabled", config_cron_jobs[i].enabled);
        cJSON_AddStringToObject(json_cron_job, "name", config_cron_jobs[i].name);
        cJSON_AddStringToObject(json_cron_job, "name", config_cron_jobs[i].name);
        cJSON_AddStringToObject(json_cron_job, "description", config_cron_jobs[i].description);
        cJSON_AddStringToObject(json_cron_job, "schedule", config_cron_jobs[i].schedule);
        cJSON_AddItemToArray(json_cron_jobs, json_cron_job);
    }
    return json_cron_jobs;
}


esp_err_t config_write_to_flash() {
    ESP_LOGI(FS_CONFIG_TAG, "Loading file: %s", FILE_SYSTEM_CONFIG);

    char filepath[ESP_VFS_PATH_MAX + 128];
    strlcpy(filepath, CONFIG_EXAMPLE_WEB_MOUNT_POINT, sizeof(filepath));
    strlcat(filepath, FILE_SYSTEM_CONFIG, sizeof(filepath));

    cJSON *json_config = cJSON_CreateObject();
    cJSON *json_mqtt_config = config_get_mqtt_json();
    cJSON *json_cron_jobs_config = config_get_cron_jobs_json();

    cJSON_AddItemToObject(json_config, "mqtt", json_mqtt_config);
    cJSON_AddItemToObject(json_config, "cron_jobs", json_cron_jobs_config);

    file_handler_write_file(filepath, json_config);
    cJSON_Delete(json_config);

    return ESP_OK;
}

esp_err_t config_parse_file(char *json_data) {
    cJSON *root = cJSON_Parse(json_data);

    const cJSON *json_mqtt;
    const cJSON *json_cron_jobs;
    
    if (root == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            ESP_LOGE(FS_CONFIG_TAG, "Error before: %s\n", error_ptr);
        }
        cJSON_Delete(root);
        return ESP_FAIL;
    }

    json_mqtt = cJSON_GetObjectItemCaseSensitive(root, "mqtt");
    json_cron_jobs = cJSON_GetObjectItemCaseSensitive(root, "cron_jobs");

    if (cJSON_IsArray(json_cron_jobs)) {
        ESP_ERROR_CHECK(config_parse_cron_jobs(json_cron_jobs));
    }

    if (cJSON_IsObject(json_mqtt)) {
        ESP_ERROR_CHECK(config_parse_mqtt(json_mqtt));
    }

    cJSON_Delete(root);
    return ESP_OK;
}

esp_err_t config_parse_mqtt(const cJSON *json_mqtt) {
    // Parse MQTT
    if (json_mqtt != NULL) {
        config_mqtt = malloc(sizeof(config_mqtt_t));

        if (cJSON_GetObjectItem(json_mqtt, "enabled")->valueint == 1) {
            config_mqtt->enabled = true;
        } else { config_mqtt->enabled = false; }

        strcpy(config_mqtt->protocol,
               cJSON_GetObjectItem(json_mqtt, "protocol")->valuestring);
        strcpy(config_mqtt->ip,
               cJSON_GetObjectItem(json_mqtt, "ip")->valuestring);
        strcpy(config_mqtt->port,
               cJSON_GetObjectItem(json_mqtt, "port")->valuestring);
        strcpy(config_mqtt->username,
               cJSON_GetObjectItem(json_mqtt, "username")->valuestring);
        strcpy(config_mqtt->password,
               cJSON_GetObjectItem(json_mqtt, "password")->valuestring);

        return ESP_OK;
    }
    return ESP_FAIL;

}

esp_err_t config_parse_cron_jobs(const cJSON *json_cron_jobs) {
    // Parse CRON_JOBS
    const cJSON *json_cron_job;
    if (json_cron_jobs != NULL) {
        int i = 0;
        cJSON_ArrayForEach(json_cron_job, json_cron_jobs) {

            if (cJSON_GetObjectItem(json_cron_job, "enabled")->valueint == 1) {
                config_cron_jobs[i].enabled = true;
            } else { config_cron_jobs[i].enabled = false; }

            strcpy(config_cron_jobs[i].name,
                   cJSON_GetObjectItem(json_cron_job, "name")->valuestring);
            strcpy(config_cron_jobs[i].description,
                   cJSON_GetObjectItem(json_cron_job, "description")->valuestring);
            strcpy(config_cron_jobs[i].schedule,
                   cJSON_GetObjectItem(json_cron_job, "schedule")->valuestring);
            i++;
        }

        return ESP_OK;
    }

    return ESP_FAIL;

}

void print_config_mqtt() {
    if (config_mqtt != NULL) {
        ESP_LOGI(FS_CONFIG_TAG, "MQTT: enabled='%i' \turl='%s://%s:%s' \tusername='%s' \tpassword='[hidden]'",
                 config_mqtt->enabled,
                 config_mqtt->protocol,
                 config_mqtt->ip,
                 config_mqtt->port,
                 config_mqtt->username);
    }

}

void print_config_cron_jobs() {
    for (int i = 0; i < CRON_JOBS_LENGTH; i++) {
        ESP_LOGI(FS_CONFIG_TAG, "CRON: enabled='%i' \tname='%s' \tschedule='%s'",
                 config_cron_jobs[i].enabled,
                 config_cron_jobs[i].name,
                 config_cron_jobs[i].schedule);
    }
}

void config_read_file() {
    ESP_LOGI(FS_CONFIG_TAG, "Loading file: %s", FILE_SYSTEM_CONFIG);
    char filepath[ESP_VFS_PATH_MAX + 128];
    strlcpy(filepath, CONFIG_EXAMPLE_WEB_MOUNT_POINT, sizeof(filepath));
    strlcat(filepath, FILE_SYSTEM_CONFIG, sizeof(filepath));

    char buffer[SCRATCH_BUFFER_SIZE_MAX + 1];
    file_handler_read_file(filepath, buffer, SCRATCH_BUFFER_SIZE_MAX);
    config_parse_file(buffer);

    print_config_mqtt();
    print_config_cron_jobs();


}

