#pragma once
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

void wifiInit(const char *WIFI_SSID, const char *WIFI_PASSWORD);
void reconnect();

void commsInit(const char *ssid, const char *password, const char *server,
               uint16_t port, const char *sub, const char *pub);
void commsUpdate();
void commsPublish(const char *payload);

void callback(char *topic, byte *payload, unsigned int length);
