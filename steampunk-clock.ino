// Platform libraries.
#include <Arduino.h>     // To add IntelliSense for platform constants.

// Read configuration and secrets.
#include "config.h"

// Project classes.
#include "status-led.h"
#include "rtc-manager.h"

StatusLed statusLed;
RTCManager rtc;

void setup() {
  initSerial();
  initLeds();
  initRTC();

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
