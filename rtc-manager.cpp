// Platform libraries.
#include <Arduino.h>     // To add IntelliSense for platform constants.
#include <Wire.h>        // I2C library required by the RTC module.

// Third-party libraries.
#include "RTClib.h"      // RTC library by Adafruit

// Read configuration and secrets.
#include "config.h"

#include "rtc-manager.h"

RTCManager::RTCManager() : _initialized(false) {}

void RTCManager::init() {
  Wire.begin(PIN_RTC_SDA, PIN_RTC_SCL);

  if (!_rtc.begin()) {
    Serial.println(F("RTCManager::init: Couldn't find RTC"));
    while (1);
  }

  if (_rtc.lostPower()) {
    Serial.print(F("RTCManager::init: RTC lost power, setting the time to the compile date/time: "));
    Serial.println(__DATE__ " " __TIME__);

    _rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  } else {
    Serial.println(F("RTCManager::init: RTC power is OK, using previously set RTC time."));
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