//
// Interface for wss_manager
//

#ifndef WSS_MANAGER_H
#define WSS_MANAGER_H

#include <esp_event.h>
#include "esp_log.h"
#include <esp_system.h>
#include <nvs_flash.h>
#include <sys/param.h>
#include "esp_netif.h"
#include "esp_eth.h"
#include <esp_http_server.h>

//ACTIONS
typedef enum wss_manager_message_t {
    WSSM_SEND_HELLO = 0,
    WSSM_SEND_SYSTEM_INFO = 1,
    WSSM_SEND_SYSTEM_SENSORS = 2,
} wss_manager_message_t;

typedef struct {
    wss_manager_message_t code;
} queue_wss_manager;

//init/start
esp_err_t wss_subscribe_handler(httpd_req_t *req);

//run
void wss_manager(void *pvParamters);


//destroy
void wss_manager_destroy();


void send_hello(void *arg);


void wss_server_send_messages(httpd_handle_t *server);

void ws_async_send(void *arg);

esp_err_t trigger_async_send(httpd_handle_t handle, httpd_req_t *req);


esp_err_t wss_echo_handler(httpd_req_t *req);

void send_sensors(void *arg);

#endif //WSS_MANAGER_H
