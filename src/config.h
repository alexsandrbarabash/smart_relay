#ifndef CONFIG_H
#define CONFIG_H

#define STRINGIZE_NX(A) #A
#define STRINGIZE(A) STRINGIZE_NX(A)

namespace Config {

constexpr const char* AP_SSID = STRINGIZE(AP_SSID_NAME);
constexpr const char* AP_PASSWORD = STRINGIZE(AP_PASS);
constexpr const char* DOMAIN = STRINGIZE(RELAY_DOMAIN_NAME);

constexpr unsigned long WIFI_CONNECTION_TIMEOUT = 15000;  // 15 second
}  // namespace Config

#endif