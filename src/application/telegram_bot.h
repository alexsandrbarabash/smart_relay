#pragma once
#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>

#include "../domain/interfaces/storage_interface.h"
#include "../domain/relay/relay_controller.h"

class TelegramBot {
public:
  TelegramBot(RelayController& relayController, IStorage& storageAdapter);
  ~TelegramBot();

  void begin();
  void loop();
  void updateToken(const String& newToken);

  void reconfigure();

  bool isActive() const;

private:
  RelayController& _relayController;
  IStorage& _storageAdapter;

  String _token;
  String _securedChatId;
  bool _isActive;

  WiFiClientSecure _client;
  UniversalTelegramBot* _bot;

  unsigned long _lastCheckTime;
  const unsigned long _checkInterval = 2000;

  void handleNewMessages(int numNewMessages);
};