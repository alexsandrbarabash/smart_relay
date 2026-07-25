#include "relay_controller.h"

#include <Arduino.h>

RelayController::RelayController(IRelay& relay, NvsStorageAdapter& storage,
                                 ITimeAdapter& timeService)
    : _relay(relay), _storage(storage), _timeService(timeService) {
}

void RelayController::begin() {
  uint8_t savedRawState = _storage.getUInt("r_status", static_cast<uint8_t>(RelayState::OFF));
  RelayState currentState = static_cast<RelayState>(savedRawState);

  if (currentState == RelayState::SCHEDULED) {
    uint32_t targetTime = _storage.getUInt("r_target_time", 0);
    bool targetAction = _storage.getUInt("r_target_act", 0) == 1;
    uint32_t currentTime = _timeService.getCurrentTimeMillisecond();

    if (currentTime >= targetTime && targetTime != 0) {
      Serial.println("Controller [begin]: Schedule time passed while offline! Executing now.");
      if (targetAction)
        on();
      else
        off();
    } else {
      Serial.println("Controller [begin]: Schedule is active. Restoring temporary inverted state.");
      setPhysicalState(!targetAction);
    }
  } else if (currentState == RelayState::ON) {
    setPhysicalState(true);
  } else {
    setPhysicalState(false);
  }
}

void RelayController::loop() {
  switch (getStatus()) {
    // case RelayState::ON:
    //   Serial.println("State: ON");
    //   break;
    // case RelayState::OFF:
    //   Serial.println("State: OFF");
    //   break;
    case RelayState::SCHEDULED:
      Serial.println("State: SCHEDULED");
      break;
      // default:
      //   Serial.println("State: UNKNOWN");
      //   break;
  }

  if (getStatus() == RelayState::SCHEDULED) {
    uint32_t targetTime = _storage.getUInt("r_target_time", 0);
    uint32_t currentTime = _timeService.getCurrentTimeMillisecond();

    if (currentTime >= targetTime && targetTime != 0) {
      bool targetAction = _storage.getUInt("r_target_act", 0) == 1;
      Serial.println("Controller [loop]: Timer reached target UTC! Toggling relay.");
      Serial.printf("-> CurrentTime: %u\n", currentTime);
      Serial.printf("-> TargetTime:  %u\n", targetTime);

      if (targetAction)
        on();
      else
        off();
    }
  }
}

void RelayController::on() {
  setPhysicalState(true);
  saveStatusToNvs(RelayState::ON);
}

void RelayController::off() {
  setPhysicalState(false);
  saveStatusToNvs(RelayState::OFF);
}

void RelayController::timer(uint32_t utcTargetTime, bool targetActionOn) {
  _storage.putUInt("r_target_time", utcTargetTime);
  _storage.putUInt("r_target_act", targetActionOn ? 1 : 0);
  saveStatusToNvs(RelayState::SCHEDULED);

  setPhysicalState(!targetActionOn);

  Serial.printf("Controller: Timer armed for UTC %u. Current state inverted to %s\n", utcTargetTime,
                !targetActionOn ? "ON" : "OFF");
}

void RelayController::setPhysicalState(bool turnOn) {
  if (turnOn) {
    Serial.println("Controller: Hardware level -> HIGH");
    _relay.on();
  } else {
    Serial.println("Controller: Hardware level -> LOW");
    _relay.off();
  }
}

void RelayController::saveStatusToNvs(RelayState state) {
  _storage.putUInt("r_status", static_cast<uint8_t>(state));
}

RelayState RelayController::getStatus() {
  uint8_t rawState = _storage.getUInt("r_status", static_cast<uint8_t>(RelayState::OFF));
  return static_cast<RelayState>(rawState);
}

bool RelayController::getPhysicalState() {
  return _relay.getState();
}
