#include <ArduinoJson.h>

void setup() {
  // Initialize "debug" serial port
  // The data rate must be much higher than the "link" serial port
  Serial.begin(115200);
  while (!Serial) continue;

  // Initialize the "link" serial port
  // Use a low data rate to reduce the error ratio
  Serial1.begin(9600);
}
 
void loop() {
  // Check if the other Arduino is transmitting
  if (Serial1.available()) 
  {
    // Allocate the JSON document
    // This one must be bigger than the sender's because it must store the strings
    StaticJsonDocument<600> doc;

    // Read the JSON document from the "link" serial port
    DeserializationError err = deserializeJson(doc, Serial1);

    if (err == DeserializationError::Ok) 
    {
      // Print the values
      // (we must use as<T>() to resolve the ambiguity)
      Serial.print("X_rx = ");
      Serial.println(doc["X"].as<float>());
      Serial.print("Y_rx = ");
      Serial.println(doc["Y"].as<float>());
      Serial.print("Z_rx = ");
      Serial.println(doc["Z"].as<float>());
      Serial.print("EMG_rx = ");
      Serial.println(doc["EMG"].as<int>());


      
    } 
    else 
    {
      // Print error to the "debug" serial port
      Serial.print("deserializeJson() returned ");
      Serial.println(err.c_str());
  
      // Flush all bytes in the "link" serial port buffer
      while (Serial1.available() > 0)
        Serial1.read();
    }
  }
}
