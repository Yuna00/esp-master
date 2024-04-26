#include <string.h>
#include "esp_wifi.h"
#include "esp_wifi_types.h"
#include "esp_event.h"
#include "esp_err.h"
#include "esp_log.h"
#include "nvs_flash.h"

#define WIFI_SSID      "ESP32-Server"
#define WIFI_PASS      "12345678"
#define MAX_STA_CONNECTIONS       2

static const char *TAG;

void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);

void wifi_init_softap(void);