#include <Arduino.h>           // To add IntelliSense for platform constants.

#ifdef ARDUINO_ARCH_ESP8266 
  #include <Servo.h>           // To control the SG90 servo motors with ESP8266.
#endif
#ifdef ARDUINO_ARCH_ESP32
  #include <ESP32Servo.h>      // To control the SG90 servo motors with ESP32.
#endif

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