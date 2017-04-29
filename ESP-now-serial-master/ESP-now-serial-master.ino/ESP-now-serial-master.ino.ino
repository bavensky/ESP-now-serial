#include <Arduino.h>
#include <MqttConnector.h>
#include <ArduinoJson.h>
#include <CMMC_Manager.h>
#include "CMMC_Interval.hpp"
#include <DHT.h>
#include <Ticker.h>
#include <SoftwareSerial.h>
//SoftwareSerial Serial (rxPin, txPin);
SoftwareSerial swSer(14, 15);

#define DHTPIN 12
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define RELAY 13
#define ECHO 4
#define TRIG 5
long duration, distance, countcome;

Ticker ticker;
CMMC_Interval timer001;

MqttConnector *mqtt;

#define MQTT_HOST         "mqtt.espert.io"
#define MQTT_PORT         1883
#define MQTT_USERNAME     ""
#define MQTT_PASSWORD     ""
#define MQTT_CLIENT_ID    ""
#define MQTT_PREFIX       "/CMMC"
#define PUBLISH_EVERY     (5000)// every 5 seconds

/* DEVICE DATA & FREQUENCY */
#define DEVICE_NAME       "CMMC-REMOTE-DOOR"

#include "_publish.h"
#include "_receive.h"
#include "init_mqtt.h"

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
  dht.begin();
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, HIGH);
  Serial.println();
  Serial.println("Serial port initialized.");
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
//  if(Serial.available() > 0) {
//    swSer.write(Serial.read());
//    Serial.println(Serial.read());
//  }
  
  mqtt->loop();
  timer001.every_ms(1000, r_ultra);
}



