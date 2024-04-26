#include "dhcp.h"
#include "wifi.h"

static const char *TAG = "Wifi Setup";

void wifi_event_handler(void* arg, esp_event_base_t event_base,
                               int32_t event_id, void* event_data) {
    if (event_id == WIFI_EVENT_AP_STACONNECTED) {
        wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
        ESP_LOGI(TAG, "station %02x:%02x:%02x:%02x:%02x:%02x join, AID=%d", //manual implementation of the MAC2STR macro. just putting some addresses into char array
                 event->mac[0], event->mac[1], event->mac[2],
                 event->mac[3], event->mac[4], event->mac[5], event->aid);
    } else if (event_id == WIFI_EVENT_AP_STADISCONNECTED) {
        wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data;
        ESP_LOGI(TAG, "station %02x:%02x:%02x:%02x:%02x:%02x leave, AID=%d",
                 event->mac[0], event->mac[1], event->mac[2],
                 event->mac[3], event->mac[4], event->mac[5], event->aid);
    }
}

void wifi_init_softap(void) {
    esp_err_t ret = nvs_flash_init();       // NVS flash is required for wifi driver persistence. It is a storage partition.
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {   // if the nvs storage contains no free space or if a new nvs is found
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();                  // wipe the whole partition and check for errors.
    }
    ESP_ERROR_CHECK(ret);
    ESP_ERROR_CHECK(esp_netif_init());    // error checking netif_init because it is only meant to trigger once upon system start.
    ESP_ERROR_CHECK(esp_event_loop_create_default());    // the same goes for event_loop_create_default.

   
    esp_netif_t *netif_ap = esp_netif_create_default_wifi_ap(); // create default wifi AP
    assert(netif_ap != NULL); // we make sure the default wifi AP has been created 

    start_dhcp_server(netif_ap);   // starting the dhcp server with static ip integration

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();    // initialize wifi config with defaults so we only have to change the necessary parts.
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));     // check if the wifi has actually updated to the new config.          

    wifi_config_t wifi_config = {    // here we can put our settings like authmode, ssid and password, max connections etc.
        .ap = {
            .ssid = "ESP32 MASTER",
            .password = "123456789",
            .channel = 1,
            .max_connection = 4,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK
        },
    };
    if (strlen("WIFI_PASS") == 0) {      // if the stringlength of pass is zero
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;   // set the wifi to become an open network.
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));    // check if the wifi mode actually got set
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config));    // check once more if the config is updated.
    ESP_ERROR_CHECK(esp_wifi_start());    //    check if the wifi is running

    ESP_LOGI(TAG, "WiFi SoftAP initialized. SSID:%s", wifi_config.ap.ssid);
}

