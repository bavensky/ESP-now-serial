#include <SoftwareSerial.h>

//SoftwareSerial mySerial (rxPin, txPin);
SoftwareSerial swSer(14, 12);

void setup() {
  Serial.begin(115200);
  swSer.begin(115200);

  Serial.println("\nSoftware serial slave");

}

void loop() {
  while (swSer.available() > 0) {
    Serial.write(swSer.read());
  }
}
