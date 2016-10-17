#include <SoftwareSerial.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
extern "C" {
#include <espnow.h>
#include <user_interface.h>
}

//SoftwareSerial mySerial (rxPin, txPin);
SoftwareSerial swSer(14, 12);

#define WIFI_DEFAULT_CHANNEL 8
uint8_t esp2[] = {0x1A, 0xFE, 0x34, 0xDB, 0x3C, 0x64}; // esp number 2 - slave

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

  esp_now_register_send_cb([](uint8_t* macaddr, uint8_t status) {
    Serial.print("send_cb");
    Serial.print("mac address: ");
    printMacAddress(macaddr);
    Serial.print(" status = "); Serial.println(status);
  });

  int res2 = esp_now_add_peer(esp2, (uint8_t)ESP_NOW_ROLE_SLAVE, (uint8_t)WIFI_DEFAULT_CHANNEL, NULL, 0);

}

uint8_t b[] = {};

void loop() {

  while (swSer.available() > 0) {
    b[0] = swSer.read();
    Serial.write(b[0]);
    Serial.println(" ");
    esp_now_send(NULL, &b[0], 1);
  }

}
