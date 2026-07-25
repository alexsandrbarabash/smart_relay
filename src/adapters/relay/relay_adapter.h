#ifndef RELAY_ADAPTER_H
#define RELAY_ADAPTER_H

#include <Arduino.h>

#include "../../domain/interfaces/relay_interface.h"

class RelayAdapter : public IRelay {
public:
  RelayAdapter(uint8_t pin);

  void begin() override;
  void on() override;
  void off() override;
  bool getState() override;

private:
  uint8_t _pin;
  bool _state;
};

#endif