#ifndef REST_GET_HANDLER_H
#define REST_GET_HANDLER_H

#include "esp_flash.h"
#include "esp_wifi.h"
#include "rest_types.h"
#include "cJSON.h"

/**
 * GET-Request
 * -> '/system'
 */
static esp_err_t rest_get_system_handler(httpd_req_t *req)
{

    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    uint32_t flash_size;
    ESP_ERROR_CHECK(esp_flash_get_size(NULL, &flash_size));

    httpd_resp_set_type(req, "application/json");
    cJSON *root = cJSON_CreateObject();

    cJSON *info = cJSON_CreateObject();
    cJSON_AddStringToObject(info, "target", CONFIG_IDF_TARGET);
    cJSON_AddNumberToObject(info, "cores", chip_info.cores);
    cJSON_AddNumberToObject(info, "major_rev", chip_info.revision / 100);
    cJSON_AddNumberToObject(info, "minor_rev", chip_info.revision % 100);
    cJSON_AddNumberToObject(info, "flash_size_MB", flash_size / (uint32_t)(1024 * 1024));
    cJSON_AddNumberToObject(info, "free_heap", esp_get_minimum_free_heap_size());
    cJSON_AddNumberToObject(info, "uptime", xTaskGetTickCount() * portTICK_PERIOD_MS);
    cJSON_AddItemToObject(root, "info", info);

    const char *sys_info = cJSON_Print(root);
    httpd_resp_sendstr(req, sys_info);
    free((void *)sys_info);
    cJSON_Delete(root);
    return ESP_OK;
}

/**
 * GET-Request
 * -> '/wifi'
 */
static esp_err_t rest_get_wifi_handler(httpd_req_t *req)
{

    // rest_server_context_t *rest_context = (rest_server_context_t *)req->user_ctx;
    // esp_netif_t *netif_config = rest_context->netif_config;
    esp_netif_ip_info_t ip_info;
    wifi_ap_record_t wifi_info;

    char network_ip[16] = "0.0.0.0";
    char network_mask[16] = "1.1.1.1";
    char network_gateway[16] = "2.2.2.2";
    char network_ssid[33] = "none";

    // ESP_ERROR_CHECK(esp_netif_get_ip_info(netif_config, &ip_info));
    // ESP_ERROR_CHECK(esp_wifi_sta_get_ap_info(&wifi_info));
    // sprintf(network_ip, IPSTR, IP2STR(&ip_info.ip));
    // sprintf(network_mask, IPSTR, IP2STR(&ip_info.netmask));
    // sprintf(network_gateway, IPSTR, IP2STR(&ip_info.gw));
    // sprintf(&network_ssid, "%s", wifi_info.ssid);

    httpd_resp_set_type(req, "application/json");
    cJSON *root = cJSON_CreateObject();
    cJSON *status = cJSON_CreateObject();

    cJSON_AddStringToObject(status, "ip", network_ip);
    cJSON_AddStringToObject(status, "netmask", network_mask);
    cJSON_AddStringToObject(status, "gateway", network_gateway);
    cJSON_AddStringToObject(status, "ssid", network_ssid);
    // cJSON_AddNumberToObject(status, "rssi", wifi_info.rssi);
    cJSON_AddItemToObject(root, "status", status);

    wifi_scan_config_t scan_config = {
        .ssid = 0,
        .bssid = 0,
        .channel = 0, /* 0--all channel scan */
        .show_hidden = 1,
        .scan_type = WIFI_SCAN_TYPE_ACTIVE,
        .scan_time.active.min = 120,
        .scan_time.active.max = 150,
    };

    // ESP_ERROR_CHECK(esp_wifi_scan_start(&scan_config, true));

    // cJSON *networks = cJSON_CreateObject();
    // uint16_t ap_count = 0;
    // wifi_ap_record_t *ap_list;
    // ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&ap_count));

    // ap_list = (wifi_ap_record_t *)malloc(ap_count * sizeof(wifi_ap_record_t));
    // if (ap_count > 0)
    //     ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&ap_count, ap_list));

    // char ap_ssid[33] = "none";
    // cJSON *networks_array = cJSON_CreateArray();
    // for (int i = 0; i < ap_count; i++)
    // {
    //     snprintf(&ap_ssid, "%s", &ap_list[i].ssid);
    //     cJSON *network = cJSON_CreateObject();
    //     cJSON_AddStringToObject(network, "ssid", ap_ssid);
    //     cJSON_AddNumberToObject(network, "rssi", ap_list[i].rssi);
    //     cJSON_AddItemToArray(networks_array, network);
    // }
    // free(ap_list);
    // cJSON_AddItemToObject(root, "networks", networks_array);

    const char *result = cJSON_Print(root);
    httpd_resp_sendstr(req, result);
    free((void *)result);
    cJSON_Delete(root);
    return ESP_OK;
}

