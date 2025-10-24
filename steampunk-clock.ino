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

void setup() {
  initSerial();
  initLeds();
  initNetwork();
  initNTP();
  initRTC();
  initServos();

  Serial.println(F("setup: DONE."));
}

void loop() {
  statusLed.toggle();

  DateTime now = rtc.getCurrentTime();
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.println(now.second(), DEC);
  Serial.print(rtc.getTemperature());
  Serial.println("ºC");

  setHour(now.hour());
  setMinute(now.minute());

  //ntpManager.showTime();
  NTPDateTime ntpTime;
  ntpManager.getCurrentTime(ntpTime);
  Serial.print("NTP time: ");
  Serial.print(ntpTime.year);
  Serial.print('/');
  Serial.print(ntpTime.month);
  Serial.print('/');
  Serial.print(ntpTime.day);
  Serial.print(" ");
  Serial.print(ntpTime.hour);
  Serial.print(':');
  Serial.print(ntpTime.minute);
  Serial.print(':');
  Serial.print(ntpTime.second);
  if (ntpTime.isDST)
    Serial.println("DST");
  else
    Serial.println("standard");

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

  Serial.println(F("initSerial: Initializing LEDs DONE."));
}

void initNetwork() {
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
  ntpManager.init(NTP_TIMEZONE, NTP_SERVER);
  Serial.println(F("initNTP: Initializing NTP DONE."));
}

void initRTC() {
  DateTime initialDateTime;

  // DateTime(F(__DATE__), F(__TIME__))
  delay(3000); // Wait a bit to ensure NTP time is ready.
  NTPDateTime ntpTime;
  ntpManager.getCurrentTime(ntpTime);

  Serial.print(F("initRTC: NTP date/time is: "));
  printDateTime(ntpTime);

  convertNtpDateTimeToRtcDateTime(ntpTime, initialDateTime);
  Serial.print(F("initRTC: Initial date/time for RTC is: "));
  printDateTime(initialDateTime);

  rtc.init(PIN_RTC_SDA, PIN_RTC_SCL, initialDateTime);
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

  // Map 0-23 hours to 0-180 degrees, inverting the direction (leftmost position is 0 hour).
  int position = map(hour, 0, 23, 180, 0);
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
  Serial.print(dt.year);
  Serial.print('/');
  Serial.print(dt.month);
  Serial.print('/');
  Serial.print(dt.day);
  Serial.print(" ");
  Serial.print(dt.hour);
  Serial.print(':');
  Serial.print(dt.minute);
  Serial.print(':');
  Serial.println(dt.second);
}

void printDateTime(const DateTime &dt) {
  Serial.print(dt.year(), DEC);
  Serial.print('/');
  Serial.print(dt.month(), DEC);
  Serial.print('/');
  Serial.print(dt.day(), DEC);
  Serial.print(" ");
  Serial.print(dt.hour(), DEC);
  Serial.print(':');
  Serial.print(dt.minute(), DEC);
  Serial.print(':');
  Serial.println(dt.second(), DEC);
}
