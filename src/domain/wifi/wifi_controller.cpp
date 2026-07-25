#include "wifi_controller.h"

#include <Arduino.h>

#include "../../config.h"

WifiController::WifiController(IWifi& wifi, IStorage& storage) : _wifi(wifi), _storage(storage) {
}

void WifiController::begin() {
  _wifi.init();

  _storage.begin("wifi");

  String ssid = _storage.getString("ssid", "");
  String pass = _storage.getString("pass", "");

  _wifi.connect(ssid.c_str(), pass.c_str());

  Serial.print("Domain: Attempting to connect to: ");
  Serial.println(ssid);

  _currentMode = Mode::CONNECTING;
  _lastActionTime = millis();
}

void WifiController::loop() {
  switch (_currentMode) {
    case Mode::CONNECTING:
      if (_wifi.isConnected()) {
        _currentMode = Mode::CONNECTED;
        Serial.println("Domain: WiFi Connected Successfully!");

      } else if (millis() - _lastActionTime > Config::WIFI_CONNECTION_TIMEOUT) {
        Serial.println("Domain: Connection timeout! Starting AP mode...");
        setupAP();
      }

      break;

    case Mode::CONNECTED:
      if (!_wifi.isConnected()) {
        Serial.println("Domain: Connection lost! Retrying...");
        begin();
      }
      break;

    case Mode::ACCESS_POINT:
      break;

    default:
      break;
  }
}

void WifiController::updateSettings(String ssid, String password) {
  Serial.println("Domain: Updating WiFi settings...");

  _storage.begin("wifi");
  _storage.saveString("ssid", ssid.c_str());
  _storage.saveString("pass", password.c_str());

  begin();
}

void WifiController::setupAP() {
  Serial.println("Domain: Starting AP mode...");
  _wifi.startAP(Config::AP_SSID, Config::AP_PASSWORD);
  _currentMode = Mode::ACCESS_POINT;
}

bool WifiController::isAP() {
  return _wifi.isAP();
}

String WifiController::getIpAddress() {
  return _wifi.getIpAddress();
}