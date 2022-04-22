#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

#include "GlobalConstants.hpp"
#include "Fingerprint.hpp"
#include "RFID/RFID.hpp"
#include "Config.hpp"
#include "Lock.hpp"
#include "Pin.hpp"
#include "system_clock.hpp"
#include "Helper.hpp"
//#include "logging/Log.hpp"

namespace Fase
{
    // Name of Safe: fase

    /*
        the windows on the display - the different modes to make changes in the settings
    */
    enum Mode
    {
        NORMAL = 0, // Normal - check if there is nothing a unlock request/a user wants to open...
        GLOBAL_SETTINGS,
        FINGERPRINT_ROOT_SETTINGS,
        FINGERPRINT_ADD_FINGER,
        FINGERPRINT_REMOVE_FINGER,
        FINGERPRINT_EMPTY_DATABASE,
        RFID_ROOT_SETTINGS,
        RFID_ADD_TAG,
        RFID_REMOVE_TAG,
        RFID_EMPTY_DATABASE,
        PIN_ROOT_SETTINGS
    };

    class Fase
    {
    public:
        Fase();
        ~Fase();
        void loop();
        // begin all the devices
        void begin();

    protected:
        void save_config();
        void read_config();
        void reset_config();

        void add_fingerprint(unsigned short id);
        void delete_fingerprint();
        void change_pin();
        /*
            @return true if a tag was read and added - false if no tag was read
        */
        bool add_RFID_tag(unsigned short id, bool force_overwrite = false);

        /*
            @return true if a tag was scanned and the tag was removed - false if
                    no tag was scanned
         */
        bool delete_RFID_tag_by_scan();

    private:
        const unsigned short lock_timer = 5; // timer in seconds the lock should lock again after its unlocked

// !!!!!! Myserial is for Fingerprint-Sensor !!!!!!
// SoftwareSerial mySerial;
#define mySerial Serial1

        Mode mode;
        Lock::Lock lock;
        StaticJsonDocument<1024> config;

        Fingerprint::Fingerprint fingerprint;
        RFID::RFID rfid;
        Pin::Pin pin;
    };
}

// ---------------- Implementations ----------------

Fase::Fase::Fase() : /*mySerial(SERIAL_RECEIVE_PIN, SERIAL_TRANSMIT_PIN),*/ lock(this->lock_timer),
                     fingerprint(&mySerial, this->lock.create_unlock_token()),
                     rfid(MFRC522_SS_PIN, MFRC522_RST_PIN, lock.create_unlock_token()),
                     pin(lock.create_unlock_token())
{
    mode = Mode::NORMAL;
}

Fase::Fase::~Fase() {}

void Fase::Fase::begin()
{
    this->rfid.begin();
    this->fingerprint.begin();
    for (unsigned short i = 0; i < RFID::NUM_OF_TAGS; ++i)
    {
        if (this->rfid.id_used(i)) // id is used
        {
            String msg = "Tag ";
            msg += i;
            msg += " is used: ";
            msg += rfid.get_tag_uid(i).to_string();
            DEBUG_PRINT(msg)
        }
    }
    this->read_config();
}

