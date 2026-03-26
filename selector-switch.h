#ifndef SELECTOR_SWITCH_H
#define SELECTOR_SWITCH_H

#include <Arduino.h>

#define BUTTON_LONG_PRESS_MS 100
#define BUTTON_ACTIVE_LOW true

typedef void (*OnPositionChangedCallbackFunction)(int position);

class SelectorSwitch {
  public:
    SelectorSwitch();

    void init();
    void tick();
    void attachOnPositionChanged(OnPositionChangedCallbackFunction callbackFn);

  private:
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