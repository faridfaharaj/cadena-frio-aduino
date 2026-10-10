#pragma once

#include "RGBLed.h"
#include <Arduino.h>
#include <DHT.h>

class ComponentManager {
public:
  static RGBLed led;
  static DHT dht;

  static void initializeComponents();
};
