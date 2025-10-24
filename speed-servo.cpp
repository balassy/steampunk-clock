// Platform libraries.
#include <Arduino.h>     // To add IntelliSense for platform constants.

#include "speed-servo.h"

const int MOVE_STEP_DELAY_MSEC = 2;

void SpeedServo::setPin(uint8_t pin) {
  _servo.attach(pin, 700, 2300);

  // Give the servo a small moment to respond after attaching.
  delay(10);  
}

void SpeedServo::moveTo(int newPosition) {
  newPosition = constrain(newPosition, 0, 180);

  // Check if the servo is properly initialized.
  if (!_servo.attached()) {
    Serial.println(F("SpeedServo::moveTo: Servo cannot move, because it is not attached!"));
    return;
  }

  // Get current servo position (0-180).
  int currentPosition = _servo.read();

  Serial.println(F("SpeedServo::moveTo: Moving servo from position ") + String(currentPosition) +
                 F(" to position ") + String(newPosition));

  // Unknown current position -> write target directly
  if (currentPosition < 0 || currentPosition > 180) {
    _servo.write(newPosition);
    return;
  }

  // Do not move if already in position.
  if (currentPosition == newPosition) {
    return;
  }

  // Determine moving direction.
  int step = (newPosition > currentPosition) ? 1 : -1;

  for (int pos = currentPosition; pos != newPosition; pos += step) {
    _servo.write(pos);
    delay(MOVE_STEP_DELAY_MSEC);
  }

  // Ensure final position exactly set.
  _servo.write(newPosition);
}