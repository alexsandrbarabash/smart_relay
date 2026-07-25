#pragma once
#include <RTClib.h>

#include "../../domain/interfaces/time_interface.h"

class DS3231Adapter : public ITimeAdapter {
public:
  DS3231Adapter();
  bool begin() override;
  String getCurrentTime() override;
  uint32_t getCurrentTimeMillisecond() override;
  void setTime(uint32_t timestamp) override;
  bool isNetworkRequired();

private:
  RTC_DS3231 _rtc;
};