/**
 * GET-Request
 * -> '/ap'
 */
static esp_err_t rest_get_ap_handler(httpd_req_t *req)
{
    if (wifi_manager_lock_json_buffer((TickType_t)10))
    {

        httpd_resp_set_status(req, http_200_hdr);
        httpd_resp_set_type(req, http_content_type_json);
        httpd_resp_set_hdr(req, http_cache_control_hdr, http_cache_control_no_cache);
        httpd_resp_set_hdr(req, http_pragma_hdr, http_pragma_no_cache);
        char *ap_buf = wifi_manager_get_ap_list_json();
        httpd_resp_send(req, ap_buf, strlen(ap_buf));
        wifi_manager_unlock_json_buffer();
    }
    else
    {
        httpd_resp_set_status(req, http_503_hdr);
        httpd_resp_send(req, NULL, 0);
        ESP_LOGE("TAG", "http_server_netconn_serve: GET /ap.json failed to obtain mutex");
    }

    /* request a wifi scan */
    wifi_manager_scan_async();
    return ESP_OK;
}

/**
 * GET-Request
 * -> '/sensors'
 */
static esp_err_t rest_get_sensors_handler(httpd_req_t *req)
{
    rest_server_context_t *rest_context = (rest_server_context_t *)req->user_ctx;
    smart_meter_sensor_t *sensors = rest_context->sensors;

    httpd_resp_set_type(req, "application/json");

    cJSON *sensors_array = cJSON_CreateArray();

    for (size_t i = 0; i < CONFIG_SMART_METER_SENSOR_LENGTH; i++)
    {
        cJSON *sensor_item = cJSON_CreateObject();
        cJSON_AddNumberToObject(sensor_item, "id", sensors[i].id);
        cJSON_AddStringToObject(sensor_item, "name", sensors[i].name);
        cJSON_AddNumberToObject(sensor_item, "count", sensors[i].count);
        cJSON_AddNumberToObject(sensor_item, "power", sensors[i].power);

        cJSON *history = cJSON_CreateObject();
        cJSON *history_day = cJSON_CreateArray();
        cJSON *history_week = cJSON_CreateArray();
        cJSON *history_month = cJSON_CreateArray();

        cJSON_AddArrayToObject(history_day, sensors[i].history.day_24_kw);
        cJSON_AddArrayToObject(history_week, sensors[i].history.week_7_kw);
        cJSON_AddArrayToObject(history_month, sensors[i].history.month_30_kw);

        cJSON_AddItemToObject(history, "day_24_kw", history_day);
        cJSON_AddItemToObject(history, "week_7_kw", history_week);
        cJSON_AddItemToObject(history, "month_30_kw", history_month);

        cJSON_AddItemToObject(sensor_item, "history", history);

        cJSON_AddItemToArray(sensors_array, sensor_item);
    }

    const char *response = cJSON_Print(sensors_array);
    httpd_resp_sendstr(req, response);
    free((void *)response);
    cJSON_Delete(sensors_array);
    return ESP_OK;
}

#endif