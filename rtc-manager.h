#ifndef RTCMANAGER_H
#define RTCMANAGER_H

#include <Arduino.h>
#include <RTClib.h>

class RTCManager {
  public:
    RTCManager();

    void init();

    DateTime getCurrentTime();
    void setCurrentTime(const DateTime &dt);
    float getTemperature();

  private:
    RTC_DS3231 _rtc;
    bool _initialized;
};

#endif /* RTCMANAGER_H */