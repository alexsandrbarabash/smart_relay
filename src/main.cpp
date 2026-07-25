#include <Arduino.h>
#include <DNSServer.h>
#include <ESPmDNS.h>

#include "adapters/nvs/nvs_storage_adapter.h"
#include "adapters/relay/relay_adapter.h"
#include "adapters/rtc/ds3231_adapter.h"
#include "adapters/wifi/wifi_adapter.h"
#include "application/api_server.h"
#include "application/telegram_bot.h"
#include "config.h"
#include "domain/relay/relay_controller.h"
#include "domain/wifi/wifi_controller.h"

WifiAdapter* wifiAdapter;
NvsStorageAdapter* storageAdapter;
WifiController* wifiController;
RelayAdapter* relayHardware;
RelayController* relayController;
ApiServer* api;
TelegramBot* bot;
DNSServer dnsServer;
ITimeAdapter* timeAdapter;

const byte DNS_PORT = 53;
#if defined(BOARD_NANO_ESP32)
const uint8_t RELAY_PIN = D2;
#elif defined(BOARD_C3_SUPERMINI)
const uint8_t RELAY_PIN = 2;
#endif

bool isServerStarted = false;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("System: Initializing...");

  storageAdapter = new NvsStorageAdapter();
  storageAdapter->begin("storage");

  timeAdapter = new DS3231Adapter();
  if (timeAdapter->begin()) {
    Serial.println("System: RTC Ready. Current Time: " + timeAdapter->getCurrentTime());
  } else {
    Serial.println("System: RTC Critical Error!");
  }

  wifiAdapter = new WifiAdapter();
  relayHardware = new RelayAdapter(RELAY_PIN);
  relayHardware->begin();

  wifiController = new WifiController(*wifiAdapter, *storageAdapter);
  relayController = new RelayController(*relayHardware, *storageAdapter, *timeAdapter);


  bot = new TelegramBot(*relayController, *storageAdapter);
  api = new ApiServer(*wifiController, *relayController, *bot, *timeAdapter);

  wifiController->begin();
  relayController->begin();
  bot->begin();

  Serial.println("System: Initialization complete.");
}

void loop() {
  wifiController->loop();
  relayController->loop();

  if (!isServerStarted && (wifiAdapter->isConnected() || wifiAdapter->isAP())) {
    api->begin();

    if (MDNS.begin(Config::DOMAIN)) {
      Serial.println("System: mDNS responder started (" + String(Config::DOMAIN) + ".local)");
      MDNS.addService("http", "tcp", 80);
    } else {
      Serial.println("System: Error setting up MDNS responder!");
    }

    if (wifiAdapter->isAP()) {
      String localDomain = String(Config::DOMAIN) + ".local";
      dnsServer.start(DNS_PORT, localDomain, WiFi.softAPIP());
      // dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());
      Serial.println("System: DNS Captive Portal started");
    }

    isServerStarted = true;
  }

  if (isServerStarted) {
    api->loop();

    if (wifiAdapter->isConnected()) {
      bot->loop();
    }

    if (wifiAdapter->isAP()) {
      dnsServer.processNextRequest();
    }
  }
}