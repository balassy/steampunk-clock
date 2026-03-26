// Platform libraries.
#include <Arduino.h>        // To add IntelliSense for platform constants.

#ifdef ARDUINO_ARCH_ESP8266
  #include <user_interface.h>      // To access RTC memory functions.
  #define RTC_MEMORY_ADDRESS 64    // Arbitrary RTC memory address (0–127 safe range).
#endif
#ifdef ARDUINO_ARCH_ESP32
  #include <Preferences.h>
  #define PREFS_NAMESPACE_NAME "reboot_manager"
  #define PREFS_KEY_NAME "reboot_marker"
  #define READ_ONLY true
  #define READ_WRITE false
  static Preferences preferences;
#endif

#include "reboot-manager.h"

#define MARKER_VALUE 0xACCE55ED  // Arbitrary marker value to indicate setup completion.

bool RebootManager::isReset() {
  BootReason reason = _getBootReason();
  bool isReset = (reason == BOOT_REASON_RESET);

  Serial.print("RebootManager: Reason: ");
  Serial.println(isReset ? "Reset" : "Not reset");

  return isReset;
}

void RebootManager::markSetupComplete() {
  Serial.println("RebootManager: Marking setup as complete.");
  _saveMarker();
}

BootReason RebootManager::_getBootReason() {
  String rawReason = _getResetReasonString();
  Serial.print("RebootManager: Raw reset reason: ");
  Serial.println(rawReason);

  bool markerExists = _hasMarker();
  Serial.print("RebootManager: Setup completion marker exists: ");
  Serial.println(markerExists ? "Yes" : "No");

  BootReason result;

  if (!markerExists) {
    result = BOOT_REASON_POWER_LOSS;
  } else if (rawReason.indexOf("Software") >= 0) {
    result = BOOT_REASON_SOFTWARE_RESTART;
  } else if (rawReason.indexOf("Deep-Sleep") >= 0) {
    result = BOOT_REASON_DEEP_SLEEP;
  } else if (rawReason.indexOf("External") >= 0) {
    result = BOOT_REASON_RESET;
  } else {
    result = BOOT_REASON_UNKNOWN;
  }

  _printBootReason(result);

  return result;
}

String RebootManager::_getResetReasonString() {
#ifdef ARDUINO_ARCH_ESP8266
  return ESP.getResetReason();
#endif
#ifdef ARDUINO_ARCH_ESP32
  esp_reset_reason_t reason = esp_reset_reason();
  switch (reason) {
    case ESP_RST_POWERON:    return "Power On";
    case ESP_RST_EXT:        return "External System";
    case ESP_RST_SW:         return "Software/System restart";
    case ESP_RST_PANIC:      return "Exception";
    case ESP_RST_INT_WDT:    return "Hardware Watchdog";
    case ESP_RST_TASK_WDT:   return "Software Watchdog";
    case ESP_RST_WDT:        return "Hardware Watchdog";
    case ESP_RST_DEEPSLEEP:  return "Deep-Sleep Wake";
    case ESP_RST_BROWNOUT:   return "Brownout";
    case ESP_RST_SDIO:       return "SDIO";
    default:                 return "Unknown";
  }
#endif
}

void RebootManager::_saveMarker() {
  #ifdef ARDUINO_ARCH_ESP8266
    uint32 marker = MARKER_VALUE;
    system_rtc_mem_write(RTC_MEMORY_ADDRESS, &marker, sizeof(marker));
  #endif
  #ifdef ARDUINO_ARCH_ESP32
    preferences.begin(PREFS_NAMESPACE_NAME, READ_WRITE);
    preferences.putUInt(PREFS_KEY_NAME, MARKER_VALUE);
    preferences.end();
  #endif  
}

bool RebootManager::_hasMarker() {
  #ifdef ARDUINO_ARCH_ESP8266
    uint32 marker = 0;
    system_rtc_mem_read(RTC_MEMORY_ADDRESS, &marker, sizeof(marker));
    return (marker == MARKER_VALUE);
  #endif
  #ifdef ARDUINO_ARCH_ESP32
    preferences.begin(PREFS_NAMESPACE_NAME, READ_ONLY);
    bool result = (preferences.getUInt(PREFS_KEY_NAME, 0) == MARKER_VALUE);
    preferences.end();
    return result;
  #endif
}

void RebootManager::_clearMarker() {
  #ifdef ARDUINO_ARCH_ESP8266
    uint32 marker = 0;
    system_rtc_mem_write(RTC_MEMORY_ADDRESS, &marker, sizeof(marker));
  #endif
  #ifdef ARDUINO_ARCH_ESP32
    preferences.begin(PREFS_NAMESPACE_NAME, READ_WRITE);
    preferences.remove(PREFS_KEY_NAME);
    preferences.end();
  #endif  
}

void RebootManager::_printBootReason(BootReason reason) {
  switch (reason) {
    case BOOT_REASON_POWER_LOSS:
      Serial.println("RebootManager: 🔌 Cold boot (power-on or full power loss)");
      break;
    case BOOT_REASON_SOFTWARE_RESTART:
      Serial.println("RebootManager: 🔄 Software restart (ESP.restart())");
      break;
    case BOOT_REASON_DEEP_SLEEP:
      Serial.println("RebootManager: 🌙 Wake from deep sleep");
      break;
    case BOOT_REASON_RESET:
      Serial.println("RebootManager: 🔁 External reset (reset button or USB reset)");
      break;
    default:
      Serial.println("RebootManager: ❓ Unknown boot reason");
      break;
  }
}