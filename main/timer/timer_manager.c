#include "timer_manager.h"

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>
#include <freertos/event_groups.h>
#include <string.h>
#include <esp_log.h>
#include <esp_chip_info.h>
#include <esp_timer.h>
#include <esp_flash.h>
#include <lwip/apps/sntp.h>
#include "cJSON.h"
#include "sensors/sensor_manager.h"

static const char *TIME_MANAGER_TAG = "time-manager";

QueueHandle_t time_manager_queue;
static EventGroupHandle_t time_manager_event_group;
static TaskHandle_t time_manager_task = NULL;


cron_job *jobs[4];


void time_manager(void *pvParameters) {
    queue_time_manager msg;
    BaseType_t xStatus;
    time_manager_send_message(TM_INIT, NULL);

    ESP_LOGI(TIME_MANAGER_TAG, "Creating Task...");
    for (;;) {
        xStatus = xQueueReceive(time_manager_queue, &msg, portMAX_DELAY);

        if (xStatus == pdPASS) {
            switch (msg.code) {
                case TM_NONE: {
                    ESP_LOGI(TIME_MANAGER_TAG, "TM_NONE");
                }
                    break;
                case TM_INIT: {
                    ESP_LOGI(TIME_MANAGER_TAG, "TM_INIT");
                    sntp_setoperatingmode(SNTP_OPMODE_POLL);
                    sntp_setservername(0, "pool.ntp.org");
                    sntp_init();
                    int _is_null = 0;
                    while (_is_null == 0) {
                        time_t now = time(NULL);
                        struct tm *time_struct = localtime(&now);
                        _is_null = time_struct->tm_min;
                        vTaskDelay(pdMS_TO_TICKS(1000));

                    }

                    time_manager_send_message(CREATE_TASKS, NULL);

                }
                    break;
                case CREATE_TASKS: {

                    jobs[0] = cron_job_create("0 * * * * *", cron_job_history_callback, (void *) SM_HISTORY_MINUTE);
                    jobs[1] = cron_job_create("0 0 * * * *", cron_job_history_callback, (void *) SM_HISTORY_HOUR);
                    jobs[2] = cron_job_create("0 0 0 * * *", cron_job_history_callback, (void *) SM_HISTORY_DAY);
                    jobs[3] = cron_job_create("* * * * * *", cron_job_history_callback, (void *) SM_HISTORY_SECOUND);

                    cron_start();
                    ESP_LOGI(TIME_MANAGER_TAG, "CREATE_TASKS");
                }
                    break;
                default: {
                    ESP_LOGI(TIME_MANAGER_TAG, "DEFAULT");
                    break;
                }
            }
        }

    }
    vTaskDelete(NULL);
}


void cron_job_history_callback(cron_job *job) {
    sensor_manager_history_t timeFrame = (sensor_manager_history_t) job->data;
    ESP_LOGI(TIME_MANAGER_TAG, "ITS ME %i", timeFrame);
}

int check_time(int time_value) {
    if (time_value == 0) {
        return true;
    } else
        return false;
}

void time_manager_start(bool log_enable) {
    if (!log_enable) {
        esp_log_level_set(TIME_MANAGER_TAG, ESP_LOG_NONE);
    }
    ESP_LOGI(TIME_MANAGER_TAG, "Starting up time_manager task...");
    time_manager_queue = xQueueCreate(3, sizeof(queue_time_manager));

    time_manager_event_group = xEventGroupCreate();

    xTaskCreate(&time_manager, "time_manager", 4096, NULL, 1, &time_manager_task);

}

void time_manager_destroy() {
    vTaskDelete(time_manager_task);
    time_manager_task = NULL;

    vEventGroupDelete(time_manager_event_group);
    time_manager_event_group = NULL;
};


BaseType_t time_manager_send_message(message_time_manager_code_t code, void *param) {
    queue_time_manager msg;
    msg.code = code;
    msg.param = param;
    return xQueueSend(time_manager_queue, &msg, portMAX_DELAY);
}
