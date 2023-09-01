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
	// delay(1000); // in order to ensure that the board boots up after long time no power

#ifdef DEBUG
	debug_init();
#else

#endif // DEBUG
	Serial.begin(9600);
	Serial.println("Hallo");
	SPI.begin();
	// Wire.begin(); // I2C

	pinMode(RELAY_PIN, OUTPUT); // lock relay
	digitalWrite(RELAY_PIN, HIGH);

	if (!system_clock.begin())
	{
		Serial.println(F("Error finding systemclock(DS3231)"));
		logger.log(F("CLOCK: Error finding systemclock(DS3231)"), Log::log_level::L_CRITICAL);
	}

	Clock::time_point tmp(system_clock.now());
	Serial.print("now time: ");
	Serial.println(tmp.to_string());

	lock.begin();

	if (system_clock.lost_power())
	{
		Serial.println(F("RTC lost power, setting time to compile time!"));
		logger.log(F("CLOCK: RTC lost power - setting compile time"), Log::log_level::L_WARNING);
		DateTime compile_time(F(__DATE__), F(__TIME__));
		system_clock.set_date_time(Clock::time_point(compile_time));
	}

	Serial.println("Startup...");

	fingerprint = new Fingerprint::Fingerprint(&Serial3, &lock);
	rfid = new RFID::RFID(RFID_SS, RFID_RST, &lock);

	/**
	 * config-reading
	 */
	Serial.println("reading config...");
	{
		{
			String config_str;
			Config::read_config(&config_str);
			deserializeJson(config, config_str);
		}

		Serial.println(F("config read - setting up devices"));

		/* ----- PIN ----- */

		if (config[F("PIN")][F("enabled")] == true)
			pin.enable();
		else
			pin.disable();
		pin.set_pin(config[F("PIN")][F("pin")]);

		/* ----- RFID ----- */

		auto rfid_ref = config[F("RFID")];
		if (rfid_ref[F("enabled")] == true)
			rfid->enable();
		else
			rfid->disable();

		JsonArray rfid_tags = config[F("RFID")][F("RFID_tags")].as<JsonArray>();
		for (uint8_t j = 0; j < rfid_tags.size(); ++j)
		{
			uint8_t uid_length = rfid_tags[j][F("tag_uid")].size();
			uint8_t *uid = new uint8_t(uid_length);
			uint8_t id = rfid_tags[j][F("id")]; // the id is not the same as the id in the array because the user could skip setting id 2
			for (uint8_t i = 0; i < uid_length; ++i)
			{
				uid[i] = rfid_tags[j][F("tag_uid")][i];
			}

			RFID::UID tmp_tag(uid, uid_length);
			rfid->add_tag(id, tmp_tag);

			delete[] uid;
		}

		/* ----- FINGERPRINT ----- */
		if (config[F("Fingerprint")][F("enabled")] == true)
			fingerprint->enable();
		else
			fingerprint->disable();

		/* ----- SYSTEM/LOCK ----- */
		lock.set_locking_period(config[F("system")][F("locking_period")]);
		lock.set_allowed_unauthorized_unlock_tries(config[F("system")][F("allowed_unauthorized_unlock_tries")]);
		m_window.set_fall_asleep_timer(config[F("system")][F("screen_timeout")]);
		logger.set_logging_level(config[F("system")][F("logging_level")]);
	}

	Serial.println("config set - starting everthing up...");

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
	Serial.println("set active_window"); // try_window t_window(&m_window);
	m_window.set_active_window(l_screen);
	// m_window.set_active_window(settings_window);

	logger.log(F("Started startup"), Log::log_level::L_DEBUG);
	// fase.begin();
	logger.log(F("Everything initialized"), Log::log_level::L_DEBUG);

	k_pad.on_input = &FGUI::MainWindow::send_input;
	k_pad.on_enter = &FGUI::MainWindow::send_enter;
	k_pad.on_backspace = &FGUI::MainWindow::send_backspace;

	Serial.println(myGLCD.getDisplayXSize());
	Serial.println(myGLCD.getDisplayYSize());

	fingerprint->begin();
	rfid->begin();

	// pin.set_pin("1");

	unob_handler.add_unob(fingerprint);
	unob_handler.add_unob(rfid);
	unob_handler.add_unob(&pin);

	logger.serial_dump();

	Config::reset_config();

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
