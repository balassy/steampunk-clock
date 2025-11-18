#include <Arduino.h>

#include "config.h"
#include "beep-manager.h"

BeepManager::BeepManager() : _initialized(false) {}

void BeepManager::init(uint8_t pin) {
  Serial.println(F("BeepManager::init: Initializing buzzer..."));

  _pin = pin;
  pinMode(_pin, OUTPUT);
  
  _initialized = true;

  Serial.println(F("BeepManager::init: Initializing buzzer DONE."));
}

void BeepManager::beep(unsigned int durationMs) {
  if(!_initialized) {
    Serial.println(F("BeepManager::beep: Not initialized!"));
    return;
  }

  Serial.print(F("BeepManager::beep: Beeping for "));
  Serial.print(durationMs);
  Serial.println(F(" msec."));

  _beepEndMillis = millis() + durationMs;
  _startBeep();
}

void BeepManager::loop() {
  if(!_initialized) {
    Serial.println(F("BeepManager::loop: Not initialized!"));
    return;
  }

  if(_isBeeping && millis() >= _beepEndMillis) {
    _stopBeep();
  }
}

void BeepManager::_startBeep() {
  Serial.println(F("BeepManager::_startBeep: Starting beep."));
  digitalWrite(_pin, HIGH);
  _isBeeping = true;
}

void BeepManager::_stopBeep() {
  Serial.println(F("BeepManager::_stopBeep: Stopping beep."));
  digitalWrite(_pin, LOW);
  _isBeeping = false;
}

