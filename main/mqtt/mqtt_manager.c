//
//
//


#include "mqtt_manager.h"


void log_error_if_nonzero(const char *message, int error_code) {
    if (error_code != 0) {
        ESP_LOGE(MQTT_MANAGER, "Last error %s: 0x%x", message, error_code);
    }
}


void print_user_property(mqtt5_user_property_handle_t user_property) {
    if (user_property) {
        uint8_t count = esp_mqtt5_client_get_user_property_count(user_property);
        if (count) {
            esp_mqtt5_user_property_item_t *item = malloc(count * sizeof(esp_mqtt5_user_property_item_t));
            if (esp_mqtt5_client_get_user_property(user_property, item, &count) == ESP_OK) {
                for (int i = 0; i < count; i++) {
                    esp_mqtt5_user_property_item_t *t = &item[i];
                    ESP_LOGI(MQTT_MANAGER, "key is %s, value is %s", t->key, t->value);
                    free((char *) t->key);
                    free((char *) t->value);
                }
            }
            free(item);
        }
    }
}

/*
 * @brief Event handler registered to receive MQTT events
 *
 *  This function is called by the MQTT client event loop.
 *
 * @param handler_args user data registered to the event.
 * @param base Event base for the handler(always MQTT Base in this example).
 * @param event_id The id for the received event.
 * @param event_data The data for the event, esp_mqtt_event_handle_t.
 */
