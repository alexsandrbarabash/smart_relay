#ifndef NVS_STORAGE_ADAPTER_H
#define NVS_STORAGE_ADAPTER_H

#include <Preferences.h>

#include "../../domain/interfaces/storage_interface.h"

class NvsStorageAdapter : public IStorage {
private:
  Preferences _prefs;
  const char* _currentNamespace;

public:
  NvsStorageAdapter();

  void begin(const char* namespaceName) override;
  void saveString(const char* key, const char* value) override;
  String getString(const char* key, const char* defaultValue) override;
  void remove(const char* key) override;
  void putUInt(const char* key, uint32_t value);
  uint32_t getUInt(const char* key, uint32_t defaultValue = 0);
};

#endif