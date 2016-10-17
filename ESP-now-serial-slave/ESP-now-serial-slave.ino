#include <SoftwareSerial.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
extern "C" {
#include <espnow.h>
#include <user_interface.h>
}

//SoftwareSerial mySerial (rxPin, txPin);
SoftwareSerial swSer(14, 12);

#define WIFI_DEFAULT_CHANNEL 9

// esp dev - esp1[] = {0x1A, 0xFE, 0x34, 0xDB, 0x3C, 0x64};
uint8_t esp1[] = {0x1A, 0xFE, 0x34, 0xDB, 0x3C, 0x64};
uint8_t esp2[] = {0x1A, 0xFE, 0x34, 0xDA, 0xF1, 0xB8}; // esp number 2 - slave
uint8_t esp3[] = {0x1A, 0xFE, 0x34, 0xEE, 0xCD, 0x15}; // esp number 3 - slave
uint8_t esp4[] = {0x5E, 0xCF, 0x7F, 0x9, 0x9A, 0xFE}; // esp number 4 - slave
uint8_t espdev[] = {0x1A, 0xFE, 0x34, 0xDB, 0x41, 0x41}; // esp - dev


void printMacAddress(uint8_t* macaddr) {
  Serial.print("{");
  for (int i = 0; i < 6; i++) {
    Serial.print("0x");
    Serial.print(macaddr[i], HEX);
    if (i < 5) Serial.print(', ');
  }
  Serial.println("}");
}

void setup() {
  WiFi.disconnect();

  Serial.begin(115200);
  swSer.begin(9600);

  Serial.println("\nSoftware serial slave");

  WiFi.mode(WIFI_STA);

  uint8_t macaddr[6];
  wifi_get_macaddr(STATION_IF, macaddr);
  Serial.print("mac address (STATION_IF): ");
  printMacAddress(macaddr);

  wifi_get_macaddr(SOFTAP_IF, macaddr);
  Serial.print("mac address (SOFTAP_IF): ");
  printMacAddress(macaddr);

  if (esp_now_init() == 0) {
    Serial.println("direct link  init ok");
  } else {
    Serial.println("dl init failed");
    ESP.restart();
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_recv_cb([](uint8_t *macaddr, uint8_t *data, uint8_t len) {
    Serial.println("recv_cb");

    Serial.print("mac address: ");
    printMacAddress(macaddr);

    Serial.print("data: ");
    for (int i = 0; i < len; i++) {
      Serial.print(data[i], HEX);
    }
    Serial.print("  ");
  });

  int res1 = esp_now_add_peer(esp1, (uint8_t)ESP_NOW_ROLE_SLAVE, (uint8_t)WIFI_DEFAULT_CHANNEL, NULL, 0);
  int res2 = esp_now_add_peer(esp2, (uint8_t)ESP_NOW_ROLE_SLAVE, (uint8_t)WIFI_DEFAULT_CHANNEL, NULL, 0);
  int res3 = esp_now_add_peer(esp3, (uint8_t)ESP_NOW_ROLE_SLAVE, (uint8_t)WIFI_DEFAULT_CHANNEL, NULL, 0);
  int res4 = esp_now_add_peer(esp4, (uint8_t)ESP_NOW_ROLE_SLAVE, (uint8_t)WIFI_DEFAULT_CHANNEL, NULL, 0);
  int res5 = esp_now_add_peer(espdev, (uint8_t)ESP_NOW_ROLE_SLAVE, (uint8_t)WIFI_DEFAULT_CHANNEL, NULL, 0);
}

uint8_t data[] = {};
uint8_t len;

void loop() {

  while (swSer.available() > 0) {
    data[0] = swSer.read();
    Serial.write(data[0]);
    Serial.println(" ");
    len = 1;
    esp_now_send(NULL, data, len);
  }

}
