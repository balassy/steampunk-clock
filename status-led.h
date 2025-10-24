#include <Arduino.h>

#ifndef status_led_h
#define status_led_h

class StatusLed {
  public:
    void setPin(uint8_t pin, uint8_t onState = HIGH, uint8_t offState = HIGH);
    void turnOn();
    void turnOff();
    void toggle();

  private:
    uint8_t _pin;
    uint8_t _onState;
    uint8_t _offState;
    uint8_t _lastState;
};

#endif /* status_led_h */