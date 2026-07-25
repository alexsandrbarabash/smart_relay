#include "api_server.h"

#include <ArduinoJson.h>

#include "../config.h"

ApiServer::ApiServer(WifiController& wifiController, RelayController& relayController,
                     TelegramBot& telegramBot, ITimeAdapter& timeAdapter)
    : _server(80),
      _wifiController(wifiController),
      _relayController(relayController),
      _telegramBot(telegramBot),
      _timeAdapter(timeAdapter) {
}

void ApiServer::begin() {
  if (!LittleFS.begin(true)) {
    Serial.println("Application: LittleFS Mount Failed! Frontend won't load.");
  }

  _server.on("/on", HTTP_POST, std::bind(&ApiServer::handleRelayOn, this));
  _server.on("/off", HTTP_POST, std::bind(&ApiServer::handleRelayOff, this));
  _server.on("/timer", HTTP_POST, std::bind(&ApiServer::handleRelayTimer, this));

  _server.on("/status", HTTP_GET, std::bind(&ApiServer::handleStatus, this));
  _server.on("/setup", HTTP_POST, std::bind(&ApiServer::handleSetup, this));
  _server.on("/setup-ap", HTTP_POST, std::bind(&ApiServer::handleSetupAP, this));

  _server.on("/telegram", HTTP_POST, std::bind(&ApiServer::handleTelegramSetup, this));
  _server.on("/sync-time", HTTP_POST, std::bind(&ApiServer::syncClock, this));

  _server.onNotFound(std::bind(&ApiServer::handleNotFound, this));

  _server.begin();
  Serial.println("Application: HTTP API Server started");
}

void ApiServer::loop() {
  _server.handleClient();
}

void ApiServer::handleRoot() {
  _server.send(200, "text/plain", "Hello World! ESP32 API is running.");
}

void ApiServer::handleStatus() {
  RelayState state = _relayController.getStatus();
  bool physicalState = _relayController.getPhysicalState();
  String relayStateStr;

  switch (state) {
    case RelayState::ON:
      relayStateStr = "on";
      break;
    case RelayState::OFF:
      relayStateStr = "off";
      break;
    case RelayState::SCHEDULED:
      relayStateStr = "scheduled";
      break;
    default:
      relayStateStr = "off";
      break;
  }

  JsonDocument doc;
  doc["deviceName"] = "Test Name";
  doc["relayState"] = relayStateStr;
  doc["physicalState"] = physicalState;
  doc["telegramActive"] = _telegramBot.isActive();
  doc["time"] = _timeAdapter.getCurrentTimeMillisecond();

  String jsonResponse;
  serializeJson(doc, jsonResponse);

  _server.sendHeader("Connection", "close");
  _server.sendHeader("Access-Control-Allow-Origin", "*");
  _server.send(200, "application/json", jsonResponse);
}

void ApiServer::handleTelegramSetup() {
  if (!_server.hasArg("plain")) {
    _server.sendHeader("Connection", "close");
    _server.send(400, "application/json", "{\"error\":\"Empty body\"}");
    return;
  }

  String jsonBody = _server.arg("plain");

  JsonDocument docInput;
  DeserializationError error = deserializeJson(docInput, jsonBody);

  if (error) {
    _server.sendHeader("Connection", "close");
    _server.send(400, "application/json", "{\"error\":\"Invalid JSON format\"}");
    return;
  }

  if (!docInput.containsKey("token")) {
    _server.sendHeader("Connection", "close");
    _server.send(400, "application/json", "{\"error\":\"Missing token field\"}");
    return;
  }

  String token = docInput["token"];

  _telegramBot.updateToken(token);

  JsonDocument docOutput;
  docOutput["status"] = "success";
  docOutput["message"] = "Telegram token updated successfully";

  String jsonResponse;
  serializeJson(docOutput, jsonResponse);

  _server.sendHeader("Connection", "close");
  _server.sendHeader("Access-Control-Allow-Origin", "*");
  _server.send(200, "application/json", jsonResponse);

  Serial.println("Application: Telegram token updated via HTTP API");
}

void ApiServer::handleSetup() {
  if (!_server.hasArg("ssid") || !_server.hasArg("pass")) {
    _server.send(400, "application/json", "{\"error\":\"Missing ssid or pass\"}");
    return;
  }

  String newSsid = _server.arg("ssid");
  String newPass = _server.arg("pass");

  _server.send(200, "application/json", "{\"message\":\"Settings saved. Reconnecting...\"}");

  Serial.printf("Application: New WiFi config received: %s\n", newSsid.c_str());

  _wifiController.updateSettings(newSsid, newPass);
}

void ApiServer::handleSetupAP() {
  Serial.println("Application: Setting up WiFi access point...");
  _wifiController.setupAP();
  _server.send(200, "application/json", "{\"message\":\"WiFi settings reset. Reconnecting...\"}");
}

void ApiServer::handleRelayOn() {
  _relayController.on();
  _server.sendHeader("Access-Control-Allow-Origin", "*");
  _server.send(200, "text/plain", "OK: Relay is ON");
}

