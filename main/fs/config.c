//
// Implementation for loading/saving the config
//

#include "config.h"

#define SCRATCH_BUFFER_SIZE_MAX (1024)
#define CRON_JOBS_LENGTH 2
static const char *FS_CONFIG_TAG = "fs-config";

config_mqtt_t *config_mqtt;
config_cron_jobs_t config_cron_jobs[CRON_JOBS_LENGTH];


config_mqtt_t *config_get_mqtt() {
    return config_mqtt;
}


config_cron_jobs_t *config_get_cron_jobs() {
    return config_cron_jobs;
}

esp_err_t config_parse_file(char *json_data) {
    cJSON *root = cJSON_Parse(json_data);

    const cJSON *json_cron_jobs;
    const cJSON *json_cron_job;
    const cJSON *json_mqtt;

    if (root == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            ESP_LOGE("SML-Parser", "Error before: %s\n", error_ptr);
        }
        cJSON_Delete(root);
        return ESP_FAIL;
    }

    json_cron_jobs = cJSON_GetObjectItemCaseSensitive(root, "cron_jobs");
    json_mqtt = cJSON_GetObjectItemCaseSensitive(root, "mqtt");

    // Parse MQTT
    if (json_mqtt != NULL) {
        config_mqtt = malloc(sizeof(config_mqtt_t));

        config_mqtt->enabled = cJSON_GetObjectItem(json_mqtt, "enabled")->valueint;
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
    }

    // Parse CRON_JOBS
    if (json_cron_jobs != NULL) {
        int i = 0;
        cJSON_ArrayForEach(json_cron_job, json_cron_jobs) {
            config_cron_jobs[i].enabled = cJSON_GetObjectItemCaseSensitive(json_cron_job, "enabled")->valueint;
            strcpy(config_cron_jobs[i].name,
                   cJSON_GetObjectItem(json_cron_job, "name")->valuestring);
            strcpy(config_cron_jobs[i].description,
                   cJSON_GetObjectItem(json_cron_job, "description")->valuestring);
            strcpy(config_cron_jobs[i].schedule,
                   cJSON_GetObjectItem(json_cron_job, "schedule")->valuestring);
            i++;
        }
    }
    cJSON_Delete(root);
    return ESP_OK;


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

void config_read_file(const char *filename) {
    ESP_LOGI(FS_CONFIG_TAG, "Loading file: %s", filename);
    char filepath[ESP_VFS_PATH_MAX + 128];
    strlcpy(filepath, CONFIG_EXAMPLE_WEB_MOUNT_POINT, sizeof(filepath));
    strlcat(filepath, filename, sizeof(filepath));
    int fd = open(filepath, O_RDONLY, 0);
    if (fd == -1) {
        ESP_LOGE(FS_CONFIG_TAG, "Failed to open file for reading: %s", filepath);
    } else {
        ESP_LOGI(FS_CONFIG_TAG, "Opened storage file");
    }

    char scratch[SCRATCH_BUFFER_SIZE_MAX];

    char *chunk = scratch;
    ssize_t read_bytes;
    do {
        /* Read file in chunks into the scratch buffer */
        read_bytes = read(fd, chunk, SCRATCH_BUFFER_SIZE_MAX);
        if (read_bytes == -1) {
            ESP_LOGE(FS_CONFIG_TAG, "Failed to read file : %s", filepath);
        } else if (read_bytes > 0) {
            ESP_LOGI(FS_CONFIG_TAG, "Loading json data...");
            config_parse_file(chunk);

        }
    } while (read_bytes > 0);
    /* Close file after sending complete */

    print_config_mqtt();
    print_config_cron_jobs();
}

