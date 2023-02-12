//
// Implementation for File handling
//
#include <esp_log.h>
#include <cJSON.h>
#include "file_hanlder.h"

#define NUM_ALPHA  26

static const char *FILE_HANDLER_TAG = "file-handler";

esp_err_t file_handler_read_file(char *path, char *buffer, size_t buffer_size) {
    ESP_LOGI(FILE_HANDLER_TAG, "[read]: %s", path);
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        ESP_LOGE(FILE_HANDLER_TAG, "Failed to open file for reading: %s", path);
    } else {
        ESP_LOGI(FILE_HANDLER_TAG, "Opened storage file %s", path);


        buffer[buffer_size] = '\0';

        size_t num = fread(buffer, sizeof(char), buffer_size, f);
        if (num == NUM_ALPHA) {  /* fread success */
            ESP_LOGI(FILE_HANDLER_TAG, "Number of characters read = %i", num);
            fclose(f);
        } else {  /* fread failed */
            if (ferror(f))         /* possibility 1 */
            {
                ESP_LOGE(FILE_HANDLER_TAG, "Error reading %s", path);
                return ESP_FAIL;
            } else if (feof(f)) {     /* possibility 2 */
                ESP_LOGI(FILE_HANDLER_TAG, "EOF found");
                ESP_LOGI(FILE_HANDLER_TAG, "Number of characters read %d", num);

            }
        }
    }
    return ESP_OK;

}

esp_err_t file_handler_write_file(char *path, cJSON *json_object) {
    ESP_LOGI(FILE_HANDLER_TAG, "[write]: %s", path);

    FILE *f = fopen(path, "w");
    if (f == NULL) {
        ESP_LOGE(FILE_HANDLER_TAG, "Failed to open file for writing: %s", path);
    } else {
        fprintf(f, "%s", cJSON_Print(json_object));
        fclose(f);
        ESP_LOGI(FILE_HANDLER_TAG, "Written storage file %s", path);

    }
    return ESP_OK;
}
