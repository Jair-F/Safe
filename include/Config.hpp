#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include <SD.h>
#include "Fingerprint.hpp"
#include "RFID/RFID.hpp"
#include "FGUI/FGUI.hpp"
// #include "pin..."

extern StaticJsonDocument<1024> config;
extern SDClass filesystem;
extern Fingerprint::Fingerprint *fingerprint;
extern RFID::RFID *rfid;
extern FGUI::MainWindow m_window;
extern Lock lock;

namespace Config
{
    bool write_config(File &config_file)
    {
        if (!config_file.availableForWrite())
        {
            return false;
        }

        config[F("Fingerprint")][F("enabled")] = fingerprint->is_enabled();

        config[F("RFID")][F("enabled")] = rfid->is_enabled();
        JsonArray rfid_tags = config[F("RFID")][F("RFID_tags")];
        rfid_tags.clear();

        // iterate over the rfid tags and add them to the config
        RFID::UID *allowed_tags = rfid->_get_allowed_tags();
        for (uint8_t id = 0; id < RFID::NUM_OF_TAGS; ++id)
        {
            if (allowed_tags[id].is_set())
            {
                rfid_tags.createNestedArray();
                auto tag = rfid_tags[rfid_tags.size() - 1];

                tag.createNestedObject(F("id"));
                tag.createNestedArray(F("tag_uid"));

                tag[F("id")] = id;
                for (uint8_t i = 0; i < allowed_tags[id].get_uid_length(); ++i)
                {
                    tag[F("tag_uid")].createNestedObject();
                    uint8_t last_element_id = tag[F("tag_uid")].size();
                    tag[F("tag_uid")][last_element_id] = allowed_tags[i].get_uid()[i];
                }
            }
        }

        // MainWindow sleep_timeout
        config[F("system")][F("sleep_timeout")] = m_window.get_fall_aspleep_timer();

        // initializing the lock
        config[F("system")][F("allowed_unauthorized_unlock_tries")] = lock.get_allowed_unauthorized_unlock_tries();
        config[F("system")][F("locking_period")] = lock.get_locking_period();
        config[F("system")][F("lock_timer")] = lock.get_lock_timer();

        serializeJson(config, config_file);
    }

    /**
     * @note !!dont forget to call filesystem.begin() and create/initialize all sensor objects
     * which will be set by the config
     *
     * @return true on success
     */
    bool read_config(File &config_file)
    {
        if (!config_file.available())
        {
            return false; // error at opening the file
        }

        deserializeJson(config, config_file);

        // initializing fingerprint
        if (config[F("Fingerprint")][F("enabled")].as<bool>())
        {
            fingerprint->enable();
        }
        else
        {
            fingerprint->disable();
        }

        // initializing RFID
        if (config[F("RFID")][F("enabled")].as<bool>())
        {
            rfid->enable();
        }
        else
        {
            rfid->disable();
        }
        JsonArray rfid_tags = config[F("RFID")][F("RFID_tags")];
        for (JsonObject rfid_tag : rfid_tags)
        {
            JsonArray j_tag_uid = rfid_tag[F("tag_uid")];

            uint8_t tag_uid_id = rfid_tag[F("id")].as<uint8_t>();
            uint8_t tag_uid_size = j_tag_uid.size();
            uint8_t *tmp_tag_uid = new uint8_t[tag_uid_size];

            uint8_t pos_counter = 0;
            for (auto tag_uid_byte : j_tag_uid) // with iterator its more efficient
            {
                tmp_tag_uid[pos_counter] = tag_uid_byte.as<uint8_t>();
                ++pos_counter;
            }

            rfid->add_tag(tag_uid_id, RFID::UID(tmp_tag_uid, tag_uid_size));

            delete[] tmp_tag_uid;
        }

        // initializing PIN
        /*
        if (config[F("PIN")][F("enabled")])
        {
            pin->enable();
        }
        else
        {
            pin->disable();
        }
        String pin = config[F("PIN")][F("pin")];
        */

        // MainWindow sleep_timeout
        m_window.set_fall_asleep_timer(config[F("system")][F("sleep_timeout")]);

        // initializing the lock
        lock.set_allowed_unauthorized_unlock_tries(config[F("system")][F("allowed_unauthorized_unlock_tries")]);
        lock.set_locking_period(config[F("system")][F("locking_period")]);
        // lock.set_lock_timer(config[F("system")][F("lock_timer")]);
    }
};
