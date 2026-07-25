#include "telegram_bot.h"

TelegramBot::TelegramBot(RelayController& relayController, IStorage& storageAdapter)
    : _relayController(relayController),
      _storageAdapter(storageAdapter),
      _isActive(false),
      _bot(nullptr),
      _lastCheckTime(0) {
  _client.setInsecure();
}

TelegramBot::~TelegramBot() {
  if (_bot != nullptr) {
    delete _bot;
  }
}

void TelegramBot::begin() {
  reconfigure();
  Serial.println("Application: Telegram Bot sub-system started");
}

void TelegramBot::reconfigure() {
  _token = _storageAdapter.getString("tg_token", "");

  if (_bot != nullptr) {
    delete _bot;
    _bot = nullptr;
  }

  if (_token.length() > 0) {
    _bot = new UniversalTelegramBot(_token, _client);
    _isActive = true;
    Serial.println("Application: Telegram Bot configured and ACTIVE");
  } else {
    _isActive = false;
    Serial.println("Application: Telegram Bot INACTIVE (missing token in NVS)");
  }
}

void TelegramBot::updateToken(const String& newToken) {
  _storageAdapter.saveString("tg_token", newToken.c_str());
  reconfigure();
}

bool TelegramBot::isActive() const {
  return _isActive;
}

void TelegramBot::loop() {
  if (!_isActive || _bot == nullptr) return;

  if (millis() - _lastCheckTime > _checkInterval) {
    int numNewMessages = _bot->getUpdates(_bot->last_message_received + 1);

    while (numNewMessages) {
      handleNewMessages(numNewMessages);
      numNewMessages = _bot->getUpdates(_bot->last_message_received + 1);
    }
    _lastCheckTime = millis();
  }
}

void TelegramBot::handleNewMessages(int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(_bot->messages[i].chat_id);

    String text = _bot->messages[i].text;

    if (text == "/on") {
      _relayController.on();
      _bot->sendMessage(chat_id, "Socket is ON 🟢", "");
    } else if (text == "/off") {
      _relayController.off();
      _bot->sendMessage(chat_id, "Socket is OFF 🔴", "");
    } else if (text == "/status") {
      RelayState state = _relayController.getStatus();
      bool physicalState = _relayController.getPhysicalState();
      String statusStr;

      switch (state) {
        case RelayState::ON:
          statusStr = "ON 🟢";
          break;
        case RelayState::OFF:
          statusStr = "OFF 🔴";
          break;
        case RelayState::SCHEDULED:
          statusStr = "Running on timer ⏳";
          break;
        default:
          statusStr = "Unknown ❓";
          break;
      }

      String physicalStr = physicalState ? "CLOSED (Power ON) ⚡" : "OPEN (Power OFF) 💤";

      String response = "Socket Status:\n";
      response += "• Logic Mode: " + statusStr + "\n";
      response += "• Relay Switch: " + physicalStr;

      _bot->sendMessage(chat_id, response, "");
    } else {
      _bot->sendMessage(
          chat_id, "Available commands:\n/on - Turn ON\n/off - Turn OFF\n/status - Check status",
          "");
    }
  }
}