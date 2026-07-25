#ifndef IRELAY_H
#define IRELAY_H

class IRelay {
public:
  virtual ~IRelay() {
  }
  virtual void on() = 0;
  virtual void off() = 0;
  virtual bool getState() = 0;
  virtual void begin() = 0;
};

#endif