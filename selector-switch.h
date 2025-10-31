#ifndef SELECTOR_SWITCH_H
#define SELECTOR_SWITCH_H

#include <Arduino.h>
#include <Adafruit_PCF8574.h> // https://github.com/adafruit/Adafruit_PCF8574

#define BUTTON_LONG_PRESS_MS 100
#define STATE_UPDATED_MANUALLY 0

typedef void (*OnPositionChangedCallbackFunction)(int position);

class SelectorSwitch {
  public:
    SelectorSwitch();

    void init(Adafruit_PCF8574 &expander);
    void tick();
    void attachOnPositionChanged(OnPositionChangedCallbackFunction callbackFn);

  private:
    Adafruit_PCF8574 _expander;
    byte _position = 0;
    bool _initialized;

    OneButton _positionButton1; 
    OneButton _positionButton2; 
    OneButton _positionButton3; 

    OnPositionChangedCallbackFunction _onPositionChangedFn = nullptr;
    static void _onPositionSelected(void *ctx);
};

struct OnPositionSelectedContext {
  SelectorSwitch* instance;
  int position;
};

#endif /* SELECTOR_SWITCH_H */