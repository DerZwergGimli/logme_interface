//
// .
//

#ifndef LOGME_INTERFACE_MQTT_MANAGER_H
#define LOGME_INTERFACE_MQTT_MANAGER_H

#include "mqtt_client.h"
#include "esp_log.h"
#include "esp_event.h"

static const char *MQTT_MANAGER = "mqtt5_manager";
static esp_mqtt5_user_property_item_t user_property_arr[] = {
        {"board", "esp32"},
        {"u",     "user"},
        {"p",     "password"}
};

#define USE_PROPERTY_ARR_SIZE   sizeof(user_property_arr)/sizeof(esp_mqtt5_user_property_item_t)

static esp_mqtt5_publish_property_config_t publish_property = {
        .payload_format_indicator = 1,
        .message_expiry_interval = 1000,
        .topic_alias = 0,
        .response_topic = "/topic/test/response",
        .correlation_data = "123456",
        .correlation_data_len = 6,
};

static esp_mqtt5_subscribe_property_config_t subscribe_property = {
        .subscribe_id = 25555,
        .no_local_flag = false,
        .retain_as_published_flag = false,
        .retain_handle = 0,
        .is_share_subscribe = true,
        .share_name = "group1",
};

static esp_mqtt5_subscribe_property_config_t subscribe1_property = {
        .subscribe_id = 25555,
        .no_local_flag = true,
        .retain_as_published_flag = false,
        .retain_handle = 0,
};

static esp_mqtt5_unsubscribe_property_config_t unsubscribe_property = {
        .is_share_subscribe = true,
        .share_name = "group1",
};

static esp_mqtt5_disconnect_property_config_t disconnect_property = {
        .session_expiry_interval = 60,
        .disconnect_reason = 0,
};

void log_error_if_nonzero(const char *message, int error_code);

void print_user_property(mqtt5_user_property_handle_t user_property);

void mqtt5_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);

void start_mqtt5_app(const char *url, const char *username, const char *password);

typedef struct mqtt_message_t {
    char topic[50];
    char message[100];
} mqtt_message_t;

void send_message_async(mqtt_message_t mqtt_message);

#endif //LOGME_INTERFACE_MQTT_MANAGER_H
