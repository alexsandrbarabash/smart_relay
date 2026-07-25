#include "ds3231_adapter.h"

DS3231Adapter::DS3231Adapter() {
}

bool DS3231Adapter::begin() {
  if (!_rtc.begin()) {
    Serial.println("RTC: Couldn't find DS3231");
    return false;
  }
  if (_rtc.lostPower()) {
    Serial.println("RTC: Lost power, let's set the time!");
    _rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  return true;
}

String DS3231Adapter::getCurrentTime() {
  DateTime now = _rtc.now();
  char buffer[10];
  sprintf(buffer, "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
  return String(buffer);
}

uint32_t DS3231Adapter::getCurrentTimeMillisecond() {
  return _rtc.now().unixtime();
}

bool DS3231Adapter::isNetworkRequired() {
  return false;
}

void DS3231Adapter::setTime(uint32_t timestamp) {
  _rtc.adjust(DateTime(timestamp));
  
  Serial.printf("RTC: Internal clock synchronized to Unix UTC %u\n", timestamp);
  DateTime now = _rtc.now();
  Serial.printf("RTC: Now %u\n", now.unixtime());
}
