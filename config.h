#pragma once
#include <Arduino.h>

extern String WIFI_SSID;
extern String WIFI_PASSWORD;

extern const char *mqtt_server;
extern int mqtt_port;
extern const char *channelTopicSub;
extern const char *channelTopicPub;

bool loadConfig();
