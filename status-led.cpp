#include "status-led.h"

void StatusLed::setPin(uint8_t pin) {
  pinMode(pin, OUTPUT);
  _pin = pin;
}

void StatusLed::turnOff() {
  digitalWrite(_pin, HIGH);
}

void StatusLed::turnOn() {
  digitalWrite(_pin, LOW);
}