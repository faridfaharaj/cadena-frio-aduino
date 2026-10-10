#include "src/comms.h"
#include "src/components/ComponentManager.h"
#include "src/config.h"

void setup() {
  Serial.begin(115200);
  delay(200);

  // Json Configuration
  JsonDocument doc = getConfig();

  if (doc.isNull()) {
    Serial.println("Config failed, halting");
    while (true)
      delay(1000);
  }

  // wifi
  const char *WIFI_SSID = doc["wifi"]["ssid"] | "";
  const char *WIFI_PASSWORD = doc["wifi"]["password"] | "";

  const char *server = doc["mqtt"]["server"] | "";
  const char *topicPub = doc["mqtt"]["topic_pub"] | "";
  const char *topicSub = doc["mqtt"]["topic_sub"] | "";

  uint16_t port = doc["mqtt"]["port"] | 1883;

  commsInit(WIFI_SSID, WIFI_PASSWORD, server, port, topicSub, topicPub);

  ComponentManager::initializeComponents();
}

void loop() {

  commsUpdate();

  float humidity = ComponentManager::dht.readHumidity();
  float temperature = ComponentManager::dht.readTemperature();

  char message[80];
  snprintf(message, sizeof(message), "Temperature: %.1f C - Humidity: %.1f%%",
           temperature, humidity);

  Serial.println(message);
  commsPublish(message);

  delay(1000);
}
