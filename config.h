#pragma once
#include <Arduino.h>

extern String WIFI_SSID;
extern String WIFI_PASSWORD;

extern String mqtt_server;
extern int mqtt_port;
extern String channelTopicSub;
extern String channelTopicPub;

bool loadConfig();
