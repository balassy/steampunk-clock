// Platform libraries.
#include <Arduino.h>     // To add IntelliSense for platform constants.

// Third-party libraries.
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <OneButton.h>   // https://github.com/mathertel/OneButton

// Read configuration and secrets.
#include "config.h"

// Project classes.
#include "reboot-manager.h"
#include "ntp-manager.h"
#include "rtc-manager.h"
#include "selector-switch.h"
#include "speed-servo.h"
#include "status-led.h"

#define BUTTON_ACTIVE_LOW true

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

  Serial.println(F("setup: DONE."));
}

void loop() {
  // IMPORTANT! Do NOT use delay() in the main loop when using OneButton, as it would prevent the button from being handled!
  settingsButton.tick();
  selectorSwitch.tick();

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

void initLeds() {
  Serial.println(F("initLeds: Initializing LEDs..."));

  statusLed.setPin(PIN_LED, LOW, HIGH);
  
  statusLed.turnOn();
  delay(2000);
  statusLed.turnOff();
  
  hourLed.setPin(PIN_HOUR_LED, HIGH, LOW);
  hourLed.turnOn();
  delay(2000);
  hourLed.turnOff();

  minuteLed.setPin(PIN_MINUTE_LED, HIGH, LOW);
  minuteLed.turnOn();
  delay(2000);
  minuteLed.turnOff();

  Serial.println(F("initLeds: Initializing LEDs DONE."));
}

void initButton() {
  Serial.println(F("initButton: Initializing settings button..."));

  settingsButton.setup(PIN_BUTTON, INPUT_PULLUP, BUTTON_ACTIVE_LOW);
  settingsButton.attachClick(onButtonClicked);
  settingsButton.attachDoubleClick(onButtonDoubleClicked);
  settingsButton.attachLongPressStop(onButtonLongClicked);

  Serial.println(F("initButton: Initializing settings button DONE."));
}

void initSwitch() {
  Serial.println(F("setup: Initializing selector switch..."));
  selectorSwitch.init(); 

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
  delay(500);
  setHour(12);
  delay(500);
  setHour(0);
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
  hour = constrain(hour, 0, 23);

  // Map 0-11 hours to 0-180 degrees, inverting the direction (leftmost position is 0 hour).
  int position = map(hour % 12, 0, 11, 180, 0);
  hourServo.moveTo(position);
}

void setMinute(int minute) {
  minute = constrain(minute, 0, 59);

  // Map 0-59 minutes to 0-180 degrees, inverting the direction (leftmost position is 0 minute).
  int position = map(minute, 0, 59, 180, 0);
  minuteServo.moveTo(position);
}

void convertNtpDateTimeToRtcDateTime(const NTPDateTime &ntpDt, DateTime &rtcDt) {
  rtcDt = DateTime(ntpDt.year, ntpDt.month, ntpDt.day, ntpDt.hour, ntpDt.minute, ntpDt.second);
} 




