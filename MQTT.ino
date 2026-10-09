#include "src/comms.h"
#include "src/config.h"

int lecturaSensorA0 = 0;

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
}

void loop() {
 
  commsUpdate();

  lecturaSensorA0 = analogRead(A0);
  Serial.print("sensorA0=");
  Serial.println(lecturaSensorA0);

  String msg = "";
  msg = msg + lecturaSensorA0;
  char message[58];
  msg.toCharArray(message, 58);
  Serial.print("menssage=");
  Serial.println(message);
  commsPublish(msg.c_str());

  delay(1000);
}