void Fase::Fase::loop()
{
    switch (mode)
    {
    case Mode::NORMAL:
    {
        fingerprint.loop();
        rfid.loop();

        // pin.loop();
        lock.loop();
        break;
    }
    case Mode::FINGERPRINT_ROOT_SETTINGS:
    {
        break;
    }

    case Mode::FINGERPRINT_ADD_FINGER:
    {
        Serial.println(F("Add-Finger - insert id..."));
        while (!Serial.available() > 0)
            delay(100);
        unsigned short id = Serial.parseInt();
        this->add_fingerprint(id);
        mode = Mode::NORMAL;
        break;
    }

    case Mode::FINGERPRINT_REMOVE_FINGER:
    {
        // delete finger
        Serial.println(F("remove finger with scan(type 1) - remove finger with id(type 0): "));
        Serial.println(F("INPUT: "));
        while (!(Serial.available() > 0))
            delay(100);
        String input = Serial.readString();
        input.remove(input.length() - 1);
        if (input == "1") /*remove with read_finger*/
        {
            Serial.println(F("Place finger"));
            uint8_t err_code;
            while (err_code != FINGERPRINT_OK)
                err_code = this->fingerprint.getImage();

            err_code = this->fingerprint.image2Tz(1); // converting to a template in order to search for matches
            err_code = this->fingerprint.fingerFastSearch();

            if (err_code == FINGERPRINT_OK) // found a match - got id
            {
                err_code = this->fingerprint.deleteModel(this->fingerprint.fingerID);
                if (err_code != FINGERPRINT_OK)
                {
                    Serial.print(F("ERROR: "));
                    Serial.println(Fingerprint::error_code_message(err_code));
                }
                else
                {
                    Serial.println(F("Found a match - fingeprint removed"));
                }
            }
            else
            {
                Serial.println(F("Didnt found a match - didnt removed anything"));
                Serial.println(Fingerprint::error_code_message(err_code));
            }
            mode = Mode::NORMAL;
        }
        else
        {
            Serial.println(F("please enter id"));
            while (!(Serial.available() > 0)) // wait until there is sometihing in serial monitor
                delay(100);
            unsigned short id = Serial.parseInt();
            uint8_t err_code = this->fingerprint.deleteModel(id);
            if (err_code != FINGERPRINT_OK)
            {
                Serial.print(F("ERROR: "));
                Serial.println(Fingerprint::error_code_message(err_code));
            }
        }
        break;
    }

    case Mode::FINGERPRINT_EMPTY_DATABASE:
    {
        // delte database
        uint8_t err_code = this->fingerprint.emptyDatabase();
        if (err_code == FINGERPRINT_OK)
        {
            Serial.println(F("cleared database..."));
        }
        else
        {
            Serial.print(F("ERROR: "));
            Serial.println(Fingerprint::error_code_message(err_code));
        }
        this->mode = Mode::NORMAL;
        break;
    }
    case Mode::RFID_ROOT_SETTINGS:
    {
        break;
    }
    case Mode::RFID_ADD_TAG:
    {
        Serial.println(F("please enter id"));
        while (!(Serial.available() > 0))
            delay(100);
        Serial.flush();
        unsigned short id = Serial.parseInt();
        Serial.println(F("Add tag..."));
        while (!this->add_RFID_tag(id, true)) // wait until user added tag
        {
            Serial.print(F("."));
            delay(500);
        }
        Serial.println();
        Serial.println(F("Tag added..."));
        save_config();
        this->mode = Mode::NORMAL;
        break;
    }
    case Mode::RFID_REMOVE_TAG:
    {
        Serial.println(F("remove tag with scan(type 1) - remove tag with id(type 0): "));
        Serial.println(F("INPUT: "));
        while (!(Serial.available() > 0))
            delay(100);
        Serial.flush();
        String input = Serial.readString();
        input.remove(input.length() - 1);

        if (input == "1") /*remove with tag_scan*/
        {
            Serial.println(F("Add tag"));
            RFID::UID tag_uid;
            tag_uid = this->rfid.read_Tag_UID(false);
            if (this->rfid.remove_tag(tag_uid))
            {
                Serial.println(F("Tag removed"));
            }
            else
            {
                Serial.println(F("tag was not in allowed_tags - didnt removed something..."));
            }
        }
        else
        {
            Serial.println(F("please enter id"));
            while (!(Serial.available() > 0))
                delay(100);
            unsigned short tag_id = Serial.parseInt();
            this->rfid.remove_tag(tag_id);
            Serial.println(F("Tag removed"));
        }
        save_config();
        mode = Mode::NORMAL;
        break;
    }
    case Mode::RFID_EMPTY_DATABASE:
    {
        this->rfid.clear_database();
        Serial.println(F("cleared database"));
        save_config();
        mode = Mode::NORMAL;
        break;
    }
    default:
    {
        break;
    }
    }

    // Serial.print("Lock-State: ");
    // Serial.println(this->lock.is_locked() == true ? "locked" : "unlocked");

    if (Serial.available() > 0)
    {
        Serial.flush(); // wait till the stream finished
        if (Serial.read() == '$')
        {
            Serial.println(F("Command send..."));
            String read = Serial.readString();
            read.remove(read.length() - 1); // remove last newline
            if (read == F("help"))
            {
                Serial.print(F("Commands: "));
                Serial.println(F("$add_finger, $normal, $rfid_remove_tag, $rfid_add_tag, $rfid_empty_database, $finger_empty_database, $remove_finger, $reset_config, $reset_time"));
                Serial.println();
            }
            else if (read == F("remove_finger"))
            {
                this->mode = Mode::FINGERPRINT_REMOVE_FINGER;
            }
            else if (read == F("add_finger"))
            {
                Serial.println(F("Setting mode to addfinger"));
                this->mode = Mode::FINGERPRINT_ADD_FINGER;
            }
            else if (read == F("finger_empty_database"))
            {
                this->mode = Mode::FINGERPRINT_EMPTY_DATABASE;
            }
            else if (read == F("normal"))
            {
                Serial.println(F("Setting mode to normal"));
                this->mode = Mode::NORMAL;
            }
            else if (read == F("rfid_add_tag"))
            {
                Serial.println(F("Setting mode to add_rfid_tag"));
                this->mode = Mode::RFID_ADD_TAG;
            }
            else if (read == F("rfid_remove_tag"))
            {
                this->mode = Mode::RFID_REMOVE_TAG;
            }
            else if (read == F("rfid_empty_database"))
            {
                Serial.println(F("Setting mode to empty_database"));
                this->mode = Mode::RFID_EMPTY_DATABASE;
            }
            else if (read == F("reset_config"))
            {
                reset_config();
                Serial.println(F("config reseted"));
            }
            else if (read == F("reset_time"))
            {
                RtcDateTime now(__DATE__, __TIME__);
                system_clock.SetDateTime(now);
                String msg = F("Time-Date reseted to: ");
                msg += __DATE__;
                msg += ' ';
                msg += __TIME__;
                Serial.println(msg);
            }
            else
            {
                Serial.print(F("Unknown command "));
                Serial.println(read);
            }
        }
    }
}

