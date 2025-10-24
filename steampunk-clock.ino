// Platform libraries.
#include <Arduino.h>     // To add IntelliSense for platform constants.

// Read configuration and secrets.
#include "config.h"

// Project classes.
#include "rtc-manager.h"
#include "speed-servo.h"
#include "status-led.h"

SpeedServo hourServo;
SpeedServo minuteServo;
StatusLed statusLed;
RTCManager rtc;

void setup() {
  initSerial();
  initLeds();
  initRTC();
  initServos();

  Serial.println(F("setup: DONE."));
}

void loop() {
  statusLed.turnOn();
  Serial.println(F("LED is ON"));
  delay(1000);


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

  setHour(now.hour());
  setMinute(now.minute());

  Serial.print(rtc.getTemperature());
  Serial.println("ºC");

  delay(1000);

  statusLed.turnOff();
  Serial.println(F("LED is OFF"));
  delay(1000);
}

void initSerial() {
  Serial.begin(SERIAL_BAUD_RATE);
  Serial.println();
  Serial.println(F("initSerial: Initializing serial connection DONE."));
}

void initLeds() {
  statusLed.setPin(PIN_LED);
  
  // Turn on the LED for diagnostics.
  statusLed.turnOn();
  delay(3000);
  statusLed.turnOff();

  Serial.println(F("initSerial: Initializing LEDs DONE."));
}

void initRTC() {
  rtc.init();
  Serial.println(F("initRTC: Initializing RTC DONE."));
}

void initServos() {
  Serial.println(F("initServos: Initializing hour servo..."));
  hourServo.setPin(PIN_SERVO_HOUR);
  setHour(23);
  setHour(0);
 
  Serial.println(F("initServos: Initializing minute servo..."));
  minuteServo.setPin(PIN_SERVO_MINUTE);
  setMinute(59);
  setMinute(0);

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