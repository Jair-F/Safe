#include <Arduino.h>
#include <SPI.h>
#include <stdlib.h>
#include <ArduinoJson.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>
#include <SD.h>

#include <MemoryFree.h>

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

#include "FGUI/FGUI.hpp"

/*
    Touch-Display:  https://www.youtube.com/watch?v=9Ms59ofSJIY
                    https://www.youtube.com/watch?v=R_V_lzAbnb8

    RTC:
        Library: RTC by Makuna
*/
/*
    ready setup clock-object
    to ensure the battery of the clock is not empty check the lost_power method in the setup.
*/
Clock::Clock<ThreeWire> system_clock(RTC_DATA, RTC_CLK, RTC_RST);

#define SD_CARD_SELECTION_PIN 10
SDClass sd;

extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t mykefont2[];
extern uint8_t Retro8x16[];
extern uint8_t Sinclair_Inverted_S[];
extern uint8_t TinyFont[];
extern uint8_t Open_Sans_Regular_10[];

UTFT myGLCD(ILI9341_16, 38, 39, 40, 41);
URTouch myTouch(6, 5, 4, 3, 2);

#define DISPLAY_ORIENTATION LANDSCAPE

#include "UI/lock_screen.hpp"
#include "UI/RFID_Settings.hpp"
#include "UI/RFID_delete_tag_by_id.hpp"
#include "UI/Fingerprint/Fingerprint_Settings.hpp"
#include "UI/Settings.hpp"

FGUI::MainWindow m_window(&myGLCD, &myTouch, {0, 0}, {320 - 1, 240 - 1}); // "{320 - 1, 240 - 1}" --> we begin to count the pixels at 0,0
// lock_screen *l_screen;

key_board::key_board<FGUI::MainWindow> k_pad(&m_window);
extern const unsigned short settings_white[];
extern const unsigned short back_sign_white[];

Lock lock(5); // connection to the physical lock and handeling the unlock objects
const byte LOGGING_LEVEL = Log::log_level::L_DEBUG;
Log::Log logger(LOGGING_LEVEL);

#define RFID_SS (8)
#define RFID_RST (9) // Not connected by default on the NFC Shield
                     // the other 2 pins are connected to the MEGA-SDA(20) and MEAGE-SCL(21)

Fingerprint::Fingerprint *fingerprint;
RFID::RFID *rfid;

StaticJsonDocument<1024> config;

// Fase::Fase fase;

#include <SPI.h>
void setup()
{
#ifdef DEBUG
    debug_init();
#else

#endif // DEBUG
    Serial.begin(9600);
    SPI.begin();
    delay(500);

    // rfid = new RFID::RFID(RFID_SS, RFID_RST, &lock);
    // rfid->begin();

    if (!sd.begin(MFRC522_SPICLOCK, SD_CARD_SELECTION_PIN))
    {
        Serial.println("ERROR while initializing sd card");
    }
    else
    {
        Serial.println("Initialized SD card");
        if (!sd.exists("log.log"))
        {
            auto file = sd.open("log.log");
            file.close();
        }
        else
        {
            Serial.println("log.log exists");
        }
    }

    Serial.println("Startup...");

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

    // l_screen = new lock_screen(&m_window);
    Settings_window settings_window(&m_window);

    Serial.println("Created lock_screen...");
    m_window.set_active_window(&settings_window);
    Serial.println("set active_window");

    if (!system_clock.GetIsRunning())
    {
        Serial.println("!!! Systemclock isnt running");
        logger.log("Systemclock isnt running", Log::log_level::L_ERROR);
    }
    else if (system_clock.lost_power())
    {
        DEBUG_PRINTLN(F("RTC-Module lost power - change battery..."))
        logger.log(F("RTC-Module lost power - change battery..."), Log::log_level::L_DEBUG);
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

    k_pad.on_input = &FGUI::MainWindow::send_input;
    k_pad.on_enter = &FGUI::MainWindow::send_enter;
    k_pad.on_backspace = &FGUI::MainWindow::send_backspace;

    Serial.println(myGLCD.getDisplayXSize());
    Serial.println(myGLCD.getDisplayYSize());

    // Adafruit_Fingerprint finger(&Serial3);
    fingerprint = new Fingerprint::Fingerprint(&Serial3, &lock);
    fingerprint->begin();

    /*
    Serial.println("waiting for tag");
    while (!rfid->tag_present())
    {
        Serial.print('.');
        delay(500);
    }
    Serial.print("tag present: ");
    auto uid = rfid->read_tag_UID();
    Serial.println(uid.to_string());
    */

    /*
    Serial.println("adding finger to sensor");
    while (!fingerprint->add_finger(1))
    {
        delay(1000);
        Serial.println('.');
    }
    */

    logger.serial_dump();
    while (true)
    {
        /*
        if (fingerprint->finger_on_sensor())
        {
            Serial.println("finger on sensor");
            auto err_code = fingerprint->read();
            if (err_code == Unlock_Object::unlock_authentication_reports::AUTHORIZED_UNLOCK_OBJECT)
            {
                Serial.println("read valid finger");
            }
            else if (err_code == Unlock_Object::unlock_authentication_reports::UNLOCK_OBJECT_READ_ERROR)
            {
                Serial.println("error while reading the finger");
            }
            else
            {
                Serial.println("unauthorized finger");
            }
        }
        */

        // k_pad.loop();
        //  m_window.loop();
        lock.loop();
        //  fase.loop();
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
