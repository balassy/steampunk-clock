// Platform libraries.
#include <Arduino.h>     // To add IntelliSense for platform constants.
#include <time.h>        // Time related functions.

#include "ntp-manager.h"

NTPManager::NTPManager() : _initialized(false) {} 

void NTPManager::init(const char* timezone, const char* server) {
  configTime(timezone, server);
  _initialized = true;
  Serial.println(F("NTPManager::init: DONE."));
}

void NTPManager::getCurrentTime(NTPDateTime &dt) {
  if (!_initialized) {
    Serial.println(F("NTPManager::getCurrentTime: NTP not initialized, cannot get time!"));
    return;
  }
  
  time(&_secondsSinceEpoch);                      // Get current time as seconds since Epoch (1970).
  localtime_r(&_secondsSinceEpoch, &_localTime);  // Convert to local time structure.

  dt.year      = _localTime.tm_year + 1900;    // years since 1900
  dt.month     = _localTime.tm_mon + 1;        // January = 0 (!)
  dt.day       = _localTime.tm_mday;           // day of month
  dt.hour      = _localTime.tm_hour;           // hours since midnight  0-23
  dt.minute    = _localTime.tm_min;            // minutes after the hour  0-59
  dt.second    = _localTime.tm_sec;            // seconds after the minute  0-61*
  dt.dayOfWeek = _localTime.tm_wday;           // days since Sunday 0-6
  dt.isDST     = (_localTime.tm_isdst == 1);   // Daylight Saving Time flag
}