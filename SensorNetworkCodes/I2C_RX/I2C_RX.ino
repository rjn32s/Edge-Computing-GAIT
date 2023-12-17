#include <Wire.h>

void setup() {
  Wire.begin(8); // join i2c bus with address 8
  Wire.onReceive(receiveEvent); // register event
}

void receiveEvent(int howMany) {
  while (Wire.available()) {
    char c = Wire.read();
    Serial.print(c);
  }
  Serial.println();
}

void loop() {
  // do nothing
}
