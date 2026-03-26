#pragma once

// Pin configuration for the NodeMCU board.
#ifdef ARDUINO_ARCH_ESP8266
  inline constexpr uint8_t PIN_LED = D4;          // GPIO2 onboard LED
  inline constexpr uint8_t PIN_HOUR_LED = D6;     // GPIO12 hour display LED (+)
  inline constexpr uint8_t PIN_MINUTE_LED = D7;   // GPIO13 minute display LED (+)
  inline constexpr uint8_t PIN_RTC_SDA = D2;      // GPIO4 SDA for RTC
  inline constexpr uint8_t PIN_RTC_SCL = D1;      // GPIO5 SCL for RTC
  inline constexpr uint8_t PIN_SERVO_HOUR = D0;   // GPIO16 hour servo
  inline constexpr uint8_t PIN_SERVO_MINUTE = D5; // GPIO14 minute servo
  inline constexpr uint8_t PIN_BUZZER = D3;       // GPIO0 active buzzer (+)
#endif

// Pin configuration for the ESP32-WROOM-32 board.
#ifdef ARDUINO_ARCH_ESP32
  inline constexpr uint8_t PIN_LED = 2;           // GPIO2 onboard LED
  inline constexpr uint8_t PIN_HOUR_LED = 12;     // GPIO12 hour display LED (+)
  inline constexpr uint8_t PIN_MINUTE_LED = 13;   // GPIO13 minute display LED (+)
  inline constexpr uint8_t PIN_RTC_SDA = 21;      // GPIO21 SDA for RTC
  inline constexpr uint8_t PIN_RTC_SCL = 22;      // GPIO22 SCL for RTC
  inline constexpr uint8_t PIN_SERVO_HOUR = 16;   // GPIO16 hour servo
  inline constexpr uint8_t PIN_SERVO_MINUTE = 17; // GPIO17 minute servo
  inline constexpr uint8_t PIN_BUZZER = 20;       // GPIO20 active buzzer (+)
#endif

inline constexpr uint8_t PIN_SETTINGS_BUTTON = 4; // GPIO4Button for resetting network settings. 

inline constexpr uint8_t PIN_SWITCH_MODE_1 = 25;
inline constexpr uint8_t PIN_SWITCH_MODE_2 = 26;
inline constexpr uint8_t PIN_SWITCH_MODE_3 = 27;


// Hardware settings.
inline constexpr unsigned long SERIAL_BAUD_RATE = 115200;

// Network configuration.
inline const char* WIFI_AP_SSID = "SteampunkClock";  // The name of the wireless network to create if cannot connect using the previously saved credentials.
inline const char* WIFI_AP_PASSWORD = "SteamPass!";  // The password required to connect to the wireless network used to configure the network parameters.

// NTP configuration.
inline const char* NTP_SERVER = "pool.ntp.org";
inline const char* NTP_TIMEZONE = "CET-1CEST,M3.5.0,M10.5.0/3"; // Full list: https://github.com/nayarsystems/posix_tz_db/blob/master/zones.json

// Clock configuration.
inline constexpr unsigned long CLOCK_UPDATE_INTERVAL_MSEC = 5000;