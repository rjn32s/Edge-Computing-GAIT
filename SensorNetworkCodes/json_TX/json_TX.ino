#include <ArduinoJson.h>
#include <Arduino_LSM9DS1.h>


int imuIndex = 0; // 0 - accelerometer, 1 - gyroscope, 2 - magnetometer
bool commandRecv = false; // flag used for indicating receipt of commands from serial port
bool startStream = false;
void setup() {
  // Initialize "debug" serial port
  // The data rate must be much higher than the "link" serial port
  Serial.begin(115200);
  //while (!Serial) continue;

  // Initialize the "link" serial port
  // Use a low data rate to reduce the error ratio
  Serial1.begin(9600);
  
  // Initialize IMU
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU");
    while (1);
  }
}
 
void loop() {
  // Values we want to transmit
  long timestamp = millis();
  int value = analogRead(1);
  float x, y, z;

  // Print the values on the "debug" serial port
  

  // Create the JSON document
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);
    int sensorValue = analogRead(A0);
  StaticJsonDocument<500> doc;
  doc["X"] = x;
  doc["Y"] = y;
  doc["Z"] = z;
  doc["EMG"] = sensorValue;
  serializeJson(doc, Serial1);
  Serial.print("X = ");
  Serial.println(x);
  Serial.print("Y = ");
  Serial.println(y);
  Serial.print("Z = ");
  Serial.println(z);
  Serial.print("EMG = ");
  Serial.println(sensorValue);
  
  
  Serial.println("---");
  }
  // Send the JSON document over the "link" serial port
  

  // Wait
  delay(1);
}
