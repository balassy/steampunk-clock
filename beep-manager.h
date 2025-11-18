#ifndef BEEP_MANAGER_H
#define BEEP_MANAGER_H 

#include <Arduino.h>

class BeepManager {
  public:
    BeepManager();

    void init(uint8_t pin);
    void beep(unsigned int durationMs);
    void loop();

  private:
    uint8_t _pin;
    bool _initialized;
    unsigned long _beepEndMillis = 0;
    bool _isBeeping = false;
    void _startBeep();
    void _stopBeep();
};

#endif /* BEEP_MANAGER_H */