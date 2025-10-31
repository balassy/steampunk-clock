#pragma once

// Pin configuration for the Wemos D1 Mini board.
inline constexpr uint8_t PIN_LED = D4;          // GPIO2 onboard LED
inline constexpr uint8_t PIN_HOUR_LED = D6;     // GPIO12 hour display LED
inline constexpr uint8_t PIN_MINUTE_LED = D7;   // GPIO13 minute display LED
inline constexpr uint8_t PIN_RTC_SDA = D2;      // GPIO4 SDA for RTC
inline constexpr uint8_t PIN_RTC_SCL = D1;      // GPIO5 SCL for RTC
inline constexpr uint8_t PIN_SERVO_HOUR = D0;   // GPIO16 for the hour servo
inline constexpr uint8_t PIN_SERVO_MINUTE = D5; // GPIO14 for the minute servo

// Pin configuration for I2C expander (PCF8574).
inline constexpr uint8_t EXPANDER_PIN_POSITION_BUTTON_1 = 0;
inline constexpr uint8_t EXPANDER_PIN_POSITION_BUTTON_2 = 1;
inline constexpr uint8_t EXPANDER_PIN_POSITION_BUTTON_3 = 2;
inline constexpr uint8_t EXPANDER_PIN_SETTINGS_BUTTON = 3; // Button for resetting network settings. Connected to PCF8574 pin P3.

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