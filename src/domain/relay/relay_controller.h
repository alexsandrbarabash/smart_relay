#pragma once

#include "../../adapters/nvs/nvs_storage_adapter.h"
#include "../../domain/interfaces/relay_interface.h"
#include "../../domain/interfaces/time_interface.h"

enum class RelayState : uint8_t { ON = 1, OFF = 2, SCHEDULED = 3 };
enum class PhysicalRelayState : uint8_t { ON = 1, OFF = 2 };

class RelayController {
public:
  RelayController(IRelay& relay, NvsStorageAdapter& storage, ITimeAdapter& timeService);

  void begin();
  void loop();

  void on();
  void off();
  void timer(uint32_t utcTargetTime, bool targetActionOn);

  RelayState getStatus();
  bool getPhysicalState();

private:
  IRelay& _relay;
  NvsStorageAdapter& _storage;
  ITimeAdapter& _timeService;

  void setPhysicalState(bool turnOn);
  void saveStatusToNvs(RelayState state);
  void checkAndExecuteSchedule();
};