bool Fase::Fase::add_RFID_tag(unsigned short id, bool force_overwrite)
{
    if (!this->rfid.id_used(id) || force_overwrite)
    {
        return this->rfid.read_add_tag(id);
    }
    Serial.print(F("ID "));
    Serial.print(id);
    Serial.println(F(" is already used..."));
    return false;
}

bool Fase::Fase::delete_RFID_tag_by_scan()
{
    RFID::UID tag_uid = this->rfid.read_Tag_UID();
    if (tag_uid) // no tag was read
    {
        return false;
    }
    return this->rfid.remove_tag(tag_uid);
}

void Fase::Fase::add_fingerprint(unsigned short id)
{
    uint8_t err_code = FINGERPRINT_NOFINGER;
    Serial.println(F("Add-Finger: "));
    Serial.println(F("place finger..."));
    while (err_code != FINGERPRINT_OK) // wating until user places the finger
    {
        err_code = this->fingerprint.getImage();
        Serial.println(Fingerprint::error_code_message(err_code));
        // delay(200);
    }
    Serial.println(F("image taken"));

    // converting image to template-model in slot 1
    err_code = this->fingerprint.image2Tz(1);
    if (err_code != FINGERPRINT_OK)
    {
        Serial.print(F("ERROR at converting image to template model: "));
        Serial.println(Fingerprint::error_code_message(err_code));
        // delay(200);
        //  exit the add-finger mode
    }

    Serial.println(F("Take finger off...."));
    while (err_code != FINGERPRINT_NOFINGER) // wating until user takes the finger of the sensor
        err_code = this->fingerprint.getImage();

    Serial.println(F("Place same finger again..."));
    while (err_code != FINGERPRINT_OK) // wating until user places the finger
        err_code = this->fingerprint.getImage();

    // converting image to template-model in slot 2
    err_code = this->fingerprint.image2Tz(2);
    if (err_code != FINGERPRINT_OK)
    {
        Serial.print(F("ERROR at converting image to template model: "));
        Serial.println(Fingerprint::error_code_message(err_code));
        delay(200);
        // exit the add-finger mode
    }

    err_code = this->fingerprint.createModel();
    if (err_code != FINGERPRINT_OK)
    {
        Serial.print(F("ERROR at creating the fingerprint model: "));
        Serial.println(Fingerprint::error_code_message(err_code));
        delay(200);
        // exit the add-finger mode
    }
    err_code = this->fingerprint.storeModel(id);
    if (err_code != FINGERPRINT_OK)
    {
        Serial.print(F("ERROR at storing the fingerprint model: "));
        Serial.println(Fingerprint::error_code_message(err_code));
        delay(200);
        // exit the add-finger mode
    }
    Serial.println(F("Stored Fingerprint-Model"));
}

