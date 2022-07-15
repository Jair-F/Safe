#pragma once

// Hardware_Pinouts

// Serial-Object (for fingerprint)
#define SERIAL_RECEIVE_PIN 2
#define SERIAL_TRANSMIT_PIN 3
#define FINGERPRINT_POWER_PIN A0

// RFID
#define MFRC522_RST_PIN A5 // RST_PIN
#define MFRC522_SS_PIN A4  // SDA_PIN

// Keypad-Pins
#define PIN_R1 A8
#define PIN_R2 A9
#define PIN_R3 A10
#define PIN_R4 A11
#define PIN_C1 A12
#define PIN_C2 A13
#define PIN_C3 A14

// RTC-Clock-Pins
#define RTC_DATA A2 // DATA/IO
#define RTC_CLK A1  // CLK/SCLK
#define RTC_RST A3  // RST/CE