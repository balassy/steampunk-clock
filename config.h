#pragma once

// Pin configuration for the ESP32-WROOM-32 board.
inline constexpr uint8_t PIN_LED = 2;             // GPIO2 onboard LED
inline constexpr uint8_t PIN_HOUR_LED = 12;       // GPIO12 hour display LED (+, - --> GND)
inline constexpr uint8_t PIN_MINUTE_LED = 13;     // GPIO13 minute display LED (+, - --> GND)
inline constexpr uint8_t PIN_RTC_SDA = 21;        // GPIO21 SDA for RTC
inline constexpr uint8_t PIN_RTC_SCL = 22;        // GPIO22 SCL for RTC
inline constexpr uint8_t PIN_SERVO_HOUR = 16;     // GPIO16 hour servo
inline constexpr uint8_t PIN_SERVO_MINUTE = 17;   // GPIO17 minute servo
inline constexpr uint8_t PIN_BUZZER = 23;         // GPIO23 active buzzer (+, - --> GND)

inline constexpr uint8_t PIN_SETTINGS_BUTTON = 4; // GPIO4 Push button for resetting network settings (--> GND). 

inline constexpr uint8_t PIN_SWITCH_MODE_1 = 25;  // GPIO25 switch mode 1 (--> 3.3V)
inline constexpr uint8_t PIN_SWITCH_MODE_2 = 26;  // GPIO26 switch mode 2 (--> 3.3V)
inline constexpr uint8_t PIN_SWITCH_MODE_3 = 27;  // GPIO27 switch mode 3 (--> 3.3V)


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