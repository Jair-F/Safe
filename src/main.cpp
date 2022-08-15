#include <Arduino.h>
#include <Adafruit_Fingerprint.h>
#include <SPI.h>
#include <MFRC522.h>
#include <stdlib.h>
#include <ArduinoJson.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>
//#include <SD.h>

#include "GlobalConstants.hpp"
#include "Helper.hpp"
#include "Fingerprint.hpp"
#include "RFID/RFID.hpp"
//#include "Fase.hpp"
#include "Keypad.hpp"
#include "system_clock.hpp"
#include "logging/Log.hpp"

//#define DEBUG

#ifdef DEBUG
//#include "avr8-stub.h"
//#include "app_api.h" // only needed with flash breakpoints
#endif // SERIAL_DEBUG

#include "UI/UI.hpp"

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

UI::MainWindow m_window(&myGLCD, &myTouch, {0, 0}, {320 - 1, 240 - 1}); // "{320 - 1, 240 - 1}" --> we begin to count the pixels at 0,0

lock_screen *l_screen;

// -------------

key_board::key_board<UI::MainWindow> k_pad(&m_window);
// Adafruit_Keypad keypad(makeKeymap(Keypad::KEYS), Keypad::ROW_PINS, Keypad::COLUMN_PINS, ROWS, COLUMNS);

extern const unsigned short settings_white[];
extern const unsigned short back_sign_white[];

const byte LOGGING_LEVEL = Log::log_level::L_DEBUG;

Log::Log logger(LOGGING_LEVEL);

/*
    ready setup clock-object
    to ensure the battery of the clock is not empty check the lost_power method in the setup.
*/
Clock::Clock<ThreeWire> system_clock(RTC_DATA, RTC_CLK, RTC_RST);

void setup()
{
#ifdef DEBUG
    debug_init();
#else

#endif // DEBUG
    Serial.begin(9600);
    // EEPROM.begin();

    Serial.println("Startup...");

    myGLCD.InitLCD(DISPLAY_ORIENTATION);
    myGLCD.setFont(SmallFont);
    myGLCD.clrScr();
    myTouch.InitTouch(DISPLAY_ORIENTATION);
    myTouch.setPrecision(PREC_MEDIUM);
    // myGLCD.drawBitmap()

    Serial.println("Display-Size: ");
    Serial.print('(');
    Serial.print(myGLCD.getDisplayXSize());
    Serial.print(",");
    Serial.print(myGLCD.getDisplayYSize());
    Serial.println(')');

    l_screen = new lock_screen(&m_window);

    Serial.println("Created lock_screen...");
    m_window.set_active_window(l_screen);
    Serial.println("set active_window");

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
    // Fase::Fase fase;
    // fase.begin();
    logger.log(F("Everything initialized"), Log::log_level::L_DEBUG);

    k_pad.on_input = &UI::MainWindow::send_input;
    k_pad.on_enter = &UI::MainWindow::send_enter;
    k_pad.on_backspace = &UI::MainWindow::send_backspace;

    Serial.println(myGLCD.getDisplayXSize());
    Serial.println(myGLCD.getDisplayYSize());

    logger.serial_dump();

    /*
    Adafruit_Fingerprint finger(&Serial3);
    finger.begin(57600);
    if (finger.verifyPassword())
    {
        Serial.println(F("FINGERPRINT: found fingerprint"));
    }
    else
    {
        Serial.println(F("FINGERPRINT: didnt found fingerprint-sensor"));
    }
    finger.LEDcontrol(FINGERPRINT_LED_ON, 2, FINGERPRINT_LED_RED, 2);
    */

    // Serial.println(l_screen.pos().x_pos);
    // Serial.println(l_screen.pos().y_pos);

    // Serial.println("active window is set");

    // Serial.println("Free memory: ");
    // Serial.println(freeMemory());
    while (true)
    {
        k_pad.loop();
        m_window.loop();
        // fase.loop();
    }
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
}