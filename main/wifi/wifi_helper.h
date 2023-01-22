#ifndef WIFI_HELPER_H
#define WIFI_HELPER_H

static void initialize_wifi()
{
    ESP_ERROR_CHECK(esp_netif_init());
}
#endif