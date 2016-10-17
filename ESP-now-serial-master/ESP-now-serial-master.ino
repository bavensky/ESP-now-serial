#include <SoftwareSerial.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
extern "C" {
#include <espnow.h>
#include <user_interface.h>
}

#define WIFI_DEFAULT_CHANNEL 9
uint8_t mac[] = {0x1A, 0xFE, 0x34, 0xEE, 0xC8, 0x99}; // esp number 2 - master

//SoftwareSerial mySerial (rxPin, txPin);

#define CMMC_DEVICE_ID   1
#define CMMC_DEVICE_MASK 0b00000010


SoftwareSerial swSer(14, 12);

#define button0 0
#define button13 13
#define LED 2

void printMacAddress(uint8_t* macaddr) {
  Serial.print("{");
  for (int i = 0; i < 6; i++) {
    Serial.print("0x");
    Serial.print(macaddr[i], HEX);
    if (i < 5) Serial.print(',');
  }
  Serial.println("}");
}

void setup() {
  WiFi.disconnect();

  Serial.begin(115200);
  swSer.begin(9600);

  Serial.println("\nSoftware serial master");

  pinMode(button0, INPUT);
  pinMode(button13, INPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);

  WiFi.mode(WIFI_AP_STA);

  uint8_t macaddr[6];
  wifi_get_macaddr(SOFTAP_IF, macaddr);
  Serial.print("mac address (SOFTAP_IF): ");
  printMacAddress(macaddr);

  if (esp_now_init() == 0) {
    Serial.println("init");
  } else {
    Serial.println("init failed");
    ESP.restart();
    return;
  }

  Serial.println("SET ROLE SLAVE");
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb([](uint8_t *macaddr, uint8_t *data, uint8_t len) {

    Serial.print(data[0], BIN);
    Serial.println(" ");
 
    Serial.println( !(data[0] & CMMC_DEVICE_MASK) );
    digitalWrite(LED, data[0] & CMMC_DEVICE_MASK);
    /*
      if((data[0] & 0B1111111) == 0B1101000)  {
      Serial.print(" Done ");
      }
    */
    Serial.println(" ");
    delay(500);
  });

  esp_now_register_send_cb([](uint8_t* macaddr, uint8_t status) {
    Serial.println("send_cb");

    Serial.print("mac address: ");
    printMacAddress(macaddr);

    Serial.print("status = "); Serial.println(status);
  });

  int res = esp_now_add_peer(mac, (uint8_t)ESP_NOW_ROLE_CONTROLLER, (uint8_t)WIFI_DEFAULT_CHANNEL, NULL, 0);

}

void loop() {
  while (Serial.available() > 0)  {
    swSer.write(Serial.read());
  }

  yield();

  //  if (digitalRead(button0) == 0)  {
  //    delay(200);
  //    byte h = 0b01101000;
  //    swSer.write(h);
  //    swSer.println("");
  //  }
  //  if (digitalRead(button13) == 0)  {
  //    delay(200);
  //    byte i = 0b01101001;
  //    swSer.write(i);
  //    swSer.println("");
  //  }

}
