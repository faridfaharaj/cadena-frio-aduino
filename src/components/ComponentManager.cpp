#include "ComponentManager.h"
#include "pins_arduino.h"

RGBLed ComponentManager::led(D0, D1, D2);
DHT ComponentManager::dht(D3, DHT11);
