#include <Arduino.h>
#include <Adafruit_Fingerprint.h>
#include <SPI.h>
#include <MFRC522.h>
#include <stdlib.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>

// Allow Debug-Outputs on the Debug-Output
#define DEBUG

// Outputs the given Data on the Debug-Output if DEBUG is defined
#if defined DEBUG
#define DEBUG_PRINT(X) \
    Serial.println((X));
#else
#define DEBUG_PRINT(X)
#endif

#include "include/Helper.hpp"
#include "include/Fingerprint.hpp"
#include "include/RFID/RFID.hpp"
#include "include/GlobalConstants.hpp"
#include "include/Fase.hpp"
#include "include/Keypad.hpp"
#include "include/system_clock.hpp"

/*
    Touch-Display:  https://www.youtube.com/watch?v=9Ms59ofSJIY
                    https://www.youtube.com/watch?v=R_V_lzAbnb8

    RTC:
        Library: RTC by Makuna
*/

// !!!!!! Myserial is for Fingerprint-Sensor !!!!!!
#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)
// For UNO and others without hardware serial, we must use software serial...
// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
// Set up the serial port to use softwareserial..
// SoftwareSerial mySerial(2, 3);
#else
// On Leonardo/M0/etc, others with hardware serial, use hardware serial!
// #0 is green wire, #1 is white
#define mySerial Serial1
#endif
Fase::Fase fase;
//                       Serial-connection   default-password for database
//  Fingerprint::Fingerprint finger(&mySerial);
void setup()
{
    Serial.begin(9600);
    EEPROM.begin();
    if (system_clock.lost_power())
    {
        DEBUG_PRINT("RTC-Module lost power - change battery...");
        // add warning to msg-log to print on the display
    }
    Serial.println("Started startup...");

    fase.begin();
}

void loop()
{
    fase.loop();
}