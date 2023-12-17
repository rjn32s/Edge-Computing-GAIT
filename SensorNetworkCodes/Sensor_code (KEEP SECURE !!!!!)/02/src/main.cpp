#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const char* ssid = "iii";
const char* password = "qwertyuio";
const char* mqttServer = "192.168.43.50";
const int mqttPort = 1883;
const char* mqttUser = "your_mqtt_username";
const char* mqttPassword = "your_mqtt_password";
const char* mqttTopic = "sensor/01";


// RTS INFO
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 19800;
const int   daylightOffset_sec = 3600;

const int jsonSize = JSON_OBJECT_SIZE(8);


#define RXD2 16
#define TXD2 17
StaticJsonDocument<jsonSize> jsonDoc;

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Received message [");
  Serial.print(topic);
  Serial.print("]: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
  }
  Serial.println();
}



void sendJsonData() {
  String jsonStr;
  serializeJson(jsonDoc, jsonStr);

  if (mqttClient.publish(mqttTopic, jsonStr.c_str())) {
    Serial.println("Published to MQTT server");
  } else {
    Serial.println("Failed to publish to MQTT server");
  }
}

struct tm getLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
  }
  return timeinfo;
}

void setup() {
  
  Serial.begin(115200);
  //while (!Serial) continue;

  Serial2.begin(9600,SERIAL_8N1, RXD2, TXD2);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");

  mqttClient.setServer(mqttServer, mqttPort);
  mqttClient.setCallback(callback);

  while (!mqttClient.connected()) {
    Serial.println("Connecting to MQTT server...");
    if (mqttClient.connect("ESP32", mqttUser, mqttPassword)) {
      Serial.println("Connected to MQTT server");
    } else {
      Serial.print("Failed to connect to MQTT server, rc=");
      Serial.println(mqttClient.state());
      delay(5000);
    }
  }

  jsonDoc["sensor"] = "04";
  jsonDoc["X"] = 0.0;
  jsonDoc["Y"] = 0.0;
  jsonDoc["Z"] = 0.0;
  jsonDoc["EMG"] = 0.0;
  jsonDoc["h"] = 0;
  jsonDoc["m"] = 0;
  jsonDoc["s"] = 0;

  Serial.println("JSON data:");
  serializeJsonPretty(jsonDoc, Serial);
  Serial.println();

  sendJsonData();

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  //struct tm timeinfo = getLocalTime();
  
}

void loop() {
  // Get the local time
struct tm timeinfo = getLocalTime();

// Concatenate the hour, minute, and second into a string with ":" as delimiter
//String timeString = String(timeinfo.tm_hour) + ":" + String(timeinfo.tm_min) + ":" + String(timeinfo.tm_sec);
  
  
  // ********************************** Deserialize the json and make another json***********************
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

      //Serial.print("X_rx = ");
      jsonDoc["X"] =  doc["X"].as<float>();
      //Serial.print("Y_rx = ");
      jsonDoc["Y"] =  doc["Y"].as<float>();
      //Serial.print("Z_rx = ");
      jsonDoc["Z"] =  doc["Z"].as<float>();
      //Serial.print("EMG_rx = ");
      jsonDoc["EMG"] =  doc["EMG"].as<float>();
      jsonDoc["h"] = timeinfo.tm_hour;
      jsonDoc["m"] = timeinfo.tm_min;
      jsonDoc["s"] = timeinfo.tm_sec;



      
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





  //jsonDoc["temperature"] = random(10, 30);
  //jsonDoc["humidity"] = random(40, 70);
  //Serial.println(timeString);
  
  Serial.println("JSON data:");
  serializeJsonPretty(jsonDoc, Serial);
  Serial.println();

  sendJsonData();

  mqttClient.loop();

  delay(1);
}



