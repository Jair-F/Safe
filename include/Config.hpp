#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include "Fingerprint.hpp"
#include "RFID/RFID.hpp"
#include "FGUI/FGUI.hpp"
#include "system_clock/time_point.hpp"
#include "system_clock/system_clock.hpp"
// #include "pin..."

extern StaticJsonDocument<1024> config;
extern Fingerprint::Fingerprint *fingerprint;
extern RFID::RFID *rfid;
extern FGUI::MainWindow m_window;
extern Lock lock;

namespace Config
{
    constexpr uint16_t LOCKED_UNTIL_TM_PT_INDICATOR_ADDR = 0x000; // 0x00000000 = no locked_until_tm_pt saved - 0x00000001 if yes
    constexpr uint16_t LOCKED_UNTIL_TM_PT_START_ADDR = 0x001;
    constexpr uint16_t LOCKED_UNTIL_TM_PT_END_ADDR = sizeof(Clock::time_point) + LOCKED_UNTIL_TM_PT_START_ADDR;
    constexpr uint16_t CONFIG_FILE_START_ADDR = LOCKED_UNTIL_TM_PT_END_ADDR + 1;
    constexpr uint16_t CONFIG_FILE_END_ADDR = SYSTEM_CLOCK_EEPROM_LAST_ADRESS;

    /**
     * @return true on success
     */
    bool store_locked_until_tm_pt(Clock::time_point &);

    /**
     * @param _tm_pt where the _tm_pt should be stored in
     * @return _tm_pt on success, nullptr on fail
     * @note on fail the data of _tm_pt is undefined
     */
    Clock::time_point *read_locked_until_tm_pt(Clock::time_point *_tm_pt);

    /**
     * @return true if a locked_until_time_point is stored in the memory
     * @details depends on if the clock lost power and the indicator value
     */
    bool locked_until_tm_pt_stored();
    /**
     * @details clears the indicator for locked_until_tm_pt
     * @return true on success
     */
    bool clear_locked_until_tm_pt();

    /**
     * @param _config_str string in which the config is stored
     * @return _config_str on success, on fail nullptr
     * @note on fail the data of _config_str is undefined
     */
    String *read_config(String *_config_str);
    /**
     * @param _config_str the config string string to store in the memory
     * @return true on success
     * @note check that _config_str gives no overflow
     */
    bool write_config(String *_config_str);

    /**
     * @return true if _config_str is short enough to be stored in the memory,
     * @details also false if system_clock lost power
     */
    bool check_config_length(const String &_config_str);

    /*







    */

    /*
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
     */

    /**
     * @note !!dont forget to call filesystem.begin() and create/initialize all sensor objects
     * which will be set by the config
     *
     * @return true on success
     */
    /*
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
        // if (config[F("PIN")][F("enabled")])
        // {
        //     pin->enable();
        // }
        // else
        // {
        //     pin->disable();
        // }
        // String pin = config[F("PIN")][F("pin")];

        // MainWindow sleep_timeout
        m_window.set_fall_asleep_timer(config[F("system")][F("sleep_timeout")]);

        // initializing the lock
        lock.set_allowed_unauthorized_unlock_tries(config[F("system")][F("allowed_unauthorized_unlock_tries")]);
        lock.set_locking_period(config[F("system")][F("locking_period")]);
        // lock.set_lock_timer(config[F("system")][F("lock_timer")]);
    }
    */
};
