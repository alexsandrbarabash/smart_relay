#include "wifi_adapter.h"

#include <WiFi.h>

void WifiAdapter::init() {
  WiFi.persistent(false);
  WiFi.disconnect(true);
}

void WifiAdapter::connect(const char* ssid, const char* password) {
  WiFi.mode(WIFI_STA);
  WiFi.setTxPower(WIFI_POWER_5dBm);
  WiFi.begin(ssid, password);
}

void WifiAdapter::startAP(const char* ssid, const char* password) {
  Serial.println("Hardware: Switching to Access Point mode...");

  WiFi.mode(WIFI_AP);
  WiFi.setTxPower(WIFI_POWER_5dBm);
  WiFi.softAP(ssid, password);

  Serial.print("Hardware: AP started. IP address: ");
  Serial.println(WiFi.softAPIP());
}

bool WifiAdapter::isConnected() {
  return WiFi.status() == WL_CONNECTED;
}

bool WifiAdapter::isAP() {
  return (WiFi.getMode() & WIFI_AP) != 0;
}

String WifiAdapter::getIpAddress() {
  if (isAP()) {
    return WiFi.softAPIP().toString();
  }
  return WiFi.localIP().toString();
}