void ApiServer::handleRelayOff() {
  _relayController.off();
  _server.sendHeader("Connection", "close");
  _server.sendHeader("Access-Control-Allow-Origin", "*");
  _server.send(200, "text/plain", "OK: Relay is OFF");
}

void ApiServer::handleRelayTimer() {
  if (!_server.hasArg("plain")) {
    _server.sendHeader("Connection", "close");
    _server.send(400, "application/json", "{\"error\":\"Empty body\"}");
    return;
  }

  String jsonBody = _server.arg("plain");

  JsonDocument docInput;
  DeserializationError error = deserializeJson(docInput, jsonBody);

  if (error) {
    _server.sendHeader("Connection", "close");
    _server.send(400, "application/json", "{\"error\":\"Invalid JSON format\"}");
    return;
  }

  if (!docInput.containsKey("operation") || !docInput.containsKey("time")) {
    _server.sendHeader("Connection", "close");
    _server.send(400, "application/json", "{\"error\":\"Missing fields: operation or time\"}");
    return;
  }

  int operation = docInput["operation"];
  unsigned long targetTime = docInput["time"];

  JsonDocument docOutput;
  docOutput["status"] = "success";
  docOutput["message"] = "Timer configured";
  docOutput["operation"] = operation;
  docOutput["time"] = targetTime;

  String jsonResponse;
  serializeJson(docOutput, jsonResponse);

  _relayController.timer(targetTime, operation);
  _server.sendHeader("Connection", "close");
  _server.sendHeader("Access-Control-Allow-Origin", "*");
  _server.send(200, "application/json", jsonResponse);

  Serial.printf("Application: Timer HTTP event. Op: %d, Time: %lu\n", operation, targetTime);
}

void ApiServer::syncClock() {
  if (!_server.hasArg("plain")) {
    _server.sendHeader("Connection", "close");
    _server.send(400, "application/json", "{\"error\":\"Empty body\"}");
    return;
  }

  String jsonBody = _server.arg("plain");

  Serial.printf("RTC: jsonBody %s\n", jsonBody.c_str());

  JsonDocument docInput;
  DeserializationError error = deserializeJson(docInput, jsonBody);

  if (error) {
    _server.sendHeader("Connection", "close");
    _server.send(400, "application/json", "{\"error\":\"Invalid JSON format\"}");
    return;
  }

  if (!docInput.containsKey("time")) {
    _server.sendHeader("Connection", "close");
    _server.send(400, "application/json", "{\"error\":\"Missing time field\"}");
    return;
  }

  uint32_t time = docInput["time"];

  _timeAdapter.setTime(time);

  _server.sendHeader("Connection", "close");
  _server.sendHeader("Access-Control-Allow-Origin", "*");
  _server.send(200);
}

String ApiServer::getNotFoundHtml() {
  return R"=====(
        <!DOCTYPE html>
        <html>
        <head><meta charset="UTF-8"><title>404</title></head>
        <body style="text-align:center; padding-top:50px; background:#1a1a1a; color:white; font-family:sans-serif;">
            <h1 style="font-size:50px; color:#ff4757;">404</h1>
            <p>Page not found</p>
        </body>
        </html>
    )=====";
}

String ApiServer::getContentType(String filename) {
  if (_server.hasArg("download"))
    return "application/octet-stream";
  else if (filename.endsWith(".htm") || filename.endsWith(".html"))
    return "text/html";
  else if (filename.endsWith(".css"))
    return "text/css";
  else if (filename.endsWith(".js"))
    return "application/javascript";
  else if (filename.endsWith(".png"))
    return "image/png";
  else if (filename.endsWith(".gif"))
    return "image/gif";
  else if (filename.endsWith(".jpg"))
    return "image/jpeg";
  else if (filename.endsWith(".ico"))
    return "image/x-icon";
  else if (filename.endsWith(".xml"))
    return "text/xml";
  else if (filename.endsWith(".pdf"))
    return "application/x-pdf";
  else if (filename.endsWith(".zip"))
    return "application/x-zip";
  else if (filename.endsWith(".gz"))
    return "application/x-gzip";
  return "text/plain";
}

bool ApiServer::handleFileRead(String path) {
  Serial.println("Application: File requested: " + path);

  if (path.endsWith("/")) path += "index.html";

  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";

  if (LittleFS.exists(pathWithGz) || LittleFS.exists(path)) {
    if (LittleFS.exists(pathWithGz)) path = pathWithGz;
    File file = LittleFS.open(path, "r");
    _server.streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
}

void ApiServer::handleNotFound() {
  if (handleFileRead(_server.uri())) {
    return;
  }

  if (_wifiController.isAP()) {
    _server.sendHeader("Location", "http://" + String(Config::DOMAIN) + ".local/", true);
    _server.send(302, "text/plain", "");
    return;
  }

  if (_server.method() == HTTP_GET) {
    if (handleFileRead("/index.html")) return;
  }

  _server.send(404, "text/html", getNotFoundHtml());
}