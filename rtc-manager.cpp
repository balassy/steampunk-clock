// Platform libraries.
#include <Arduino.h>     // To add IntelliSense for platform constants.
#include <Wire.h>        // I2C library required by the RTC module.

// Third-party libraries.
#include "RTClib.h"      // RTC library by Adafruit

#include "rtc-manager.h"

RTCManager::RTCManager() : _initialized(false) {}

void RTCManager::init(int pinSDA, int pinSCL) {
  Wire.begin(pinSDA, pinSCL);

  if (!_rtc.begin()) {
    Serial.println(F("RTCManager::init: Couldn't find RTC"));
    while (1);
  }

  _initialized = true;

  Serial.println(F("RTCManager::init: DONE."));    
}

DateTime RTCManager::getCurrentTime() {
  if (!_initialized) {
    Serial.println(F("RTCManager::getCurrentTime: RTC not initialized, returning default time!"));
    return DateTime("2000-01-01T00:00:00");
  }

  return _rtc.now();
}

void RTCManager::setCurrentTime(const DateTime &newDateTime) {
  if (!_initialized) {
    Serial.println(F("RTCManager::setCurrentTime: RTC not initialized, cannot set time!"));
    return;
  }
  _rtc.adjust(newDateTime);
}

float RTCManager::getTemperature() {
  if (!_initialized) {
      Serial.println(F("RTCManager::getTemperature: RTC not initialized, returning 0.0!"));
      return 0.0f;
  }
  return _rtc.getTemperature();
}

bool RTCManager::isAdjustmentNeeded() {
  if (!_initialized) {
    Serial.println(F("RTCManager::isAdjustmentNeeded: RTC not initialized, returning false!"));
    return false;
  }
  return _rtc.lostPower();
}

void RTCManager::printDateTime(const DateTime &dt) {
  char formattedDateTime[32];
  snprintf(formattedDateTime, sizeof(formattedDateTime), "RTC: %04d.%02d.%02d. %02d:%02d:%02d",
           dt.year(), 
           dt.month(), 
           dt.day(),
           dt.hour(), 
           dt.minute(), 
           dt.second());
  Serial.println(formattedDateTime);
}