#pragma once

#include <ArduinoJson.h>
#include <OSFS.h>
#include "GlobalConstants.hpp"
#include "Fingerprint.hpp"
#include "RFID/RFID.hpp"
#include "Config.hpp"
#include "Lock.hpp"
#include "Pin.hpp"
#include "../lib/exception.hpp"

namespace Fase
{
    // Name of Safe: fase
    enum Mode
    {
        Normal = 0, // Normal - check if there is nothing a unlock request/a user wants to open...
        Global_Settings,
        Fingerprint_Settings,
        RFID_Settings,
        Pin_Settings
    };

    class Fase
    {
    public:
        Fase();
        ~Fase();
        void loop();

    protected:
        void save_config();
        void read_config();
        void reset_config();

        void add_fingerprint();
        void delete_fingerprint();
        void change_pin();
        /*
            @return true if a tag was read and added - false if no tag was read
        */
        bool add_RFID_tag(unsigned short id);

        /*
            @return true if a tag was scanned and the tag was removed - false if
                    no tag was scanned
         */
        bool delete_RFID_tag_by_scan();

    private:
        const unsigned short lock_timer = 5; // timer in seconds the lock should lock again after its unlocked

        // !!!!!! Myserial is for Fingerprint-Sensor !!!!!!
        SoftwareSerial mySerial;

        Mode mode;
        Lock::Lock lock;
        StaticJsonDocument<1024> config;

        Fingerprint::Fingerprint fingerprint;
        RFID::RFID rfid;
        Pin::Pin pin;
    };
}

// ---------------- Implementations ----------------

Fase::Fase::Fase() : mySerial(SERIAL_RECEIVE_PIN, SERIAL_TRANSMIT_PIN), lock(this->lock_timer),
                     fingerprint(&mySerial, this->lock.create_unlock_token()),
                     rfid(MFRC522_SS_PIN, MFRC522_RST_PIN, lock.create_unlock_token()),
                     pin(lock.create_unlock_token())
{
    mode = Mode::Normal;
}

void Fase::Fase::loop()
{
    switch (mode)
    {
    case Mode::Normal:
    {
        fingerprint.loop();
        rfid.loop();
        pin.loop();
        lock.loop();
        break;
    }
    case Mode::Fingerprint_Settings:
    {
        enum
        {
            FINGERPRINT_ROOT_SETTINGS_PAGE,
            ADD_FINGER,
            DELETE_FINGER,
            EMPTY_DATABASE
        } FINGERPRINT_SETTINGS_STATE = FINGERPRINT_ROOT_SETTINGS_PAGE;

        switch (FINGERPRINT_SETTINGS_STATE)
        {
        /*
            The root page where we select wether to add a finger or delete or to enable or disable the sensor...
         */
        case FINGERPRINT_ROOT_SETTINGS_PAGE:
        {
            break;
        }

        case ADD_FINGER:
        {
            this->add_fingerprint();
            break;
        }

        case DELETE_FINGER:
        {
            // delete finger
            if (false /*remove with read_finger*/)
            {
                uint8_t err_code;
                while (err_code != FINGERPRINT_OK)
                    err_code = this->fingerprint.getImage();

                err_code = this->fingerprint.fingerFastSearch();
                if (err_code == FINGERPRINT_OK) // found a match - got id
                {
                    err_code = this->fingerprint.deleteModel(this->fingerprint.fingerID);
                    if (err_code != FINGERPRINT_OK)
                    {
                        Serial.print("ERROR: ");
                        Serial.println(Fingerprint::error_code_message(err_code));
                    }
                    else
                    {
                        Serial.println("Found a match - fingeprint removed");
                    }
                }
                else
                {
                    Serial.println("Didnt found a match - didnt removed anything");
                    Serial.println(Fingerprint::error_code_message(err_code));
                }
            }
            else
            {
                unsigned short id = Serial.parseInt();
                Serial.read();
                uint8_t err_code = this->fingerprint.deleteModel(id);
                if (err_code != FINGERPRINT_OK)
                {
                    Serial.print("ERROR: ");
                    Serial.println(Fingerprint::error_code_message(err_code));
                }
            }
            break;
        }

        case EMPTY_DATABASE:
        {
            // delte database
            break;
        }

        default: // if none
            break;
        }
    }
    case Mode::RFID_Settings:
    {
        enum
        {
            RFID_ROOT_SETTINGS_PAGE,
            ADD_TAG,
            DELETE_TAG,
            EMPTY_DATABASE
        } RFID_SETTINGS_STATE = RFID_ROOT_SETTINGS_PAGE;
        switch (RFID_SETTINGS_STATE)
        {
        case ADD_TAG:
        {
            Serial.println("please enter id");
            unsigned short id = Serial.parseInt();
            Serial.read();
            Serial.println("Add tag...");
            while (!this->add_RFID_tag(id)) // wait until user added tag
            {
            }
            break;
        }
        case DELETE_TAG:
        {
            if (false /*delte tag by scan*/)
            {
                while (delete_RFID_tag_by_scan()) // wait until user added tag
                    ;
            }
            else
            {
                Serial.println("Enter id");
                unsigned short id = Serial.parseInt();
                Serial.read();
                this->rfid.remove_tag(id);
            }
            break;
        }
        case EMPTY_DATABASE:
        {
            this->rfid.clear_database();
            break;
        }
        default:
            break;
        }
        break;
    }

    default:
        break;
    }
}

