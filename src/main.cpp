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

//#define DEBUG

#ifdef DEBUG
#include "avr8-stub.h"
#include "app_api.h" // only needed with flash breakpoints
#endif               // SERIAL_DEBUG

#include "UI/UI.hpp"
#include <MemoryFree.h>

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

#define DISPLAY_ORIENTATION LANDSCAPE

#include "UI/lock_screen.hpp"

UI::MainWindow m_window(&myGLCD, &myTouch, {0, 0}, {320, 240});

lock_screen *l_screen;

// -------------

// Fase::Fase fase;

Keypad::Keypad<UI::MainWindow> k_pad(&m_window);

void setup()
{
#ifdef DEBUG
    debug_init();
#else
    Serial.begin(9600);
#endif // DEBUG

    EEPROM.begin();

    myGLCD.InitLCD(DISPLAY_ORIENTATION);
    myGLCD.setFont(SmallFont);
    myGLCD.clrScr();
    myTouch.InitTouch(DISPLAY_ORIENTATION);
    myTouch.setPrecision(PREC_MEDIUM);

    Serial.println("Display-Size: ");
    Serial.print('(');
    Serial.print(myGLCD.getDisplayXSize());
    Serial.print(",");
    Serial.print(myGLCD.getDisplayYSize());
    Serial.println(')');

    l_screen = new lock_screen(&m_window);

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
    // fase.begin();
    logger.log(F("Everything initialized"), Log::log_level::L_DEBUG);

    k_pad.on_input = &m_window.send_input;
    k_pad.on_enter = &m_window.send_enter;
    k_pad.on_backspace = &m_window.send_backspace;
    k_pad.begin();

    Serial.println(myGLCD.getDisplayXSize());
    Serial.println(myGLCD.getDisplayYSize());

    // logger.serial_dump();
    m_window.set_active_window(l_screen);

    // Serial.println(l_screen.pos().x_pos);
    // Serial.println(l_screen.pos().y_pos);

    // Serial.println("active window is set");

    // Serial.println("Free memory: ");
    // Serial.println(freeMemory());
}

void loop()
{
    /*
    char key = k_pad.get();
    if (key != '\0')
    {
        Serial.println(key);
    }
    */
    k_pad.loop();
    m_window.loop();

    // fase.loop();
}