// Platform libraries.
#include <Arduino.h>     // To add IntelliSense for platform constants.

// Third-party libraries.
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

// Read configuration and secrets.
#include "config.h"

// Project classes.
#include "ntp-manager.h"
#include "rtc-manager.h"
#include "speed-servo.h"
#include "status-led.h"

NTPManager ntpManager;
SpeedServo hourServo;
SpeedServo minuteServo;
StatusLed statusLed;
StatusLed hourLed;
StatusLed minuteLed;
RTCManager rtc;

char formattedDateTime[32];

void setup() {
  initSerial();
  initLeds();
  initRTC();
  initServos();

  Serial.println(F("setup: DONE."));
}

void loop() {
  statusLed.toggle();

  DateTime now = rtc.getCurrentTime();
  printDateTime(now);

  Serial.print(rtc.getTemperature());
  Serial.println("ºC");

  setHour(now.hour());
  setMinute(now.minute());

  delay(1000);
}

void initSerial() {
  Serial.begin(SERIAL_BAUD_RATE);
  Serial.println();
  Serial.println(F("initSerial: Initializing serial connection DONE."));
}

void initLeds() {
  statusLed.setPin(PIN_LED, LOW, HIGH);
  
  statusLed.turnOn();
  delay(2000);
  statusLed.turnOff();
  
  hourLed.setPin(PIN_HOUR_LED, HIGH, LOW);
  hourLed.turnOn();
  delay(2000);
  hourLed.turnOff();

  minuteLed.setPin(PIN_MINUTE_LED, HIGH, LOW);
  minuteLed.turnOn();
  delay(2000);
  minuteLed.turnOff();

  Serial.println(F("initLeds: Initializing LEDs DONE."));
}

void initNetwork() {
  Serial.println(F("initNetwork: Connecting to network..."));
  
  WiFiManager wifiManager;
  bool isConnected;
  isConnected = wifiManager.autoConnect(WIFI_AP_SSID, WIFI_AP_PASSWORD);

  if(!isConnected) {
    Serial.println("initNetwork: Failed to connect, restarting...");
    ESP.restart();
  } 
  else { 
    Serial.println("initNetwork: Connected successfully to " + String(WIFI_AP_SSID));
  }

  Serial.println(F("initNetwork: Initializing network DONE."));
}

void initNTP() {
  Serial.println(F("initNTP: Connecting to NTP server..."));
  ntpManager.init(NTP_TIMEZONE, NTP_SERVER);
  Serial.println(F("initNTP: Initializing NTP DONE."));
}

void initRTC() {
  rtc.init(PIN_RTC_SDA, PIN_RTC_SCL);

  if(rtc.isAdjustmentNeeded()) {
    Serial.println(F("initRTC: RTC adjustment is needed, setting the time."));

    initNetwork();
    initNTP();

    Serial.println(F("initRTC: Fetching current NTP time... "));
    NTPDateTime ntpTime;
    ntpManager.getCurrentTime(ntpTime);

    Serial.print(F("initRTC: NTP time is: "));
    printDateTime(ntpTime);

    Serial.print(F("initRTC: Setting RTC time to NTP time: "));
    DateTime initialDateTime;
    convertNtpDateTimeToRtcDateTime(ntpTime, initialDateTime);
    printDateTime(initialDateTime);
    rtc.setCurrentTime(initialDateTime);
  } else {
    Serial.print(F("initRTC: RTC adjustment is NOT needed, keeping the previously set time: "));
    DateTime currentRtcTime = rtc.getCurrentTime();
    printDateTime(currentRtcTime);
  }
  Serial.println(F("initRTC: Initializing RTC DONE."));
}

void initServos() {
  Serial.println(F("initServos: Initializing hour servo..."));
  hourLed.turnOn();
  hourServo.setPin(PIN_SERVO_HOUR);
  setHour(23);
  delay(500);
  setHour(12);
  delay(500);
  setHour(0);
  hourLed.turnOff();
 
  Serial.println(F("initServos: Initializing minute servo..."));
  minuteLed.turnOn();
  minuteServo.setPin(PIN_SERVO_MINUTE);
  setMinute(59);
  delay(500);
  setMinute(30);
  delay(500); 
  setMinute(0);
  minuteLed.turnOff(); 

  Serial.println(F("initServos: Initializing servos DONE."));
}

void setHour(int hour) {
  hour = constrain(hour, 0, 23);

  // Map 0-11 hours to 0-180 degrees, inverting the direction (leftmost position is 0 hour).
  int position = map(hour % 12, 0, 11, 180, 0);
  hourServo.moveTo(position);
}

void setMinute(int minute) {
  minute = constrain(minute, 0, 59);

  // Map 0-59 minutes to 0-180 degrees, inverting the direction (leftmost position is 0 minute).
  int position = map(minute, 0, 59, 180, 0);
  minuteServo.moveTo(position);
}

void convertNtpDateTimeToRtcDateTime(const NTPDateTime &ntpDt, DateTime &rtcDt) {
  rtcDt = DateTime(ntpDt.year, ntpDt.month, ntpDt.day, ntpDt.hour, ntpDt.minute, ntpDt.second);
} 

void printDateTime(const NTPDateTime &dt) {
  snprintf(formattedDateTime, sizeof(formattedDateTime), "NTP: %04d.%02d.%02d. %02d:%02d:%02d %s",
           dt.year, 
           dt.month, 
           dt.day,
           dt.hour, 
           dt.minute, 
           dt.second,
           dt.isDST ? " DST" : "");
  Serial.println(formattedDateTime);
}

void printDateTime(const DateTime &dt) {
  snprintf(formattedDateTime, sizeof(formattedDateTime), "RTC: %04d.%02d.%02d. %02d:%02d:%02d",
           dt.year(), 
           dt.month(), 
           dt.day(),
           dt.hour(), 
           dt.minute(), 
           dt.second());
  Serial.println(formattedDateTime);
}
