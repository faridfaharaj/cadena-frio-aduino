#pragma once
#include <cstdint>

class ComponentBase {
protected:
  virtual uint8_t getPin(int idx) const = 0;
};
