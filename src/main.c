#include <string.h>
#include "dhcp.h"
#include "esp_wifi.h"
#include "esp_wifi_types.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_netif.h"
#include "esp_http_server.h"
#include "wifi.h"
#include "uart.h"

static httpd_handle_t start_webserver(void); 
static esp_err_t toggle_led_handler(httpd_req_t *req);
static const httpd_uri_t toggle_led = {
    .uri       = "/toggle",
    .method    = HTTP_GET,
    .handler   = toggle_led_handler,
    .user_ctx  = NULL
};

static const char *TAG = "wifi softAP";

static esp_err_t toggle_led_handler(httpd_req_t *req) {
    const char resp[] = "LED Toggle Command Sent";
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

static httpd_handle_t start_webserver(void) {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    httpd_handle_t server = NULL;
    if (httpd_start(&server, &config) == ESP_OK) {
        ESP_LOGI(TAG, "Registering URI handlers");
        httpd_register_uri_handler(server, &toggle_led);
        return server;
    }

    ESP_LOGI(TAG, "Error starting server!");
    return NULL;
}

void app_main(void) {
    ESP_ERROR_CHECK(nvs_flash_init());
    init();
    wifi_init_softap();
    start_webserver();
}
