#include <Arduino.h>
#include <SPI.h>
#include <stdlib.h>
#include <ArduinoJson.h>
#include <RTCLib.h>
#include <uEEPROMLib.h>

#include "GlobalConstants.hpp"
#include "Helper.hpp"
#include "Fingerprint.hpp"
#include "RFID/RFID.hpp"
#include "Pin.hpp"
// #include "Fase.hpp"
#include "Keypad.hpp"
#include "system_clock/system_clock.hpp"
#include "logging/Log.hpp"
#include "Config.hpp"
#include "Lock/Lock.hpp"
#include "Lock/UNOB_handler.hpp"
// #define DEBUG

#ifdef DEBUG
// #include "avr8-stub.h"
// #include "app_api.h" // only needed with flash breakpoints
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
Clock::Clock system_clock;
// RTC_DS3231 system_clock;
uEEPROMLib system_clock_eeprom(0x57); // 0x57 is the default I2C address for the DS3231 EEPROM

#define filesystem_CARD_SELECTION_PIN 10
// SDClass filesystem;

UTFT myGLCD(ILI9341_16, 38, 39, 40, 41);
URTouch myTouch(6, 5, 4, 3, 2);

#define DISPLAY_ORIENTATION LANDSCAPE

#include "UI/lock_screen.hpp"
#include "UI/RFID_Settings.hpp"
#include "UI/Fingerprint/Fingerprint_Settings.hpp"
#include "UI/Settings.hpp"
#include "UI/boot_screen.hpp"
#include "UI/info_screen.hpp"

#include "UI/try_window.hpp"

FGUI::MainWindow m_window(&myGLCD, &myTouch, {0, 0}, {320 - 1, 240 - 1}); // "{320 - 1, 240 - 1}" --> we begin to count the pixels at 0,0
FGUI::WindowBase *home_window = nullptr;								  // the window to which the system returns after falling asleep
lock_screen *l_screen = nullptr;
Settings_window *settings_window = nullptr;
Info_Screen *info_screen = nullptr;

key_board::key_board<FGUI::MainWindow> k_pad(&m_window);

Lock lock(10); // connection to the physical lock and handeling the unlock objects
const byte LOGGING_LEVEL = Log::log_level::L_DEBUG;
Log::Log logger(LOGGING_LEVEL);

Fingerprint::Fingerprint *fingerprint;
RFID::RFID *rfid;
Pin pin(&lock, true);

UNOB_handler unob_handler; // unlock object handler

StaticJsonDocument<1024> config;

// Fase::Fase fase;

void fall_asleep_handler()
{
	m_window.set_active_window(home_window);
	fingerprint->led_control(Fingerprint::Fingerprint::led_modes::LED_OFF);
}

void setup()
{
#ifdef DEBUG
	debug_init();
#else

#endif // DEBUG
	Serial.begin(9600);
	Serial.println("Hallo");
	SPI.begin();
	// Wire.begin(); // I2C

	if (!system_clock.begin())
	{
		Serial.println(F("Error finding systemclock(DS3231)"));
		logger.log(F("CLOCK: Error finding systemclock(DS3231)"), Log::log_level::L_CRITICAL);
	}
	if (system_clock.lost_power())
	{
		Serial.println(F("RTC lost power, setting time to compile time!"));
		logger.log(F("CLOCK: RTC lost power - setting compile time"), Log::log_level::L_WARNING);
		DateTime compile_time(F(__DATE__), F(__TIME__));
		system_clock.set_date_time(Clock::time_point(compile_time));
	}

	Clock::time_point tmp(system_clock.now());
	Serial.print("now time: ");
	Serial.println(tmp.to_string());

	lock.begin();

	Serial.print("system_clock_eeprom page size: ");
	Serial.println(system_clock_eeprom.page_size);
	Serial.println("num of system_clock_eeprom pages: 4");
	for (uint16_t i = 0; i < system_clock_eeprom.page_size; ++i)
	{
		Serial.print(system_clock_eeprom.eeprom_read(i));
	}
	Serial.println();
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

	// boot_screen b_screen(&m_window);
	// m_window.set_active_window(&b_screen);

	l_screen = new lock_screen(&m_window);
	settings_window = new Settings_window(l_screen);
	info_screen = new Info_Screen(l_screen);
	home_window = l_screen;

	Serial.println("Created lock_screen...");
	m_window.on_fall_asleep = &fall_asleep_handler;
	Serial.println("set active_window");

	// try_window t_window(&m_window);
	m_window.set_active_window(l_screen);
	// m_window.set_active_window(settings_window);

	/*
	system_clock.Begin();
	if (system_clock.IsDateTimeValid())
	{
		Serial.println("RTC lost confidence in the DateTime!");
		RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
		system_clock.SetDateTime(compiled);
	}
	if (system_clock.GetIsWriteProtected())
	{
		system_clock.SetIsWriteProtected(false);
	}
	if (!system_clock.GetIsRunning())
	{
		Serial.println("!!! Systemclock isnt running");
		logger.log("Systemclock isnt running", Log::log_level::L_ERROR);
	}
	if (system_clock.lost_power())
	{
		DEBUG_PRINTLN(F("RTC-Module lost power - change battery..."))
		logger.log(F("RTC-Module lost power - change battery..."), Log::log_level::L_DEBUG);
		Serial.println("Setting time to compile time time");
		RtcDateTime now(__DATE__, __TIME__);
		system_clock.SetDateTime(now);
		// add warning to msg-log to print on the display
	}
	else
	{
		DEBUG_PRINTLN(F("RTC-Module doesnt lost power"))
	}
	*/

	/*
	DEBUG_PRINTLN((int)(sizeof(RtcDateTime) % sizeof(uint8_t)));
	DEBUG_PRINTLN((int)(sizeof(RtcDateTime) / sizeof(uint8_t)));
	DEBUG_PRINTLN((int)(sizeof(RtcDateTime)) * 8);
	DEBUG_PRINTLN((int)sizeof(unsigned short));
	DEBUG_PRINTLN(sizeof(byte));

	for (uint8_t i = 0; i < 2; ++i)
	{
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
		delay(2000);
	}
	*/

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

	rfid = new RFID::RFID(RFID_SS, RFID_RST, &lock);
	rfid->begin();

	pin.set_pin("1");

	unob_handler.add_unob(fingerprint);
	unob_handler.add_unob(rfid);
	unob_handler.add_unob(&pin);

	logger.serial_dump();

	while (true)
	{
		k_pad.loop();
		m_window.loop();
		// lock.loop();
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
