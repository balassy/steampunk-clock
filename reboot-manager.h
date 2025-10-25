#ifndef REBOOT_MANAGER_H
#define REBOOT_MANAGER_H

#include <Arduino.h>

enum BootReason {
  BOOT_REASON_POWER_LOSS,         // Power-on or full power loss
  BOOT_REASON_SOFTWARE_RESTART,   // Software restart (ESP.restart)
  BOOT_REASON_DEEP_SLEEP,         // Wake from deep sleep
  BOOT_REASON_RESET,              // Manual or USB reset
  BOOT_REASON_UNKNOWN             // Unknown (fallback)
};

class RebootManager {
  public:
  static bool isReset();
  static void markSetupComplete();
  
  private:
  static void _saveMarker();
  static bool _hasMarker();
  static void _clearMarker();  
  static BootReason _getBootReason();
  static void _printBootReason(BootReason reason);
};

#endif /* REBOOT_MANAGER_H */