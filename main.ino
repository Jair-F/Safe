#include <Arduino.h>
#include <Adafruit_Fingerprint.h>
#include <SPI.h>
#include <MFRC522.h>
#include <stdlib.h>
#include <OSFS.h>
#include <ArduinoJson.h>
#include <EEPROM.h>

#include "include/Helper.hpp"
#include "include/Fingerprint.hpp"
#include "include/RFID/RFID.hpp"
#include "include/GlobalConstants.hpp"

// Touch-Display: https://www.youtube.com/watch?v=9Ms59ofSJIY

// MFRC522 rfid(MFRC522_SS_PIN, MFRC522_RST_PIN);
Lock::Lock lock(5, Lock::UNLOCKED, true);
RFID::RFID rfid(MFRC522_SS_PIN, MFRC522_RST_PIN, lock.create_unlock_token());

// !!!!!! Myserial is for Fingerprint-Sensor !!!!!!
#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)
// For UNO and others without hardware serial, we must use software serial...
// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
// Set up the serial port to use softwareserial..
SoftwareSerial mySerial(2, 3);
#else
// On Leonardo/M0/etc, others with hardware serial, use hardware serial!
// #0 is green wire, #1 is white
#define mySerial Serial1
#endif

//                      Serial-connection   default-password for database
// Fingerprint::Fingerprint finger(&mySerial);
void setup()
{
    Serial.begin(9600);
    EEPROM.begin();
    Serial.println("reading config...");
    unsigned int iterator = 0;
    char c = ' ';
    String conf;
    while (c != '\0' && iterator <= 4096)
    {
        c = static_cast<char>(EEPROM.read(iterator));
        conf += c;
        ++iterator;
    }
    Serial.println(conf);
    StaticJsonDocument<1024> config;
    config.clear();
    config.createNestedObject("Keyboard");
    config["Keyboard"]["PIN"] = "00000";
    config["Keyboard"]["enabled"] = true;

    config.createNestedObject("RFID");
    config["RFID"]["enabled"] = true;
    config["RFID"].createNestedObject("RFID_tags");
    config["RFID"]["RFID_tags"]["0"] = "1b 2f 43 23";

    config.createNestedObject("Fingerprint");
    config["Fingerprint"]["enabled"] = true;

    // serializeJsonPretty(config, Serial);
    conf = "";
    serializeJson(config, conf);
    unsigned int i = 0;
    for (i = 0; i < conf.length() && i <= 4096; ++i)
    {
        EEPROM.update(i, conf[i]);
    }
    EEPROM.update(i, '\0');
    Serial.println("Ready...");
    while (true)
        delay(1000);
    // SPI.begin();
    // rfid.PCD_Init();    // init rfid
    // rfid.PCD_DumpVersionToSerial();
    rfid.begin();
    while (!Serial)
        ;

    Serial.println("Add tag");
    while (!rfid.read_add_tag(0))
    {
        Serial.println("Add tag to reader...");
        delay(250);
    }
    for (unsigned short i = 0; i < RFID::NUM_OF_TAGS; ++i)
    {
        Serial.print(i);
        Serial.print(": ");
        Serial.println(rfid.get_tag_uid(i));
    }

    // finger.begin();
    // finger.emptyDatabase();
    //
    // finger.getParameters();
    // finger.getTemplateCount();
    // Serial.print("Templates: ");
    // Serial.println(finger.templateCount);   // how many fingerprints are stored in the sensor database
    //
    // Serial.println("Add finger: ");
    // while (!Serial.available());
    // int id = Serial.parseInt();
    // Serial.println(id);
    // finger.add_finger(id);
}

void loop()
{
    rfid.loop();
    lock.loop();
    Serial.println(lock.is_locked() == true ? "Lock is locked" : "Lock is unlocked");
    delay(1000);

    // Serial.println("ID_Check: ");
    // while (!Serial.available());
    // int id = Serial.parseInt();
    // Serial.read();
    // Serial.println(id);
    // if (finger.check_id_used(id)) {
    //     Serial.println("used model id");
    // }
    // else {
    //     Serial.println("not used model id");
    // }

    // byte b = 123;
    // Serial.println(b, BIN);
    ////b = Serial.parseInt();
    ////Serial.read();
    // Serial.println(b);
    // Serial.println(bin_to_dec(b));
    // Serial.println();
    // delay(2000);
}