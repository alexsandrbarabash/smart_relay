#ifndef WIFI_CONTROLLER_H
#define WIFI_CONTROLLER_H

#include "../interfaces/storage_interface.h"
#include "../interfaces/wifi_interface.h"

class WifiController {
public:
  enum class Mode { INITIALIZING, CONNECTING, CONNECTED, ACCESS_POINT };

  WifiController(IWifi& wifiAdapter, IStorage& storage);

  void begin();
  void loop();
  void updateSettings(String ssid, String password);
  void setupAP();
  bool isAP();
  String getIpAddress();

private:
  IStorage& _storage;
  IWifi& _wifi;
  Mode _currentMode = Mode::INITIALIZING;
  unsigned long _lastActionTime = 0;
};

#endif