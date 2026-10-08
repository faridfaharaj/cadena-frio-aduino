#include "config.h"
#include <LittleFS.h>

String WIFI_SSID;
String WIFI_PASSWORD;

JsonDocument getConfig() {
  JsonDocument doc;

  if (!LittleFS.begin()) {
    Serial.println("ERROR: Failed to mount LittleFS");
    return doc;
  }

  File file = LittleFS.open("/config.json", "r");

  if (!file) {
    Serial.println("ERROR: Failed to open /config.json");
    return doc;
  }

  DeserializationError error = deserializeJson(doc, file);

  file.close();

  if (error) {
    Serial.print("ERROR: ");
    Serial.println(error.c_str());
    doc.clear();
  }

  return doc;
}
