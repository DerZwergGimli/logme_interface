#ifndef WSS_HANDLER_H
#define WSS_HANDLER_H

#include <esp_event.h>
#include "esp_log.h"
#include <esp_system.h>
#include <nvs_flash.h>
#include <sys/param.h>
#include "esp_netif.h"
#include "esp_eth.h"
#include <esp_http_server.h>

static const char *WS_SERVER = "WS_Server";
static const size_t max_clients = 4;

struct async_resp_arg
{
    httpd_handle_t hd;
    int fd;
    smart_meter_sensor_t *sensors;
};

static void ws_async_send(void *arg)
{
    static const char *data = "Async data";
    struct async_resp_arg *resp_arg = arg;
    httpd_handle_t hd = resp_arg->hd;
    int fd = resp_arg->fd;
    httpd_ws_frame_t ws_pkt;
    memset(&ws_pkt, 0, sizeof(httpd_ws_frame_t));
    ws_pkt.payload = (uint8_t *)data;
    ws_pkt.len = strlen(data);
    ws_pkt.type = HTTPD_WS_TYPE_TEXT;

    httpd_ws_send_frame_async(hd, fd, &ws_pkt);
    free(resp_arg);
}

static esp_err_t trigger_async_send(httpd_handle_t handle, httpd_req_t *req)
{
    struct async_resp_arg *resp_arg = malloc(sizeof(struct async_resp_arg));
    resp_arg->hd = req->handle;
    resp_arg->fd = httpd_req_to_sockfd(req);
    return httpd_queue_work(handle, ws_async_send, resp_arg);
}

void websocket_task(void *req)
{

    for (;;)
    {

        httpd_ws_send_frame(req, "hello");

        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }

    vTaskDelete(NULL);
}

static esp_err_t wss_subscribe_handler(httpd_req_t *req)
{
    ESP_LOGI(WS_SERVER, "Subtribe, the new connection was opened");
    return ESP_OK;
}

/*
 * This handler echos back the received ws data
 * and triggers an async send if certain message received
 */
static esp_err_t echo_handler(httpd_req_t *req)
{

    if (req->method == HTTP_GET)
    {
        ESP_LOGI(WS_SERVER, "Handshake done, the new connection was opened");
        return ESP_OK;
    }

    httpd_ws_frame_t ws_pkt;
    uint8_t *buf = NULL;
    memset(&ws_pkt, 0, sizeof(httpd_ws_frame_t));
    ws_pkt.type = HTTPD_WS_TYPE_TEXT;
    /* Set max_len = 0 to get the frame len */
    esp_err_t ret = httpd_ws_recv_frame(req, &ws_pkt, 0);
    if (ret != ESP_OK)
    {
        ESP_LOGE(WS_SERVER, "httpd_ws_recv_frame failed to get frame len with %d", ret);
        return ret;
    }
    ESP_LOGI(WS_SERVER, "frame len is %d", ws_pkt.len);
    if (ws_pkt.len)
    {
        /* ws_pkt.len + 1 is for NULL termination as we are expecting a string */
        buf = calloc(1, ws_pkt.len + 1);
        if (buf == NULL)
        {
            ESP_LOGE(WS_SERVER, "Failed to calloc memory for buf");
            return ESP_ERR_NO_MEM;
        }
        ws_pkt.payload = buf;
        /* Set max_len = ws_pkt.len to get the frame payload */
        ret = httpd_ws_recv_frame(req, &ws_pkt, ws_pkt.len);
        if (ret != ESP_OK)
        {
            ESP_LOGE(WS_SERVER, "httpd_ws_recv_frame failed with %d", ret);
            free(buf);
            return ret;
        }
        ESP_LOGI(WS_SERVER, "Got packet with message: %s", ws_pkt.payload);
    }
    ESP_LOGI(WS_SERVER, "Packet type: %d", ws_pkt.type);
    if (ws_pkt.type == HTTPD_WS_TYPE_TEXT &&
        strcmp((char *)ws_pkt.payload, "Trigger async") == 0)
    {
        free(buf);
        return trigger_async_send(req->handle, req);
    }

    ret = httpd_ws_send_frame(req, &ws_pkt);
    if (ret != ESP_OK)
    {
        ESP_LOGE(WS_SERVER, "httpd_ws_send_frame failed with %d", ret);
    }
    free(buf);
    return ret;
}

static void send_hello(void *arg)
{
    static const char *data = "Hello client";
    struct async_resp_arg *resp_arg = arg;
    httpd_handle_t hd = resp_arg->hd;
    int fd = resp_arg->fd;
    httpd_ws_frame_t ws_pkt;
    memset(&ws_pkt, 0, sizeof(httpd_ws_frame_t));
    ws_pkt.payload = (uint8_t *)data;
    ws_pkt.len = strlen(data);
    ws_pkt.type = HTTPD_WS_TYPE_TEXT;

    httpd_ws_send_frame_async(hd, fd, &ws_pkt);
    free(resp_arg);
}

// Get all clients and send async message
static void wss_server_send_messages(httpd_handle_t *server, smart_meter_sensor_t *sensors)
{
    bool send_messages = true;

    // Send async message to all connected clients that use websocket protocol every 10 seconds
    while (send_messages)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        if (!*server)
        { // httpd might not have been created by now
            continue;
        }
        size_t clients = max_clients;
        int client_fds[max_clients];
        if (httpd_get_client_list(*server, &clients, client_fds) == ESP_OK)
        {
            for (size_t i = 0; i < clients; ++i)
            {
                int sock = client_fds[i];
                if (httpd_ws_get_fd_info(*server, sock) == HTTPD_WS_CLIENT_WEBSOCKET)
                {
                    ESP_LOGI(WS_SERVER, "Active client (fd=%d) -> sending async message", sock);
                    struct async_resp_arg *resp_arg = malloc(sizeof(struct async_resp_arg));
                    resp_arg->hd = *server;
                    resp_arg->fd = sock;
                    resp_arg->sensors = sensors;
                    if (httpd_queue_work(resp_arg->hd, send_hello, resp_arg) != ESP_OK)
                    {
                        ESP_LOGE(WS_SERVER, "httpd_queue_work failed!");
                        send_messages = false;
                        break;
                    }
                }
            }
        }
        else
        {
            ESP_LOGE(WS_SERVER, "httpd_get_client_list failed!");
            return;
        }
    }
}

#endif