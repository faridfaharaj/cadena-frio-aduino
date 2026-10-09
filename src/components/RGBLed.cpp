#include "RGBLed.h"

RGBLed::RGBLed(uint8_t red_pin, uint8_t green_pin, uint8_t blue_pin)
    : Component<3>(
          {{red_pin, OUTPUT}, {green_pin, OUTPUT}, {blue_pin, OUTPUT}}) {}

short toRGBChannel(short brightness) {
    return brightness * 1023L / 255;
}

void RGBLed::setColor(uint32_t color) {
    analogWrite(pins[0], 1023 - toRGBChannel((color >> 16) & 0xFF));
    analogWrite(pins[1], 1023 - toRGBChannel((color >> 8) & 0xFF));
    analogWrite(pins[2], 1023 - toRGBChannel(color & 0xFF));
}

void RGBLed::setColor(uint16_t red_val, uint16_t green_val, uint16_t blue_val){
    analogWrite(pins[0], red_val);
    analogWrite(pins[1], green_val);
    analogWrite(pins[2], blue_val);
}

void RGBLed::setColor(uint8_t red_val, uint8_t green_val, uint8_t blue_val){
  digitalWrite(pins[0], red_val);
  digitalWrite(pins[1], green_val);
  digitalWrite(pins[2], blue_val);
}

void RGBLed::setRed(){
  digitalWrite(pins[0], LOW);
  digitalWrite(pins[1], HIGH);
  digitalWrite(pins[2], HIGH);
}

void RGBLed::setGreen(){ 
  digitalWrite(pins[0], HIGH);
  digitalWrite(pins[1], LOW);
  digitalWrite(pins[2], HIGH);
}

void RGBLed::setBlue(){
  digitalWrite(pins[0], HIGH);
  digitalWrite(pins[1], HIGH);
  digitalWrite(pins[2], LOW);
}

void RGBLed::setOff(){
  digitalWrite(pins[0], HIGH);
  digitalWrite(pins[1], HIGH);
  digitalWrite(pins[2], HIGH);
}
