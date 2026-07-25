#ifndef STORAGE_INTERFACE_H
#define STORAGE_INTERFACE_H

#include <Arduino.h>

class IStorage {
public:
  virtual ~IStorage() {
  }
  virtual void begin(const char* namespaceName) = 0;
  virtual void saveString(const char* key, const char* value) = 0;
  virtual String getString(const char* key, const char* defaultValue) = 0;
  virtual void remove(const char* key) = 0;
};

#endif
