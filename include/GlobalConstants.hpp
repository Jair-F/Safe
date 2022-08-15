#pragma once
#include <Arduino.h>

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
#define PIN_C1 DAC0
#define PIN_C2 DAC1
#define PIN_C3 CANRX

/*
#define PIN_R1 A0
#define PIN_R2 A1
#define PIN_R3 A2
#define PIN_R4 A3
#define PIN_C1 A4
#define PIN_C2 A5
#define PIN_C3 A6
*/

// RTC-Clock-Pins

#define RTC_DATA A2 // DATA/IO
#define RTC_CLK A1  // CLK/SCLK
#define RTC_RST A3  // RST/CE

/*
#define RTC_DATA A9 // DATA/IO
#define RTC_CLK A8  // CLK/SCLK
#define RTC_RST A10 // RST/CE
*/
