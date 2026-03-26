#include <Arduino.h>
#include <OneButton.h>        // https://github.com/mathertel/OneButton

#include "config.h"
#include "selector-switch.h"

SelectorSwitch::SelectorSwitch() : _initialized(false) {}

void SelectorSwitch::init() {
  Serial.println(F("SelectorSwitch::init: Initializing selector switch..."));

  _positionButton1.setup(PIN_SWITCH_MODE_1, INPUT_PULLUP, BUTTON_ACTIVE_LOW);
  _positionButton1.setPressMs(BUTTON_LONG_PRESS_MS);
  _positionButton1.attachLongPressStart(_onPositionSelected, new OnPositionSelectedContext{this, 1});
  _positionButton1.attachLongPressStop(_onPositionSelected, new OnPositionSelectedContext{this, 0});

  _positionButton2.setup(PIN_SWITCH_MODE_2, INPUT_PULLUP, BUTTON_ACTIVE_LOW);
  _positionButton2.setPressMs(BUTTON_LONG_PRESS_MS);
  _positionButton2.attachLongPressStart(_onPositionSelected, new OnPositionSelectedContext{this, 2}); 
  _positionButton2.attachLongPressStop(_onPositionSelected, new OnPositionSelectedContext{this, 0});

  _positionButton3.setup(PIN_SWITCH_MODE_3, INPUT_PULLUP, BUTTON_ACTIVE_LOW);
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

  _positionButton1.tick();
  _positionButton2.tick();
  _positionButton3.tick();
}

void SelectorSwitch::attachOnPositionChanged(OnPositionChangedCallbackFunction callbackFn) {
  _onPositionChangedFn = callbackFn;
}

void SelectorSwitch::_onPositionSelected(void *ctx) {
  Serial.print("SelectorSwitch::_onPositionSelected, position: ");
  OnPositionSelectedContext *params = static_cast<OnPositionSelectedContext *>(ctx);
  SelectorSwitch* self = params->instance;
  int position = params->position;
  Serial.println(position);

  self->_position = position;

  if (self->_onPositionChangedFn != nullptr) {
    self->_onPositionChangedFn(position);
  }
}