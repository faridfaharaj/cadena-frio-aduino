#include "config.h"
#include <ArduinoJson.h>
#include <LittleFS.h>

String WIFI_SSID;
String WIFI_PASSWORD;

String mqtt_server;
int mqtt_port;
String channelTopicSub;
String channelTopicPub;

bool loadConfig() {
  if (!LittleFS.begin()) {
    Serial.println("ERROR: Failed to mount LittleFS");
    return false;
  }

  File file = LittleFS.open("/config.json", "r");

  if (!file) {
    Serial.println("ERROR: Failed to open /config.json");
    return false;
  }

  JsonDocument doc;

  DeserializationError error = deserializeJson(doc, file);

  file.close();

  if (error) {
    Serial.print("ERROR: ");
    Serial.println(error.c_str());
    return false;
  }

  WIFI_SSID = doc["wifi"]["ssid"].as<String>();
  WIFI_PASSWORD = doc["wifi"]["password"].as<String>();

  mqtt_server = doc["mqtt"]["server"].as<String>();
  mqtt_port = doc["mqtt"]["port"].as<int>();
  channelTopicPub = doc["mqtt"]["topic_pub"].as<String>();
  channelTopicSub = doc["mqtt"]["topic_sub"].as<String>();

  return true;
}
