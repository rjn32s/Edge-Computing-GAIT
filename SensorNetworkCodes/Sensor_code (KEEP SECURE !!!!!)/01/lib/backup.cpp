#include <ArduinoJson.h>

#include <WiFi.h>
#include "time.h"

const char* ssid = "iii";
const char* password = "qwertyuio";

#define RXD2 16
#define TXD2 17



const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 19800;
const int   daylightOffset_sec = 3600;

void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  Serial.print("Day of week: ");
  Serial.println(&timeinfo, "%A");
  Serial.print("Month: ");
  Serial.println(&timeinfo, "%B");
  Serial.print("Day of Month: ");
  Serial.println(&timeinfo, "%d");
  Serial.print("Year: ");
  Serial.println(&timeinfo, "%Y");
  Serial.print("Hour: ");
  Serial.println(&timeinfo, "%H");
  Serial.print("Hour (12 hour format): ");
  Serial.println(&timeinfo, "%I");
  Serial.print("Minute: ");
  Serial.println(&timeinfo, "%M");
  Serial.print("Second: ");
  Serial.println(&timeinfo, "%S");

  Serial.println("Time variables");
  char timeHour[3];
  strftime(timeHour,3, "%H", &timeinfo);
  Serial.println(timeHour);
  char timeWeekDay[10];
  strftime(timeWeekDay,10, "%A", &timeinfo);
  Serial.println(timeWeekDay);
  Serial.println();
}

void setup() {
  // Initialize "debug" serial port
  // The data rate must be much higher than the "link" serial port

  Serial.begin(115200);
  while (!Serial) continue;

  // Initialize the "link" serial port
  // Use a low data rate to reduce the error ratio
  Serial2.begin(9600,SERIAL_8N1, RXD2, TXD2);



  // Let's connect to the WIFI
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

}
 
void loop() {
  // Check if the other Arduino is transmitting
  Serial.println("Running:::)))");
  printLocalTime();
  delay(1000);
  if (Serial2.available()) 
  {
    // Allocate the JSON document
    // This one must be bigger than the sender's because it must store the strings
    StaticJsonDocument<600> doc;

    // Read the JSON document from the "link" serial port
    DeserializationError err = deserializeJson(doc, Serial2);



    // ************************** Let's Publish the code with the MQTT protocol*********************
    // Topic = "sensor/01"
      


    // ************************************************************************************************

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
      while (Serial2.available() > 0)
        Serial2.read();
    }
  }
}