bool Fase::Fase::add_RFID_tag(unsigned short id)
{
    if (!this->rfid.id_used(id))
    {
        return this->rfid.read_add_tag(id);
    }
    Serial.print("ID ");
    Serial.print(id);
    Serial.println("is already used...");
    return false;
}

bool Fase::Fase::delete_RFID_tag_by_scan()
{
    String tag_uid = this->rfid.read_Tag_ID();
    if (tag_uid.length() == 0) // no tag was read
    {
        return false;
    }
    return this->rfid.remove_tag(tag_uid);
}

void Fase::Fase::add_fingerprint()
{
    uint8_t err_code = FINGERPRINT_NOFINGER;
    Serial.println("Add-Finger: ");
    Serial.println("place finger...");
    while (err_code != FINGERPRINT_OK) // wating until user places the finger
    {
        err_code = this->fingerprint.getImage();
        Serial.println(Fingerprint::error_code_message(err_code));
        delay(200);
    }
    Serial.print("image taken");

    // converting image to template-model in slot 1
    err_code = this->fingerprint.image2Tz(1);
    if (err_code != FINGERPRINT_OK)
    {
        Serial.print("ERROR at converting image to template model: ");
        Serial.println(Fingerprint::error_code_message(err_code));
        delay(200);
        // exit the add-finger mode
    }

    Serial.println("Take finger off....");
    while (err_code != FINGERPRINT_NOFINGER) // wating until user takes the finger of the sensor
        err_code = this->fingerprint.getImage();

    Serial.println("Place same finger again...");
    while (err_code != FINGERPRINT_OK) // wating until user places the finger
        err_code = this->fingerprint.getImage();

    // converting image to template-model in slot 2
    err_code = this->fingerprint.image2Tz(2);
    if (err_code != FINGERPRINT_OK)
    {
        Serial.print("ERROR at converting image to template model: ");
        Serial.println(Fingerprint::error_code_message(err_code));
        delay(200);
        // exit the add-finger mode
    }

    err_code = this->fingerprint.createModel();
    if (err_code != FINGERPRINT_OK)
    {
        Serial.print("ERROR at creating the fingerprint model: ");
        Serial.println(Fingerprint::error_code_message(err_code));
        delay(200);
        // exit the add-finger mode
    }
    err_code = this->fingerprint.storeModel(0);
    if (err_code != FINGERPRINT_OK)
    {
        Serial.print("ERROR at storing the fingerprint model: ");
        Serial.println(Fingerprint::error_code_message(err_code));
        delay(200);
        // exit the add-finger mode
    }
}

void Fase::Fase::reset_config()
{
    const char *default_config = "{\"Keyboard\":{\"PIN\":\"\",\"enabled\":false},\"RFID\":{\"enabled\":false,\"RFID_tags\":[]},\"Fingerprint\":{\"enabled\":false}}";
    config.clear();
    DeserializationError error = deserializeJson(config, default_config);
    if (error)
    {
        Serial.print("ERROR while reseting the config: Code: ");
        Serial.print(error.code());
        Serial.print(" Message: ");
        Serial.println(error.c_str());
    }
    serializeJsonPretty(config, Serial);
}

void Fase::Fase::save_config()
{
    auto RFID_tags_ref = config["RFID"]["RFID_tags"];
    // clearing all rfid_tags from the config
    RFID_tags_ref.clear();
    // saving rfid_tags in the config...
    for (unsigned short i = 0; i < RFID::NUM_OF_TAGS; ++i)
    {
        if (this->rfid.id_used(i))
        {
            RFID_tags_ref.add(JsonObject());
            RFID_tags_ref[RFID_tags_ref.size() - 1]["id"] = i;
            RFID_tags_ref[RFID_tags_ref.size() - 1]["tag_uid"] = this->rfid.get_tag_uid(i);
        }
    }
    // deleting the tags from the config - save memory
    RFID_tags_ref.clear();

    serializeJsonPretty(config, Serial);
    return;

    String config_str = "";
    serializeJson(config, config_str);
    Config::error error = Config::write_config(config_str);
    if (!error == Config::error::NO_ERROR)
    {
        Serial.println("An error occoured while saving the config...");
    }
}

void Fase::Fase::read_config()
{
    String config_str = Config::read_config();
    DeserializationError error = deserializeJson(config, config_str);
    if (error)
    {
        Serial.print("ERROR while reading the config: Code: ");
        Serial.print(error.code());
        Serial.print(" Message: ");
        Serial.println(error.c_str());
    }

    // initializing pin-keyboard
    if (config["Keyboard"]["enabled"])
    {
        this->pin.enable();
    }
    else
    {
        this->pin.disable();
    }

    // initializing Fingerprint
    if (config["Fingerprint"]["enabled"])
    {
        this->fingerprint.enable();
    }
    else
    {
        this->fingerprint.disable();
    }

    // initializing RFID
    if (config["RFID"]["enabled"])
    {
        this->rfid.enable();
    }
    else
    {
        this->rfid.disable();
    }
    auto RFID_tags_ref = config["RFID"]["RFID_tags"];
    for (unsigned short i = 0; i < RFID_tags_ref.size(); ++i)
    {
        this->rfid.add_tag(RFID_tags_ref[i]["id"], RFID_tags_ref[i]["tag_uid"]);
    }
    // deleting the tags from the config - save memory
    RFID_tags_ref.clear();
}
