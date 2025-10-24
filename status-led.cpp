#include "status-led.h"

void StatusLed::setPin(uint8_t pin, uint8_t onState, uint8_t offState) {
  pinMode(pin, OUTPUT);
  _pin = pin;
  _onState = onState;
  _offState = offState;
}

void StatusLed::turnOff() {
  digitalWrite(_pin, _offState);
  _lastState = _offState;
}

void StatusLed::turnOn() {
  digitalWrite(_pin, _onState);
  _lastState = _onState;
}

void StatusLed::toggle() {
  if (_lastState == _onState) {
    turnOff();
  } else {
    turnOn();
  }
}