#include <Arduino.h>
#include <Adafruit_Fingerprint.h>
#include <SPI.h>
#include <MFRC522.h>
#include <stdlib.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>

#include "GlobalVariables.hpp"
#include "GlobalConstants.hpp"
#include "Helper.hpp"
#include "Fingerprint.hpp"
#include "RFID/RFID.hpp"
#include "Fase.hpp"
#include "Keypad.hpp"
#include "system_clock.hpp"
#include "logging/Log.hpp"

#ifdef SERIAL_DEBUG
#include "avr8-stub.h"
#include "app_api.h" // only needed with flash breakpoints
#endif               // SERIAL_DEBUG

#include "UI/MainWindow.hpp"
#include "UI/Widget.hpp"
#include "UI/Button.hpp"

/*
    Touch-Display:  https://www.youtube.com/watch?v=9Ms59ofSJIY
                    https://www.youtube.com/watch?v=R_V_lzAbnb8

    RTC:
        Library: RTC by Makuna
*/

extern uint8_t SmallFont[];
extern uint8_t BigFont[];

UTFT myGLCD(ILI9341_16, 38, 39, 40, 41);
URTouch myTouch(6, 5, 4, 3, 2);

#define TOUCH_ORIENTATION PORTRAIT

position upper_left{20, 40};
position lower_right{130, 70};

Button _button(upper_left, lower_right, myGLCD, myTouch);

// -------------

Fase::Fase fase;

// Keypad::Keypad k_pad;

void setup()
{
    Serial.begin(9600);

    myGLCD.InitLCD();
    myGLCD.setFont(SmallFont);
    myGLCD.clrScr();
    myTouch.InitTouch(TOUCH_ORIENTATION);
    myTouch.setPrecision(PREC_MEDIUM);
    _button.setText("Click mich");
    //_button.on_click = []()
    //{ Serial.println("Clicked button"); };
    //_button.on_release = []()
    //{ Serial.println("Released button"); };

    EEPROM.begin();
    if (system_clock.lost_power())
    {
        DEBUG_PRINTLN(F("RTC-Module lost power - change battery..."))
        // logger.log(F("RTC-Module lost power - change battery..."), Log::log_level::L_DEBUG);
        RtcDateTime now(__DATE__, __TIME__);
        system_clock.SetDateTime(now);
        // add warning to msg-log to print on the display
    }
    else
    {
        DEBUG_PRINTLN(F("RTC-Module doesnt lost power"))
    }
    DEBUG_PRINTLN((int)(sizeof(RtcDateTime) % sizeof(uint8_t)));
    DEBUG_PRINTLN((int)(sizeof(RtcDateTime) / sizeof(uint8_t)));
    DEBUG_PRINTLN((int)(sizeof(RtcDateTime)) * 8);
    DEBUG_PRINTLN((int)sizeof(unsigned short));
    DEBUG_PRINTLN(sizeof(byte));

    DEBUG_PRINT("Time: ");
    RtcDateTime now = system_clock.GetDateTime();
    DEBUG_PRINT(now.Day());
    DEBUG_PRINT('/');
    DEBUG_PRINT(now.Month());
    DEBUG_PRINT('/');
    DEBUG_PRINT(now.Year());
    DEBUG_PRINT(' ');
    DEBUG_PRINT(now.Hour());
    DEBUG_PRINT(':');
    DEBUG_PRINT(now.Minute());
    DEBUG_PRINT(':');
    DEBUG_PRINT(now.Second());
    DEBUG_PRINT(F("   Weekday: "));
    DEBUG_PRINTLN(now.DayOfWeek());

    logger.log(F("Started startup"), Log::log_level::L_DEBUG);
    fase.begin();
    logger.log(F("Everything initialized"), Log::log_level::L_DEBUG);

    // k_pad.begin();

    logger.serial_dump();
}

void loop()
{
    _button.loop();
    /*
    char key = k_pad.get();
    if (key != '\0')
    {
        Serial.println(key);
    }
    */

    fase.loop();
}