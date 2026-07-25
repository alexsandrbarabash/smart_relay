#ifndef API_SERVER_H
#define API_SERVER_H

#include <LittleFS.h>
#include <WebServer.h>

#include "../domain/interfaces/time_interface.h"
#include "../domain/interfaces/wifi_interface.h"
#include "../domain/relay/relay_controller.h"
#include "../domain/wifi/wifi_controller.h"
#include "telegram_bot.h"

class ApiServer {
private:
  WebServer _server;
  WifiController& _wifiController;
  RelayController& _relayController;
  TelegramBot& _telegramBot;
  ITimeAdapter& _timeAdapter;

  void handleRoot();
  void handleStatus();
  void handleSetup();
  void handleSetupAP();

  void handleRelayOn();
  void handleRelayOff();
  void handleRelayTimer();
  void handleTelegramSetup();
  void syncClock();

  void handleNotFound();
  String getNotFoundHtml();
  bool handleFileRead(String path);
  String getContentType(String filename);

public:
  ApiServer(WifiController& wifiController, RelayController& relayController,
            TelegramBot& telegramBot, ITimeAdapter& timeAdapter);

  void begin();
  void loop();
};

#endif