#include <Wire.h>

void setup() {
  Wire.begin();
}

void loop() {
  Wire.beginTransmission(8); // start transmission to device with address 8
  Wire.write("Hello, slave!"); // send data
  Wire.endTransmission();
  delay(1000);
}
