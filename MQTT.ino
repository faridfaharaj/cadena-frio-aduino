#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "config.h"
#include <ArduinoJson.h>
#include <LittleFS.h>

#define SENSOR_A0 A0
#define LED1 D0 // Red
#define LED2 D1 // Green
#define LED3 D2 // Blue

const char *mqtt_server;
const char *channelTopicSub;
const char *channelTopicPub;
char clientId[24];

WiFiClient espClient;
PubSubClient client(espClient);

int lecturaSensorA0 = 0;

void setup_wifi(const char *WIFI_SSID, const char *WIFI_PASSWORD) {
  delay(100);
  Serial.println();
  Serial.print("macAddress: ");
  Serial.println(WiFi.macAddress());
  Serial.print("Seting up WiFi --> ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  short timeOutTimer = 30 * 2;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    if (timeOutTimer == 0) {
      Serial.print("\r      \r");
      Serial.print("Its taking too long to connect    ");
    }

    Serial.print(".");

    if (timeOutTimer % 4 == 0) {
      timeOutTimer = timeOutTimer < 0 ? 0 : timeOutTimer;
      Serial.print("\b\b\b\b    \b\b\b\b");
    }

    timeOutTimer--;
  }
  randomSeed(micros());
  Serial.println();
  Serial.println("WiFi connnected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char *topic, byte *payload, unsigned int length) {
  char *cstring = (char *)payload;
  cstring[length] = '\0'; // Adds a terminate to end of string based on length
                          // of current payload
  Serial.println(cstring);
  switch (cstring[1]) {
  case 'R':
    Serial.println("\tRojo");
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
    break;
  case 'G':
    Serial.println("\tVerde");
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, HIGH);
    break;
  case 'B':
    Serial.println("\tAzul");
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, LOW);
    break;
  case 'X':
    Serial.println("\tAPAGAR todo!");
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
    break;
  default:
    break;
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Trying MQTT connection ...");

    if (client.connect(clientId)) {
      Serial.println("Connecting to MQTT broker");
      client.subscribe(channelTopicSub);
    } else {
      Serial.print("Connection Error, rc=");
      Serial.print(client.state());
      Serial.println(" ... retrying in 6 seconds");
      delay(6000);
    }
  }
}

// ARDUINO FRAMEWORK FUNCTIONS ----------
void setup() {
  Serial.begin(115200);
  delay(200);

  // Pins
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, HIGH);

  // Json Configuration
  JsonDocument doc = loadConfig();

  if (doc.isNull()) {
    Serial.println("Config failed, halting");
    while (true)
      delay(1000);
  }

  const char *WIFI_SSID = doc["wifi"]["ssid"] | "";
  const char *WIFI_PASSWORD = doc["wifi"]["password"] | "";

  mqtt_server = strdup(doc["mqtt"]["server"] | "");
  channelTopicPub = strdup(doc["mqtt"]["topic_pub"] | "");
  channelTopicSub = strdup(doc["mqtt"]["topic_sub"] | "");

  uint16_t mqtt_port = doc["mqtt"]["port"] | 1883;

  // Client ID
  uint8_t mac[6];
  WiFi.macAddress(mac);
  snprintf(clientId, sizeof(clientId), "%s-%02X%02X%02X%02X%02X%02X", "Client-",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

  // wifi
  setup_wifi(WIFI_SSID, WIFI_PASSWORD);
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  lecturaSensorA0 = analogRead(A0);
  Serial.print("sensorA0=");
  Serial.println(lecturaSensorA0);

  String msg = String(clientId) + ": ";
  msg = msg + lecturaSensorA0;
  char message[58];
  msg.toCharArray(message, 58);
  Serial.print("menssage=");
  Serial.println(message);
  client.publish(channelTopicPub, message);

  delay(1000);
}
