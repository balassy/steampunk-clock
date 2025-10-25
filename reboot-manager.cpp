// Platform libraries.
#include <Arduino.h>        // To add IntelliSense for platform constants.
#include <user_interface.h> // To access RTC memory functions.

#include "reboot-manager.h"

#define RTC_MEMORY_ADDRESS 64    // Arbitrary RTC memory address (0–127 safe range).
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
  String rawReason = ESP.getResetReason();
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

void RebootManager::_saveMarker() {
  uint32 marker = MARKER_VALUE;
  system_rtc_mem_write(RTC_MEMORY_ADDRESS, &marker, sizeof(marker));
}

bool RebootManager::_hasMarker() {
  uint32 marker = 0;
  system_rtc_mem_read(RTC_MEMORY_ADDRESS, &marker, sizeof(marker));
  return (marker == MARKER_VALUE);
}

void RebootManager::_clearMarker() {
  uint32 marker = 0;
  system_rtc_mem_write(RTC_MEMORY_ADDRESS, &marker, sizeof(marker));
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