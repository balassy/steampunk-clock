#include <Arduino.h>
#include <Adafruit_PCF8574.h> // https://github.com/adafruit/Adafruit_PCF8574
#include <OneButton.h>        // https://github.com/mathertel/OneButton

#include "config.h"
#include "selector-switch.h"

SelectorSwitch::SelectorSwitch() : _initialized(false) {}

void SelectorSwitch::init(Adafruit_PCF8574 &expander) {
  Serial.println(F("SelectorSwitch::init: Initializing selector switch..."));

  _expander = expander; 
  _expander.pinMode(EXPANDER_PIN_POSITION_BUTTON_1, INPUT_PULLUP);
  _expander.pinMode(EXPANDER_PIN_POSITION_BUTTON_2, INPUT_PULLUP);
  _expander.pinMode(EXPANDER_PIN_POSITION_BUTTON_3, INPUT_PULLUP);

  _positionButton1.setup(STATE_UPDATED_MANUALLY, true);
  _positionButton1.setPressMs(BUTTON_LONG_PRESS_MS);
  _positionButton1.attachLongPressStart(_onPositionSelected, new OnPositionSelectedContext{this, 1});
  _positionButton1.attachLongPressStop(_onPositionSelected, new OnPositionSelectedContext{this, 0});

  _positionButton2.setup(STATE_UPDATED_MANUALLY, true);
  _positionButton2.setPressMs(BUTTON_LONG_PRESS_MS);
  _positionButton2.attachLongPressStart(_onPositionSelected, new OnPositionSelectedContext{this, 2}); 
  _positionButton2.attachLongPressStop(_onPositionSelected, new OnPositionSelectedContext{this, 0});

  _positionButton3.setup(STATE_UPDATED_MANUALLY, true);
  _positionButton3.setPressMs(BUTTON_LONG_PRESS_MS);
  _positionButton3.attachLongPressStart(_onPositionSelected, new OnPositionSelectedContext{this, 3}); 
  _positionButton3.attachLongPressStop(_onPositionSelected, new OnPositionSelectedContext{this, 0});

  _initialized = true;

  Serial.println(F("SelectorSwitch::init: Initializing selector switch DONE."));
}

void SelectorSwitch::tick() {
  if(!_initialized) {
    Serial.println(F("SelectorSwitch::tick: Not initialized!"));
    return;
  }

  bool positionButton1State = (_expander.digitalRead(EXPANDER_PIN_POSITION_BUTTON_1) == LOW);
  _positionButton1.tick(positionButton1State);

  bool positionButton2State = (_expander.digitalRead(EXPANDER_PIN_POSITION_BUTTON_2) == LOW);
  _positionButton2.tick(positionButton2State);

  bool positionButton3State = (_expander.digitalRead(EXPANDER_PIN_POSITION_BUTTON_3) == LOW);
  _positionButton3.tick(positionButton3State);
}

void SelectorSwitch::attachOnPositionChanged(OnPositionChangedCallbackFunction callbackFn) {
  _onPositionChangedFn = callbackFn;
}

void SelectorSwitch::_onPositionSelected(void *ctx) {
  Serial.println("SelectorSwitch::_onPositionSelected");
  OnPositionSelectedContext *params = static_cast<OnPositionSelectedContext *>(ctx);
  SelectorSwitch* self = params->instance;
  int position = params->position;

  self->_position = position;

  if (self->_onPositionChangedFn != nullptr) {
    self->_onPositionChangedFn(position);
  }
}