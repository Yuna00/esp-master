#include "dhcp.h"

void start_dhcp_server(esp_netif_t *netif_ap) {
    esp_netif_ip_info_t ip_info;

    // Define the static IP configuration
    IP4_ADDR(&ip_info.ip, 192, 168, 1, 1);  // IP
    IP4_ADDR(&ip_info.gw, 192, 168, 1, 1);  // Gateway
    IP4_ADDR(&ip_info.netmask, 255, 255, 255, 0);  // Netmask

    // Apply the static IP configuration
    esp_netif_dhcps_stop(netif_ap);
    esp_netif_set_ip_info(netif_ap, &ip_info);
    esp_netif_dhcps_start(netif_ap);
}
