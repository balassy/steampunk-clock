#include <Arduino.h>           // To add IntelliSense for platform constants.
#include <Servo.h>             // To control the SG90 servo motors.

#ifndef SPEED_SERVO_H
#define SPEED_SERVO_H

class SpeedServo {
  public:
    void setPin(uint8_t pin);
    void moveTo(int position);

  private:
    Servo _servo;
};

#endif /* SPEED_SERVO_H */