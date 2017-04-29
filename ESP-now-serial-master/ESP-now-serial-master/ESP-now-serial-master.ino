#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include "CMMC_Interval.hpp"
#include <MqttConnector.h>
#include <CMMC_Manager.h>
#include <Ticker.h>
#include <SoftwareSerial.h>
//SoftwareSerial mySerial (rxPin, txPin);
SoftwareSerial swSer(14, 15);

#define RELAY 13
#define ECHO 4
#define TRIG 5
long duration, distance, countcome;

Ticker ticker;
CMMC_Interval timer001;

#include "init_mqtt.h"
#include "_publish.h"
#include "_receive.h"

String MQTT_HOST        = "mqtt.cmmc.io";
String MQTT_USERNAME    = "";
String MQTT_PASSWORD    = "";
String MQTT_CLIENT_ID   = "";
String MQTT_PREFIX      = "CMMC";
int    MQTT_PORT        = 1883;

int PUBLISH_EVERY       = 5000;

MqttConnector *mqtt;

void r_ultra() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  duration = pulseIn(ECHO, HIGH);
  distance = (duration / 2) / 29.1;

  Serial.print("Distance = ");
  Serial.print(distance);
  Serial.println(" cm");
  delay(100);

  if (distance < 40) {
    countcome += 1;
  }
}

void init_hardware()
{
  Serial.begin(115200);
  swSer.begin(9600);
  delay(10);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  Serial.println();
  Serial.println("Starting...");
  pinMode(LED_BUILTIN, OUTPUT);
}

void setup()
{
  init_hardware();
  CMMC_Manager manager(0, LED_BUILTIN);
  manager.start();
  init_mqtt();
}

void loop()
{
  mqtt->loop();
  timer001.every_ms(1000, r_ultra);
}
