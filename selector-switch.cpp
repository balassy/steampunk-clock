#include <Arduino.h>
#include <Adafruit_PCF8574.h> // https://github.com/adafruit/Adafruit_PCF8574
#include <OneButton.h>        // https://github.com/mathertel/OneButton

#include "selector-switch.h"

SelectorSwitch::SelectorSwitch() : _initialized(false) {}

void SelectorSwitch::init() {
  Serial.println(F("SelectorSwitch::init: Initializing selector switch..."));

  if (!_expander.begin(0x20, &Wire)) {
    Serial.println(F("SelectorSwitch::init: Couldn't find PCF8574 extender board on the bus, boot halted!"));
    while (1);
  }

  _expander.pinMode(POSITION_BUTTON_1_PIN, INPUT_PULLUP);
  _expander.pinMode(POSITION_BUTTON_2_PIN, INPUT_PULLUP);
  _expander.pinMode(POSITION_BUTTON_3_PIN, INPUT_PULLUP);

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

  bool positionButton1State = (_expander.digitalRead(POSITION_BUTTON_1_PIN) == LOW);
  _positionButton1.tick(positionButton1State);

  bool positionButton2State = (_expander.digitalRead(POSITION_BUTTON_2_PIN) == LOW);
  _positionButton2.tick(positionButton2State);

  bool positionButton3State = (_expander.digitalRead(POSITION_BUTTON_3_PIN) == LOW);
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