#include <Arduino.h>

#include "esp_event.h"
#include "esp_log.h"
#include <esp_wifi.h>
#include <WiFi.h>

#include <esp_wifi.h>
#include <WiFi.h>

static const int CONNECTED_BIT = BIT0;
void init_wifi(wifi_mode_t mode)
{
    const uint8_t protocol = WIFI_PROTOCOL_LR;
    tcpip_adapter_init();
//    ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
    ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
    ESP_ERROR_CHECK( esp_wifi_set_mode(mode) );
    EventGroupHandle_t wifi_event_group = xEventGroupCreate();

    if (mode == WIFI_MODE_STA) {
        ESP_ERROR_CHECK( esp_wifi_set_protocol(WIFI_IF_STA, protocol) );
       wifi_config_t config = {
        .sta = {        //ERROR HERE
                .ssid = /*{[0]='K',[1]='h',[2]='a',[3]='t',[4]='r',[5]='i'}*/{'K','h','a','t','r','i'},
                .password = /*{[0]='s',[1]='a',[2]='m',[3]='e',[4]='e',[5]='p',[6]='1',[7]='8'}*/{'s','a','m','e','e','p','1','8'},
                .bssid_set = false
        }   
        };
        ESP_ERROR_CHECK( esp_wifi_set_config(WIFI_IF_STA, &config));
        ESP_ERROR_CHECK( esp_wifi_start() );
        ESP_ERROR_CHECK( esp_wifi_connect() );

        xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT,
                            false, true, portMAX_DELAY);
        ESP_LOGI(TAG, "Connected to AP");
    } else {
        ESP_ERROR_CHECK( esp_wifi_set_protocol(WIFI_IF_AP, protocol) );
        wifi_config_t config = {
          .ap = {                                   //ERROR HERE
                .ssid = /*{[0]='K',[1]='h',[2]='a',[3]='t',[4]='r',[5]='i'}*/{'K','h','a','t','r','i'},
                .password = /*{[0]='s',[1]='a',[2]='m',[3]='e',[4]='e',[5]='p',[6]='1',[7]='8'}*/{'s','a','m','e','e','p','1','8'},
                .ssid_len = 0,
                .authmode = WIFI_AUTH_WPA_WPA2_PSK,
                .ssid_hidden = false,
                .max_connection = 3,
                .beacon_interval = 100,
          }
        };
        ESP_ERROR_CHECK( esp_wifi_set_config(WIFI_IF_AP, &config));
        ESP_ERROR_CHECK( esp_wifi_start() );
    }
}
void setup()
{
 Serial.begin(115200);
 init_wifi(WIFI_MODE_STA);
}
void loop()
{
  if (WiFi.status()!=WL_CONNECTED)
    Serial.println("Not connected");
  else
  {
    Serial.println("Connected");
    Serial.println(WiFi.localIP());
  }
}