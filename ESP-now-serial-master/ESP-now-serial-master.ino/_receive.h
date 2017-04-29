extern SoftwareSerial swSer;

PubSubClient::callback_t on_message_arrived =
[&](const MQTT::Publish & pub) -> void {
  String topic = pub.topic();
  String payload = pub.payload_string();
  String text = topic + " => " + payload;

  if (payload == "ON")  {
    swSer.write('o');
  } else if (payload == "OFF") {
    swSer.write('h');
  }

  if (payload == "RON")  {
    digitalWrite(RELAY, LOW);
  } else if (payload == "ROFF") {
    digitalWrite(RELAY, HIGH);
  }
  Serial.println(text);
};
