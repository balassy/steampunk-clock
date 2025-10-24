// Platform libraries.
#include <Arduino.h>     // To add IntelliSense for platform constants.

// Read configuration and secrets.
#include "config.h"

// Project classes.
#include "status-led.h"

StatusLed statusLed;

void setup() {
  initSerial();
  initLed();

  Serial.println(F("Setup DONE."));
}

void loop() {
  statusLed.turnOn();
  Serial.println(F("LED is ON"));
  delay(1000);
  statusLed.turnOff();
  Serial.println(F("LED is OFF"));
  delay(1000);
}

void initSerial() {
  Serial.begin(SERIAL_BAUD_RATE);
  Serial.println();
  Serial.println(F("Initializing serial connection DONE."));
}

void initLed() {
  statusLed.setPin(PIN_LED);
  
  // Turn on the LED for diagnostics.
  statusLed.turnOn();
  delay(3000);
  statusLed.turnOff();
}
