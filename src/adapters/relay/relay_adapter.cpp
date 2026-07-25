#include "relay_adapter.h"

#define RELAY_ON  LOW
#define RELAY_OFF HIGH

RelayAdapter::RelayAdapter(uint8_t pin) : _pin(pin), _state(false) {
}

void RelayAdapter::begin() {
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, LOW);
}

void RelayAdapter::on() {
  digitalWrite(_pin, RELAY_ON);
  _state = true;
  Serial.println("Relay Hardware: ON");
}

void RelayAdapter::off() {
  digitalWrite(_pin, RELAY_OFF);
  _state = false;
  Serial.println("Relay Hardware: OFF");
}

bool RelayAdapter::getState() {
  return _state;
}