void mqtt5_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    ESP_LOGD(MQTT_MANAGER, "Event dispatched from event loop base=%s, event_id=%ld", base, event_id);
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;

    ESP_LOGD(MQTT_MANAGER, "free heap size is %lu, maxminu %lu", esp_get_free_heap_size(),
             esp_get_minimum_free_heap_size());
    switch ((esp_mqtt_event_id_t) event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(MQTT_MANAGER, "MQTT_EVENT_CONNECTED");
            print_user_property(event->property->user_property);
            esp_mqtt5_client_set_user_property(&publish_property.user_property, user_property_arr,
                                               USE_PROPERTY_ARR_SIZE);
            esp_mqtt5_client_set_publish_property(client, &publish_property);
            msg_id = esp_mqtt_client_publish(client, "/topic/qos1", "data_3", 0, 1, 1);
//            esp_mqtt5_client_delete_user_property(publish_property.user_property);
//            publish_property.user_property = NULL;
//            ESP_LOGI(MQTT_MANAGER, "sent publish successful, msg_id=%d", msg_id);
//
//            esp_mqtt5_client_set_user_property(&subscribe_property.user_property, user_property_arr,
//                                               USE_PROPERTY_ARR_SIZE);
//            esp_mqtt5_client_set_subscribe_property(client, &subscribe_property);
//            msg_id = esp_mqtt_client_subscribe(client, "/topic/qos0", 0);
//            esp_mqtt5_client_delete_user_property(subscribe_property.user_property);
//            subscribe_property.user_property = NULL;
//            ESP_LOGI(MQTT_MANAGER, "sent subscribe successful, msg_id=%d", msg_id);
//
//            esp_mqtt5_client_set_user_property(&subscribe1_property.user_property, user_property_arr,
//                                               USE_PROPERTY_ARR_SIZE);
//            esp_mqtt5_client_set_subscribe_property(client, &subscribe1_property);
//            msg_id = esp_mqtt_client_subscribe(client, "/topic/qos1", 2);
//            esp_mqtt5_client_delete_user_property(subscribe1_property.user_property);
//            subscribe1_property.user_property = NULL;
//            ESP_LOGI(MQTT_MANAGER, "sent subscribe successful, msg_id=%d", msg_id);
//
//            esp_mqtt5_client_set_user_property(&unsubscribe_property.user_property, user_property_arr,
//                                               USE_PROPERTY_ARR_SIZE);
//            esp_mqtt5_client_set_unsubscribe_property(client, &unsubscribe_property);
//            msg_id = esp_mqtt_client_unsubscribe(client, "/topic/qos0");
//            ESP_LOGI(MQTT_MANAGER, "sent unsubscribe successful, msg_id=%d", msg_id);
//            esp_mqtt5_client_delete_user_property(unsubscribe_property.user_property);
//            unsubscribe_property.user_property = NULL;
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(MQTT_MANAGER, "MQTT_EVENT_DISCONNECTED");
            print_user_property(event->property->user_property);
            break;
        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(MQTT_MANAGER, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            print_user_property(event->property->user_property);
            esp_mqtt5_client_set_publish_property(client, &publish_property);
            msg_id = esp_mqtt_client_publish(client, "/topic/qos0", "data", 0, 0, 0);
            ESP_LOGI(MQTT_MANAGER, "sent publish successful, msg_id=%d", msg_id);
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(MQTT_MANAGER, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            print_user_property(event->property->user_property);
            esp_mqtt5_client_set_user_property(&disconnect_property.user_property, user_property_arr,
                                               USE_PROPERTY_ARR_SIZE);
            esp_mqtt5_client_set_disconnect_property(client, &disconnect_property);
            esp_mqtt5_client_delete_user_property(disconnect_property.user_property);
            disconnect_property.user_property = NULL;
            esp_mqtt_client_disconnect(client);
            break;
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(MQTT_MANAGER, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            print_user_property(event->property->user_property);
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(MQTT_MANAGER, "MQTT_EVENT_DATA");
            print_user_property(event->property->user_property);
            ESP_LOGI(MQTT_MANAGER, "payload_format_indicator is %d", event->property->payload_format_indicator);
            ESP_LOGI(MQTT_MANAGER, "response_topic is %.*s", event->property->response_topic_len,
                     event->property->response_topic);
            ESP_LOGI(MQTT_MANAGER, "correlation_data is %.*s", event->property->correlation_data_len,
                     event->property->correlation_data);
            ESP_LOGI(MQTT_MANAGER, "content_type is %.*s", event->property->content_type_len,
                     event->property->content_type);
            ESP_LOGI(MQTT_MANAGER, "TOPIC=%.*s", event->topic_len, event->topic);
            ESP_LOGI(MQTT_MANAGER, "DATA=%.*s", event->data_len, event->data);
            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGI(MQTT_MANAGER, "MQTT_EVENT_ERROR");
            print_user_property(event->property->user_property);
            ESP_LOGI(MQTT_MANAGER, "MQTT5 return code is %d", event->error_handle->connect_return_code);
            if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
                log_error_if_nonzero("reported from esp-tls", event->error_handle->esp_tls_last_esp_err);
                log_error_if_nonzero("reported from tls stack", event->error_handle->esp_tls_stack_err);
                log_error_if_nonzero("captured as transport's socket errno",
                                     event->error_handle->esp_transport_sock_errno);
                ESP_LOGI(MQTT_MANAGER, "Last errno string (%s)",
                         strerror(event->error_handle->esp_transport_sock_errno));
            }
            break;
        default:
            ESP_LOGI(MQTT_MANAGER, "Other event id:%d", event->event_id);
            break;
    }
}

void mqtt5_app_start(void) {
    esp_mqtt5_connection_property_config_t connect_property = {
            .session_expiry_interval = 10,
            .maximum_packet_size = 1024,
            .receive_maximum = 65535,
            .topic_alias_maximum = 2,
            .request_resp_info = true,
            .request_problem_info = true,
            .will_delay_interval = 10,
            .payload_format_indicator = true,
            .message_expiry_interval = 10,
            .response_topic = "/test/response",
            .correlation_data = "123456",
            .correlation_data_len = 6,
    };

    esp_mqtt_client_config_t mqtt5_cfg = {
            .broker.address.uri = "mqtt://192.168.1.124:1883",
            .session.protocol_ver = MQTT_PROTOCOL_V_5,
            .network.disable_auto_reconnect = true,

            //.credentials.username = "123",
            //.credentials.authentication.password = "456",
            .session.last_will.topic = "/topic/will",
            .session.last_will.msg = "i will leave",
            .session.last_will.msg_len = 12,
            .session.last_will.qos = 1,
            .session.last_will.retain = true,
    };

    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt5_cfg);

    /* Set connection properties and user properties */
    esp_mqtt5_client_set_user_property(&connect_property.user_property, user_property_arr, USE_PROPERTY_ARR_SIZE);
    esp_mqtt5_client_set_user_property(&connect_property.will_user_property, user_property_arr, USE_PROPERTY_ARR_SIZE);
    esp_mqtt5_client_set_connect_property(client, &connect_property);

    /* If you call esp_mqtt5_client_set_user_property to set user properties, DO NOT forget to delete them.
     * esp_mqtt5_client_set_connect_property will malloc buffer to store the user_property and you can delete it after
     */
    esp_mqtt5_client_delete_user_property(connect_property.user_property);
    esp_mqtt5_client_delete_user_property(connect_property.will_user_property);

    /* The last argument may be used to pass data to the event handler, in this example mqtt_event_handler */
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt5_event_handler, NULL);
    esp_mqtt_client_start(client);
}