void Fase::Fase::reset_config()
{
    const char *default_config = "{\"Keyboard\":{\"PIN\":\"\",\"enabled\":false},\"RFID\":{\"enabled\":true,\"RFID_tags\":[]},\"Fingerprint\":{\"enabled\":true}}";
    config.clear();
    DeserializationError error = deserializeJson(config, default_config);
    if (error)
    {
        Serial.print(F("ERROR while reseting the config: Code: "));
        Serial.print(error.code());
        Serial.print(F(" Message: "));
        Serial.println(error.c_str());
    }
    serializeJsonPretty(config, Serial);

    String config_str;
    serializeJson(config, config_str);
    Config::error err = Config::write_config(config_str);
    if (!err == Config::error::NO_ERROR)
    {
        Serial.println(F("An error occoured while saving the config..."));
    }
}

void Fase::Fase::save_config()
{
    auto RFID_tags_ref = config[F("RFID")][F("RFID_tags")];
    // clearing all rfid_tags from the config
    RFID_tags_ref.clear();
    // saving rfid_tags in the config...
    DEBUG_PRINT(F("Reading tags..."))
    for (unsigned short i = 0; i < RFID::NUM_OF_TAGS; ++i)
    {
        String msg = F("Tag num: ");
        msg += i;
        if (this->rfid.id_used(i))
        {
            DEBUG_PRINT(this->rfid.get_tag_uid(i).to_string())
            msg += F(" -- is used...");
            RFID_tags_ref.add(JsonObject());
            RFID_tags_ref[RFID_tags_ref.size() - 1][F("id")] = i;
            RFID_tags_ref[RFID_tags_ref.size() - 1][F("tag_uid")] = this->rfid.get_tag_uid(i).to_string();
        }
        DEBUG_PRINT(msg)
    }

    // printing tags to serial
    serializeJsonPretty(config, Serial);

    String config_str = "";
    serializeJson(config, config_str);
    Config::error error = Config::write_config(config_str);
    if (!error == Config::error::NO_ERROR)
    {
        Serial.println(F("An error occoured while saving the config..."));
    }
    // deleting the tags from the config - save memory
    RFID_tags_ref.clear();
}

void Fase::Fase::read_config()
{
    String config_str = Config::read_config();
    DeserializationError error = deserializeJson(config, config_str);
    if (error)
    {
        Serial.print(F("ERROR while reading the config: Code: "));
        Serial.print(error.code());
        Serial.print(F(" Message: "));
        Serial.println(error.c_str());
    }

    config_str = "";
    serializeJsonPretty(config, config_str);
    DEBUG_PRINT(config_str)
    config_str = "";

    // initializing pin-keyboard
    if (config[F("Keyboard")][F("enabled")])
    {
        this->pin.enable();
    }
    else
    {
        this->pin.disable();
    }

    // initializing Fingerprint
    if (config[F("Fingerprint")][F("enabled")])
    {
        this->fingerprint.enable();
    }
    else
    {
        this->fingerprint.disable();
    }

    // initializing RFID
    if (config[F("RFID")][F("enabled")])
    {
        this->rfid.enable();
    }
    else
    {
        this->rfid.disable();
    }
    auto RFID_tags_ref = config[F("RFID")][F("RFID_tags")];
    for (unsigned short i = 0; i < RFID_tags_ref.size(); ++i)
    {
        if (i < RFID::NUM_OF_TAGS)
        {
            String tag_uid_str = RFID_tags_ref[i][F("tag_uid")];
            DEBUG_PRINT(tag_uid_str)
            RFID::UID uid(tag_uid_str);
            if (uid) // if uid is a valid uid
            {
                this->rfid.add_tag(RFID_tags_ref[i][F("id")], uid);
            }
        }
    }
    // deleting the tags from the config - save memory
    RFID_tags_ref.clear();
}
