// Platform libraries.
#include <Arduino.h>     // To add IntelliSense for platform constants.

// Third-party libraries.
#include <WiFiManager.h>      // https://github.com/tzapu/WiFiManager
#include <OneButton.h>        // https://github.com/mathertel/OneButton
#include <Adafruit_PCF8574.h> // https://github.com/adafruit/Adafruit_PCF8574
#include <Wire.h>             // I2C library

// Read configuration and secrets.
#include "config.h"

// Project classes.
#include "beep-manager.h"
#include "reboot-manager.h"
#include "ntp-manager.h"
#include "rtc-manager.h"
#include "selector-switch.h"
#include "speed-servo.h"
#include "status-led.h"

#define BUTTON_ACTIVE_LOW true

Adafruit_PCF8574 expander;
BeepManager beepManager;
NTPManager ntpManager;
SelectorSwitch selectorSwitch;
SpeedServo hourServo;
SpeedServo minuteServo;
StatusLed statusLed;
StatusLed hourLed;
StatusLed minuteLed;
RTCManager rtc;
OneButton settingsButton;

unsigned long lastClockUpdateMsec = 0;

enum Mode {
  ALL_LIGHTS_OFF = 0,
  BACKLIGHTS_ONLY,
  AMBIENT_LIGHTS_ONLY,
  ALL_LIGHTS_ON
};

void setup() {
  initSerial();
  initExpander();
  initLeds();
  initButton();
  initSwitch();
  
  if (RebootManager::isReset())
  {
    Serial.println(F("setup: Detected reset."));
  } else {
    Serial.println(F("setup: Detected power-up boot."));
  }
  RebootManager::markSetupComplete();
  
  initRTC();
  initServos();
  initBuzzer();

  Serial.println(F("setup: DONE."));
}

void loop() {
  // IMPORTANT! Do NOT use delay() in the main loop when using OneButton, as it would prevent the button from being handled!
  bool settingsButtonState = (expander.digitalRead(EXPANDER_PIN_SETTINGS_BUTTON) == LOW);
  settingsButton.tick(settingsButtonState);

  selectorSwitch.tick();

  beepManager.loop();

  // Manually detecting the number of milliseconds passed to avoid using delay() which would block button handling.
  unsigned long currentMsec = millis();
  if(currentMsec - lastClockUpdateMsec >= CLOCK_UPDATE_INTERVAL_MSEC) {
    lastClockUpdateMsec = currentMsec;
    updateClock();
  }
}

void initSerial() {
  Serial.begin(SERIAL_BAUD_RATE);
  Serial.println();
  Serial.println(F("initSerial: Initializing serial connection DONE."));
}

void initExpander() {
  Serial.println(F("initExpander: Initializing I2C expander..."));

  if (!expander.begin(0x20, &Wire)) {
    Serial.println(F("initExpander: Couldn't find PCF8574 extender board on the bus, boot halted!"));
    while (1);
  }

  Serial.println(F("initExpander: Initializing I2C expander DONE."));
}

void initLeds() {
  Serial.println(F("initLeds: Initializing LEDs..."));

  statusLed.setPin(PIN_LED, LOW, HIGH);
  
  statusLed.turnOn();
  delay(500);
  statusLed.turnOff();
  
  hourLed.setPin(PIN_HOUR_LED, HIGH, LOW);
  hourLed.turnOn();
  delay(500);
  hourLed.turnOff();

  minuteLed.setPin(PIN_MINUTE_LED, HIGH, LOW);
  minuteLed.turnOn();
  delay(500);
  minuteLed.turnOff();

  Serial.println(F("initLeds: Initializing LEDs DONE."));
}

void initBuzzer() {
  Serial.println(F("initBuzzer: Initializing buzzer..."));
  beepManager.init(PIN_BUZZER);
  Serial.println(F("initBuzzer: Initializing buzzer DONE."));
}

void initButton() {
  Serial.println(F("initButton: Initializing settings button..."));

  expander.pinMode(EXPANDER_PIN_SETTINGS_BUTTON, INPUT_PULLUP);

  settingsButton.setup(STATE_UPDATED_MANUALLY, BUTTON_ACTIVE_LOW);
  settingsButton.attachClick(onButtonClicked);
  settingsButton.attachDoubleClick(onButtonDoubleClicked);
  settingsButton.attachLongPressStop(onButtonLongClicked);

  Serial.println(F("initButton: Initializing settings button DONE."));
}

void initSwitch() {
  Serial.println(F("setup: Initializing selector switch..."));
  selectorSwitch.init(expander); 

  selectorSwitch.attachOnPositionChanged([](int position){
    Serial.print(F("Selector switch position changed to: "));
    Serial.println(position);
    onModeChanged(static_cast<Mode>(position));
  });

  Serial.println(F("setup: Initializing selector switch DONE."));
}

void initNetwork() {
  Serial.println(F("initNetwork: Initializing network..."));
  
  WiFiManager wifiManager;
  bool isConnected;
  isConnected = wifiManager.autoConnect(WIFI_AP_SSID, WIFI_AP_PASSWORD);

  if(!isConnected) {
    Serial.println("initNetwork: Failed to connect, restarting...");
    ESP.restart();
  } 
  else { 
    Serial.println("initNetwork: Connected successfully to " + String(WIFI_AP_SSID));
  }

  Serial.println(F("initNetwork: Initializing network DONE."));
}

