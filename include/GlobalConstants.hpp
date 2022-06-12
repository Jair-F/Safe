#pragma once

// Hardware_Pinouts

// Serial-Object (for fingerprint)
#define SERIAL_RECEIVE_PIN 2
#define SERIAL_TRANSMIT_PIN 3
#define FINGERPRINT_POWER_PIN A0

// RFID
#define MFRC522_RST_PIN 49 // RST_PIN
#define MFRC522_SS_PIN 53  // SDA_PIN

// Keypad-Pins
#define PIN_R1 2
#define PIN_R2 3
#define PIN_R3 4
#define PIN_R4 5
#define PIN_C1 8
#define PIN_C2 9
#define PIN_C3 10

// RTC-Clock-Pins
#define RTC_DATA 5 // DATA/IO
#define RTC_CLK 6  // CLK/SCLK
#define RTC_RST 7  // RST/CE