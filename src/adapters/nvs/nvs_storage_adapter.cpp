#include "nvs_storage_adapter.h"

NvsStorageAdapter::NvsStorageAdapter() : _currentNamespace("storage") {
}

void NvsStorageAdapter::begin(const char* namespaceName) {
  _currentNamespace = namespaceName;
  _prefs.begin(_currentNamespace, false);
  _prefs.end();
}

void NvsStorageAdapter::saveString(const char* key, const char* value) {
  _prefs.begin(_currentNamespace, false);
  _prefs.putString(key, value);
  _prefs.end();
}

String NvsStorageAdapter::getString(const char* key, const char* defaultValue) {
  _prefs.begin(_currentNamespace, true);
  String value = _prefs.getString(key, defaultValue);
  _prefs.end();
  return value;
}

void NvsStorageAdapter::remove(const char* key) {
  _prefs.begin(_currentNamespace, false);
  _prefs.remove(key);
  _prefs.end();
}

void NvsStorageAdapter::putUInt(const char* key, uint32_t value) {
  _prefs.begin(_currentNamespace, false);
  _prefs.putUInt(key, value);
  _prefs.end();
}

uint32_t NvsStorageAdapter::getUInt(const char* key, uint32_t defaultValue) {
  _prefs.begin(_currentNamespace, true);
  uint32_t value = _prefs.getUInt(key, defaultValue);
  _prefs.end();
  return value;
}
