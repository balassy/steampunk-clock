// Platform libraries.
#include <Arduino.h>     // To add IntelliSense for platform constants.
#include <time.h>        // Time related functions.

#include "ntp-manager.h"

NTPManager::NTPManager() : _initialized(false) {} 

void NTPManager::init(const char* timezone, const char* server) {
  Serial.println(F("NTPManager::init: Initializing NTP..."));
  configTime(timezone, server);
  
  // Wait until time is correctly returned.
  time_t now;
  while ((now = time(nullptr)) < 100000) {  // roughly < 1973
    Serial.println(F("NTPManager::init: Waiting 1 second for NTP time to be ready..."));
    delay(1000);
  }

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

void NTPManager::printDateTime(const NTPDateTime &dt) {
  char formattedDateTime[32];
  snprintf(formattedDateTime, sizeof(formattedDateTime), "NTP: %04d.%02d.%02d. %02d:%02d:%02d%s",
           dt.year, 
           dt.month, 
           dt.day,
           dt.hour, 
           dt.minute, 
           dt.second,
           dt.isDST ? " DST" : "");
  Serial.println(formattedDateTime);
}