#include "config.h"
#include <ArduinoJson.h>
#include <LittleFS.h>

String WIFI_SSID;
String WIFI_PASSWORD;

const char *mqtt_server;
int mqtt_port;
const char *channelTopicSub;
const char *channelTopicPub;

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

  mqtt_server = strdup(doc["mqtt"]["server"].as<const char *>());
  mqtt_port = doc["mqtt"]["port"].as<int>();
  channelTopicPub = strdup(doc["mqtt"]["topic_pub"].as<const char *>());
  channelTopicSub = strdup(doc["mqtt"]["topic_sub"].as<const char *>());

  return true;
}
