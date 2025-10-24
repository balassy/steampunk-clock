// Pin configuration.
const uint8_t PIN_LED = D4;          // GPIO2 onboard LED
const uint8_t PIN_HOUR_LED = D6;     // GPIO12 hour display LED
const uint8_t PIN_MINUTE_LED = D7;   // GPIO13 minute display LED
const uint8_t PIN_RTC_SDA = D2;      // GPIO4 SDA for RTC
const uint8_t PIN_RTC_SCL = D1;      // GPIO5 SCL for RTC
const uint8_t PIN_SERVO_HOUR = D0;   // GPIO16 for the hour servo
const uint8_t PIN_SERVO_MINUTE = D5; // GPIO14 for the minute servo

// Hardware settigs.
const unsigned long SERIAL_BAUD_RATE = 115200;

// Network configuration.
const char* WIFI_AP_SSID = "SteampunkClock";  // The name of the wireless network to create if cannot connect using the previously saved credentials.
const char* WIFI_AP_PASSWORD = "SteamPass!";  // The password required to connect to the wireless network used to configure the network parameters.