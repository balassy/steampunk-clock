// Platform libraries.
#include <Arduino.h>     // To add IntelliSense for platform constants.
#include <Wire.h>        // I2C library required by the RTC module.

// Third-party libraries.
#include "RTClib.h" // RTC library by Adafruit

// Read configuration and secrets.
#include "config.h"

// Project classes.
#include "status-led.h"

StatusLed statusLed;
RTC_DS3231 rtc;

void setup() {
  initSerial();
  initLeds();
  initRTC();

  Serial.println(F("Setup DONE."));
}

void loop() {
  statusLed.turnOn();
  Serial.println(F("LED is ON"));
  delay(1000);


  DateTime now = rtc.now();
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
  Wire.begin(PIN_RTC_SDA, PIN_RTC_SCL);

  if (!rtc.begin()) {
    Serial.println(F("initRTC: Couldn't find RTC"));
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println(F("initRTC: RTC lost power, setting the time!"));
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  } else {
    Serial.println(F("initRTC: RTC power is OK, using previously set RTC time."));
  }

  Serial.println(F("initRTC: Initializing RTC DONE."));
}
