#include <MqttConnector.h>

extern MqttConnector* mqtt;

extern String MQTT_HOST;
extern String MQTT_USERNAME;
extern String MQTT_PASSWORD;
extern String MQTT_CLIENT_ID;
extern String MQTT_PREFIX;

extern int MQTT_PORT;
extern int PUBLISH_EVERY;

extern void register_publish_hooks();
extern void register_receive_hooks();

// MQTT INITIALIZER
void init_mqtt()
{
  mqtt = new MqttConnector(MQTT_HOST.c_str(), MQTT_PORT);

  mqtt->on_connecting([&](int counter, bool * flag) {
    Serial.printf("[%lu] MQTT CONNECTING.. \r\n", counter);
    // 5 minutes
    if (counter >= 600) {
      ESP.reset();
    }
    delay(1000);
  });

  mqtt->on_prepare_configuration([&](MqttConnector::Config * config) -> void {
    MQTT_CLIENT_ID = ESP.getChipId();
    config->clientId  = MQTT_CLIENT_ID;
    config->channelPrefix = MQTT_PREFIX;
    config->enableLastWill = true;
    config->retainPublishMessage = true;
    /*
        config->mode
        ===================
        | MODE_BOTH       |
        | MODE_PUB_ONLY   |
        | MODE_SUB_ONLY   |
        ===================
    */
    config->mode = MODE_BOTH;
    config->firstCapChannel = false;

    config->username = String(MQTT_USERNAME);
    config->password = String(MQTT_PASSWORD);

    // FORMAT
    // d:quickstart:<type-id>:<device-id>
    //config->clientId  = String("d:quickstart:esp8266meetup:") + macAddr;
    //config->topicPub  = String("iot-2/evt/status/fmt/json");
  });

  mqtt->on_after_prepare_configuration([&](MqttConnector::Config config) -> void {
    Serial.printf("[USER] HOST = %s\r\n", config.mqttHost.c_str());
    Serial.printf("[USER] PORT = %d\r\n", config.mqttPort);
    Serial.printf("[USER] PUB  = %s\r\n", config.topicPub.c_str());
    Serial.printf("[USER] SUB  = %s\r\n", config.topicSub.c_str());
  });

  register_publish_hooks();
  register_receive_hooks();

  mqtt->connect();
}
