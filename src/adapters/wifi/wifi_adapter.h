#ifndef WIFI_ADAPTER_H
#define WIFI_ADAPTER_H

#include "../../domain/interfaces/wifi_interface.h"

class WifiAdapter : public IWifi {
public:
  void init();
  void connect(const char* ssid, const char* password);
  void startAP(const char* ssid, const char* password);
  bool isConnected();
  bool isAP();
  String getIpAddress();
};

#endif