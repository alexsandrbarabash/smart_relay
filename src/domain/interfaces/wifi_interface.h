#ifndef WIFI_INTERFACE_H
#define WIFI_INTERFACE_H

#include <Arduino.h>

class IWifi {
public:
  virtual ~IWifi() {
  }
  virtual void init() = 0;
  virtual void connect(const char* ssid, const char* password) = 0;
  virtual void startAP(const char* ssid, const char* password) = 0;
  virtual bool isConnected() = 0;
  virtual bool isAP() = 0;
  virtual String getIpAddress() = 0;
};

#endif