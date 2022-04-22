#include <Arduino.h>
#include <Adafruit_Fingerprint.h>
#include <SPI.h>
#include <MFRC522.h>
#include <stdlib.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>

#include "Helper.hpp"
#include "Fingerprint.hpp"
#include "RFID/RFID.hpp"
#include "GlobalConstants.hpp"
#include "Fase.hpp"
#include "Keypad.hpp"
#include "system_clock.hpp"

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
        DEBUG_PRINT(F("RTC-Module lost power - change battery..."))
        RtcDateTime now(__DATE__, __TIME__);
        system_clock.SetDateTime(now);
        // add warning to msg-log to print on the display
    }
    else
    {
        DEBUG_PRINT(F("RTC-Module doesnt lost power"))
    }
    Serial.println((int)(sizeof(RtcDateTime) % sizeof(uint8_t)));
    Serial.println((int)(sizeof(RtcDateTime) / sizeof(uint8_t)));
    Serial.println((int)(sizeof(RtcDateTime)) * 8);
    Serial.println((int)sizeof(unsigned short));
    Serial.println(sizeof(byte));

    Serial.print("Time: ");
    RtcDateTime now = system_clock.GetDateTime();
    Serial.print(now.Day());
    Serial.print('/');
    Serial.print(now.Month());
    Serial.print('/');
    Serial.print(now.Year());
    Serial.print(' ');
    Serial.print(now.Hour());
    Serial.print(':');
    Serial.print(now.Minute());
    Serial.print(':');
    Serial.print(now.Second());
    Serial.print(F("   Weekday: "));
    Serial.println(now.DayOfWeek());

    Serial.println(F("Started startup..."));

    fase.begin();
}

void loop()
{
    fase.loop();
}