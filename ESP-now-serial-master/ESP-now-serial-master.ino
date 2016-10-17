#include <SoftwareSerial.h>

//SoftwareSerial mySerial (rxPin, txPin);
SoftwareSerial swSer(14, 12);

#define button0 0
#define button13 13

void setup() {
  Serial.begin(115200);
  swSer.begin(115200);

  Serial.println("\nSoftware serial master");

  swSer.write("man");
  swSer.println("");

  pinMode(button0, INPUT);
  pinMode(button13, INPUT);
}

void loop() {
  if (digitalRead(button0) == 0)  {
    delay(200);
    swSer.write("man");
    swSer.println("");
  }
  if (digitalRead(button13) == 0)  {
    delay(200);
    swSer.write("bavensky");
    swSer.println("");
  }

}
