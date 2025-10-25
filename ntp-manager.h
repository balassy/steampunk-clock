#ifndef NTP_MANAGER_H
#define NTP_MANAGER_H

#include <Arduino.h>

struct NTPDateTime {
  int year;
  int month;
  int day;
  int hour;
  int minute;
  int second;
  int dayOfWeek;
  bool isDST;
};

class NTPManager {
  public:
    NTPManager();
    void init(const char* timezone, const char* server);
    void getCurrentTime(NTPDateTime &dt);
    
    static void printDateTime(const NTPDateTime &dt);

  private:
    bool _initialized;
    time_t _secondsSinceEpoch;
    tm _localTime; 
};

#endif /* NTP_MANAGER_H */