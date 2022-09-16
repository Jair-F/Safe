#include <Arduino.h>
#include <Adafruit_Fingerprint.h>
#include <SPI.h>
#include <stdlib.h>
#include <ArduinoJson.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>
#include <SdFat.h>
#include <Adafruit_PN532.h>

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

#define SD_CARD_SELECTION_PIN 53
SdFat sd;

extern uint8_t SmallFont[];
extern uint8_t BigFont[];

UTFT myGLCD(ILI9341_16, 38, 39, 40, 41);
URTouch myTouch(6, 5, 4, 3, 2);

#define DISPLAY_ORIENTATION LANDSCAPE

#include "UI/lock_screen.hpp"
#include "UI/RFID_Settings.hpp"

FGUI::MainWindow m_window(&myGLCD, &myTouch, {0, 0}, {320 - 1, 240 - 1}); // "{320 - 1, 240 - 1}" --> we begin to count the pixels at 0,0
lock_screen *l_screen;
RFID_settings_window *rfid_settings_wnd;

key_board::key_board<FGUI::MainWindow> k_pad(&m_window);
// Adafruit_Keypad keypad(makeKeymap(key_board::KEYS), key_board::ROW_PINS, key_board::COLUMN_PINS, ROWS, COLUMNS);

extern const unsigned short settings_white[];
extern const unsigned short back_sign_white[];

Lock lock(5); // connection to the physical lock and handeling the unlock objects
const byte LOGGING_LEVEL = Log::log_level::L_DEBUG;
Log::Log logger(LOGGING_LEVEL);

#define PN532_IRQ (A7)
#define PN532_RESET (3) // Not connected by default on the NFC Shield
// the other 2 pins are connected to the MEGA-SDA(20) and MEAGE-SCL(21)
// Adafruit_PN532 rfid(PN532_IRQ, PN532_RESET);

StaticJsonDocument<1024> config;

Fingerprint::Fingerprint fingerprint(&Serial3, &lock);
RFID::RFID rfid(A7, 2, &lock);
// Fase::Fase fase;

void setup()
{
#ifdef DEBUG
    debug_init();
#else

#endif // DEBUG
    Serial.begin(9600);
    // EEPROM.begin();
    delay(500);

    if (!sd.begin(SD_CARD_SELECTION_PIN))
    {
        Serial.println("ERROR while initializing sd card");
    }
    else
    {
        Serial.println("Initialized SD card");
    }

    if (sd.format())
    {
        Serial.println("Success at sd card format.");
    }
    else
    {
        Serial.println("Error while formating the sd card");
    }
    auto fat_type = sd.fatType();

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

    l_screen = new lock_screen(&m_window);
    rfid_settings_wnd = new RFID_settings_window(&m_window);

    Serial.println("Created lock_screen...");
    m_window.set_active_window(l_screen);
    Serial.println("set active_window");

    if (system_clock.lost_power())
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

    logger.serial_dump();

    /*
        rfid.begin();
        auto version_data = rfid.getFirmwareVersion();
        if (!version_data)
        {
            Serial.print("Didn’t find PN53x board");
        }
        else
        {
            Serial.print("PN532-Version: ");
            Serial.println((version_data >> 24) & 0xFF, HEX);
            Serial.print("Firmware ver. ");
            Serial.print((version_data >> 16) & 0xFF, DEC);
            Serial.print('.');
            Serial.println((version_data >> 8) & 0xFF, DEC);
        }
        bool status = rfid.SAMConfig();
        if (!status)
        {
            Serial.println("Error configuring PN5322 for ISO1443A-Cards");
        }
        */

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
        /*
        bool rfid_read_succes;
        uint8_t uid[7]; // two types of uids - type1(4 bytes long) and type2(7 bytes long)
        uint8_t uid_length;
        String tmp;

        rfid_read_succes = rfid.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uid_length);
        if (rfid_read_succes)
        {
            Serial.println("Found an ISO14443A card");
            Serial.print(" UID Length: ");
            Serial.print(uid_length, DEC);
            Serial.println(" bytes");
            Serial.print(" UID Value: ");
            rfid.PrintHex(uid, uid_length);
            Serial.println("");

            if (uid_length == 4)
            {
                // We probably have a Mifare Classic card …
                Serial.println("Seems to be a Mifare Classic card (4 byte UID)");

                // Now we need to try to authenticate it for read/write access
                // Try with the factory default KeyA: 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF
                Serial.println("Trying to authenticate block 4 with default KEYA value");
                uint8_t keya[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

                // Start with block 4 (the first block of sector 1) since sector 0
                // contains the manufacturer data and it’s probably better just
                // to leave it alone unless you know what you’re doing
                rfid_read_succes = rfid.mifareclassic_AuthenticateBlock(uid, uid_length, 4, MIFARE_CMD_AUTH_A, keya);

                if (rfid_read_succes)
                {
                    Serial.println("Sector 1 (Blocks 4…7) has been authenticated");
                    uint8_t data[16];

                    // If you want to write something to block 4 to test with, uncomment
                    // the following line and this text should be read back in a minute
                    // memcpy(data, (const uint8_t){ 'a', 'd', 'a', 'f', 'r', 'u', 'i', 't', '.', 'c', 'o', 'm', 0, 0, 0, 0 }, sizeof data);
                    // success = nfc.mifareclassic_WriteDataBlock (4, data);

                    // Try to read the contents of block 4
                    rfid_read_succes = rfid.mifareclassic_ReadDataBlock(4, data);

                    if (rfid_read_succes)
                    {
                        // Data seems to have been read … spit it out
                        Serial.println("Reading Block 4:");
                        rfid.PrintHexChar(data, 16);
                        Serial.println("");

                        // Wait a bit before reading the card again
                        delay(1000);
                    }
                    else
                    {
                        Serial.println("Ooops … unable to read the requested block. Try another key?");
                    }

                    uint8_t data_to_write[16] = {
                        1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
                    for (uint8_t i = 0; i < 16; ++i)
                    {
                        data_to_write[i] = random(0, 256);
                    }
                    bool ret = rfid.mifareclassic_WriteDataBlock(4, data_to_write);
                    if (ret)
                    {
                        Serial.println("verifying write...");
                        uint8_t check_data_block[16];
                        bool same = true;
                        bool suc = rfid.mifareclassic_ReadDataBlock(4, check_data_block);
                        Serial.println("checking read data");
                        for (uint8_t i = 0; i < 16; ++i)
                        {
                            if (check_data_block[i] != data_to_write[i] || suc == false)
                            {
                                same = false;
                                break;
                            }
                        }
                        if (same)
                            Serial.println("write - success");
                        else
                            Serial.println("write - fail");
                        delay(1500);
                    }
                    else
                    {
                        Serial.println("write - fail");
                    }
                }
                else
                {
                    Serial.println("Ooops … authentication failed: Try another key?");
                }
            }

            if (uid_length == 7)
            {
                // We probably have a Mifare Ultralight card …
                Serial.println("Seems to be a Mifare Ultralight tag (7 byte UID)");

                // Try to read the first general-purpose user page (#4)
                Serial.println("Reading page 4");
                uint8_t data[32];
                rfid_read_succes = rfid.mifareclassic_WriteDataBlock(4, data);
                if (rfid_read_succes)
                {
                    // Data seems to have been read … spit it out
                    rfid.PrintHexChar(data, 4);
                    Serial.println("");

                    // Wait a bit before reading the card again
                    delay(1000);
                }
                else
                {
                    Serial.println("Ooops … unable to read the requested page!?");
                }
            }
        }
        */

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
