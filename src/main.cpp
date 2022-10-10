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
#include "Config.hpp"
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

#define filesystem_CARD_SELECTION_PIN 10
SDClass filesystem;

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

Lock lock(10); // connection to the physical lock and handeling the unlock objects
const byte LOGGING_LEVEL = Log::log_level::L_DEBUG;
Log::Log logger(LOGGING_LEVEL);

#define RFID_SS (8)
#define RFID_RST (9) // Not connected by default on the NFC Shield
					 // the other 2 pins are connected to the MEGA-filesystemA(20) and MEAGE-SCL(21)

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

	pinMode(12, OUTPUT);
	digitalWrite(12, LOW);

	rfid = new RFID::RFID(RFID_SS, RFID_RST, &lock);
	rfid->begin();

	if (filesystem.begin(MFRC522_SPICLOCK, filesystem_CARD_SELECTION_PIN))
	{
		Serial.println("Initialized filesystem card");
		if (!filesystem.exists("log.log"))
		{
			auto file = filesystem.open("log.log");
			if (!file.available())
			{
				Serial.println("error while creating/opening log.log");
			}
			file.close();
		}
		else
		{
			Serial.println("log.log exists");
		}

		if (filesystem.exists("config"))
		{
			Serial.println("config file exists");
			/*
			auto config_file = filesystem.open("config", O_READ);
			Serial.print("file size: ");
			Serial.println(config_file.size());
			while (config_file.position() < config_file.size())
			{
				char buffer;
				config_file.readBytes(&buffer, 1);
				Serial.print(buffer);
			}
			config_file.seek(0); // jump to begin - we read already the whole file

			deserializeJson(config, config_file);
			JsonObject tag = config["RFID"]["RFID_tags"][0];
			uint8_t uid_size = tag["tag_uid"].size();
			uint8_t *tag_uid = new uint8_t[uid_size];
			uint8_t counter = 0;
			Serial.println("reading tag from json object");
			// JsonArray j_tag_uid = ;
			for (auto t_uid : static_cast<JsonArray>(tag["tag_uid"]))
			{
				Serial.print(t_uid.as<uint8_t>());
				Serial.print(' ');
				tag_uid[counter] = t_uid.as<uint8_t>();
				++counter;
			}
			Serial.println();
			Serial.println("printing the uint8_t tag_uid");
			for (uint8_t i = 0; i < uid_size; ++i)
			{
				Serial.print(tag_uid[i]);
				Serial.print(' ');
			}
			Serial.println();
			RFID::UID id(tag_uid, uid_size);
			Serial.println(id.to_string());
			rfid->add_tag(tag["id"], id);
			delete[] tag_uid;

			config_file.close();
			*/
		}
		else
			Serial.println("config doesnt exists");
	}
	else
	{
		Serial.println("ERROR while initializing filesystem card");
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
		system_clock.SetIsRunning(true);
	}
	if (system_clock.lost_power())
	{
		DEBUG_PRINTLN(F("RTC-Module lost power - change battery..."))
		logger.log(F("RTC-Module lost power - change battery..."), Log::log_level::L_DEBUG);
		Serial.println("Setting time to compile time time");
		RtcDateTime now(__DATE__, __TIME__);
		system_clock.SetIsWriteProtected(false);
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
	auto config_file = filesystem.open(F("config"), O_READ);
	if (!Config::read_config(config_file))
	{
		logger.log(F("Error while reading the config file"), Log::log_level::L_CRITICAL);
	}
	else
	{
		logger.log(F("read config file"), Log::log_level::L_DEBUG);
	}

	logger.serial_dump();

	while (true)
	{
		if (Serial.available() > 0)
		{
			char input;
			Serial.readBytes(&input, 1);
			Serial.println(input);
			if (input == 's')
			{
				digitalWrite(12, HIGH);
				delay(1000);
				rfid->begin();
				if (filesystem.begin())
				{
					Serial.println("success filesystem...");
				}
				else
				{
					Serial.println("error filesystem");
				}
			}
			else if (input == 'o')
			{
				digitalWrite(12, LOW);
				delay(1000);
				if (filesystem.begin())
				{
					Serial.println("success filesystem...");
					if (!filesystem.exists("log.log"))
					{
						// auto file = filesystem.open("log.log");
						// file.close();
						Serial.println("log.log doesnt exists");
					}
					else
					{
						Serial.println("log.log exists");
					}
				}
				else
				{
					Serial.println("error filesystem");
				}
			}
		}
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

		k_pad.loop();
		// m_window.loop();
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