void initNTP() {
  Serial.println(F("initNTP: Connecting to NTP server..."));
  ntpManager.init(NTP_TIMEZONE, NTP_SERVER);
  Serial.println(F("initNTP: Initializing NTP DONE."));
}

void initRTC() {
  rtc.init(PIN_RTC_SDA, PIN_RTC_SCL);

  if(rtc.isAdjustmentNeeded()) {
    Serial.println(F("initRTC: RTC adjustment is needed, setting the time."));

    initNetwork();
    initNTP();

    Serial.println(F("initRTC: Fetching current NTP time... "));
    NTPDateTime ntpTime;
    ntpManager.getCurrentTime(ntpTime);

    Serial.print(F("initRTC: NTP time is: "));
    NTPManager::printDateTime(ntpTime);

    Serial.print(F("initRTC: Setting RTC time to NTP time: "));
    DateTime initialDateTime;
    convertNtpDateTimeToRtcDateTime(ntpTime, initialDateTime);
    RTCManager::printDateTime(initialDateTime);
    rtc.setCurrentTime(initialDateTime);
  } else {
    Serial.print(F("initRTC: RTC adjustment is NOT needed, keeping the previously set time: "));
    DateTime currentRtcTime = rtc.getCurrentTime();
    RTCManager::printDateTime(currentRtcTime);
  }
  Serial.println(F("initRTC: Initializing RTC DONE."));
}

void initServos() {
  Serial.println(F("initServos: Initializing hour servo..."));
  hourLed.turnOn();
  hourServo.setPin(PIN_SERVO_HOUR);
  setHour(23);
  delay(2000);
  setHour(0);
  delay(2000);
  setHour(5);
  delay(2000);

  for (int h = 0; h < 12; h++) {
    setHour(h);
    delay(500);
  }
  hourLed.turnOff();
 
  Serial.println(F("initServos: Initializing minute servo..."));
  minuteLed.turnOn();
  minuteServo.setPin(PIN_SERVO_MINUTE);
  setMinute(59);
  delay(500);
  setMinute(30);
  delay(500); 
  setMinute(0);
  minuteLed.turnOff(); 

  Serial.println(F("initServos: Initializing servos DONE."));
}


void updateClock() {
  Serial.println(F("updateClock: Updating clock..."));

  statusLed.toggle();

  DateTime now = rtc.getCurrentTime();
  RTCManager::printDateTime(now);

  setHour(now.hour());
  setMinute(now.minute());

  Serial.println(F("updateClock: Updating clock DONE."));
}

static void onButtonClicked() {
  Serial.println("onButtonClicked: Single clicked detected!");
}

static void onButtonDoubleClicked() {
  Serial.println("onButtonDoubleClicked: Double clicked detected!");
}

static void onButtonLongClicked() {
  Serial.println("onButtonLongClicked: Long clicked detected!");
  beepManager.beep(100);
}

static void onModeChanged(Mode newMode) {
  //TODO: Fill in the implementation to change lighting based on mode.
  switch (newMode)
  {
    case ALL_LIGHTS_OFF:
      Serial.println("onModeChanged: ALL_LIGHTS_OFF selected.");
      hourLed.turnOff();
      minuteLed.turnOff();
      break;
    
    case BACKLIGHTS_ONLY:
      Serial.println("onModeChanged: BACKLIGHTS_ONLY selected.");
      hourLed.turnOn();
      minuteLed.turnOff();
      break;

    case AMBIENT_LIGHTS_ONLY:
      Serial.println("onModeChanged: AMBIENT_LIGHTS_ONLY selected.");
      hourLed.turnOff();
      minuteLed.turnOn();
      break;

    case ALL_LIGHTS_ON:
      Serial.println("onModeChanged: ALL_LIGHTS_ON selected.");
      hourLed.turnOn();
      minuteLed.turnOn();
      break;

    default:
      break;
  }
}

void setHour(int hour) {
  Serial.print(F("setHour: Setting hour to "));
  Serial.println(hour);

  hour = constrain(hour, 0, 23);

  // The full display is about 97 degree.
  // Map 0-11 hours to 41-159 degrees, inverting the direction (leftmost position is 0 hour).
  int position = map(hour % 12, 0, 11, 159, 41);
  hourServo.moveTo(position);
}

void setMinute(int minute) {
  Serial.print(F("setMinute: Setting minute to "));
  Serial.println(minute);

  minute = constrain(minute, 0, 59);

  // Map 0-59 minutes to 0-180 degrees, inverting the direction (leftmost position is 0 minute).
  int position = map(minute, 0, 59, 180, 0);
  minuteServo.moveTo(position);
}

void convertNtpDateTimeToRtcDateTime(const NTPDateTime &ntpDt, DateTime &rtcDt) {
  rtcDt = DateTime(ntpDt.year, ntpDt.month, ntpDt.day, ntpDt.hour, ntpDt.minute, ntpDt.second);
} 




