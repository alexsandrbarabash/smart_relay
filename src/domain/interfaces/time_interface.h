#pragma once
#include <Arduino.h>

class ITimeAdapter {
public:
  virtual ~ITimeAdapter() {
  }
  virtual bool begin() = 0;
  virtual String getCurrentTime() = 0;
  virtual uint32_t getCurrentTimeMillisecond() = 0;
  virtual void setTime(uint32_t timestamp);
  virtual bool isNetworkRequired() = 0;
};
