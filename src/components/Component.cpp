#include "Component.h"

template <std::size_t N> Component<N>::Component(const Pin (&pins)[N]) {
  for (std::size_t i = 0; i < N; ++i) {
    pinMode(pins[i].pin, pins[i].mode);
    this->pins[i] = pins[i].pin;
  }
}

template class Component<1>;
template class Component<2>;
template class Component<3>;
