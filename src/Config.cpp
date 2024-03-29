#include "Config.hpp"
#include "system_clock/system_clock.hpp"
#include "Lock/Lock.hpp"
#include "Fingerprint.hpp"
#include "RFID/RFID.hpp"
#include "Pin.hpp"
#include "FGUI/lib/MainWindow.hpp"

#include <uEEPROMLib.h>

extern uEEPROMLib system_clock_eeprom;
extern Clock::Clock system_clock;

extern Fingerprint::Fingerprint *fingerprint;
extern RFID::RFID *rfid;
extern Pin pin;
extern Lock lock;
extern StaticJsonDocument<1024> config;
extern FGUI::MainWindow m_window;
extern Log::Log logger;

bool Config::store_locked_until_tm_pt(Clock::time_point &_tm)
{
    bool success = system_clock_eeprom.eeprom_write(LOCKED_UNTIL_TM_PT_INDICATOR_ADDR, 0x00000001);
    return success && system_clock_eeprom.eeprom_write(LOCKED_UNTIL_TM_PT_START_ADDR, &_tm, sizeof(Clock::time_point));
}

Clock::time_point *Config::read_locked_until_tm_pt(Clock::time_point *_tm_pt)
{
    if (_tm_pt == nullptr)
        return nullptr;

    // checking for error at reading
    if (!system_clock_eeprom.eeprom_read(LOCKED_UNTIL_TM_PT_START_ADDR, reinterpret_cast<byte *>(_tm_pt), sizeof(Clock::time_point)))
        return nullptr;
    else
        return _tm_pt;
}

bool Config::locked_until_tm_pt_stored()
{
    return (!system_clock.lost_power()) &&
           (!static_cast<bool>(system_clock_eeprom.eeprom_read(LOCKED_UNTIL_TM_PT_INDICATOR_ADDR)));
}

bool Config::clear_locked_until_tm_pt()
{
    return system_clock_eeprom.eeprom_write(LOCKED_UNTIL_TM_PT_INDICATOR_ADDR, 0x00000000);
}

String *Config::read_config(String *_config_str)
{
    if (_config_str == nullptr)
        return nullptr;

    byte read;
    uint16_t reading_addr = CONFIG_FILE_START_ADDR;
    do
    {
        read = system_clock_eeprom.eeprom_read(reading_addr);
        (*_config_str) += static_cast<char>(read);
        // (*_config_str) += static_cast<char>(system_clock_eeprom.eeprom_read(reading_addr));

        /*
        Serial.print("reading_addr: ");
        Serial.print(reading_addr);
        Serial.print(" - ");
        Serial.println((char)read);
        */

        ++reading_addr;
    } while (read != '\0' && reading_addr < CONFIG_FILE_END_ADDR);

    if (_config_str->charAt(_config_str->length() - 1) == '\0')
        _config_str->remove(_config_str->length() - 1);

    // Serial.println();
    // Serial.println((*_config_str));
    return _config_str;
}

bool Config::write_config(String *_config_str)
{
    // Serial.println("writing config...");
    if (_config_str == nullptr)
        return false;
    if (!check_config_length(*_config_str))
        return false;

    // Serial.println("writing following config str:");
    // Serial.println((*_config_str));

    bool success = system_clock_eeprom.eeprom_write(CONFIG_FILE_START_ADDR,
                                                    const_cast<char *>(_config_str->c_str()),
                                                    _config_str->length()); // every count of length is one char/byte

    // adding eof if the _config_str doesnt fills the whole memory for endmarking at reading
    if (success && (_config_str->length() + CONFIG_FILE_START_ADDR < CONFIG_FILE_END_ADDR))
        success = system_clock_eeprom.eeprom_write(CONFIG_FILE_START_ADDR + _config_str->length(), '\0');
    return success;
}

bool Config::check_config_length(const String &_config_str)
{
    return _config_str.length() <= CONFIG_FILE_END_ADDR - CONFIG_FILE_START_ADDR;
}

String &Config::create_config_str(String &_config_str)
{
    auto RFID_tags_ref = config[F("RFID")][F("RFID_tags")];
    RFID_tags_ref.clear();
    for (uint8_t id = 0; id < RFID::NUM_OF_TAGS; ++id)
    {
        if (rfid->id_used(id))
        {
            RFID_tags_ref.add(JsonObject());
            RFID_tags_ref[RFID_tags_ref.size() - 1][F("id")] = id;
            auto uid = rfid->get_tag_uid(id);
            for (uint8_t i = 0; i < uid.get_uid_length(); ++i)
            {
                RFID_tags_ref[RFID_tags_ref.size() - 1][F("tag_uid")].add(uid.get_uid()[i]);
            }

            // RFID_tags_ref[RFID_tags_ref.size() - 1][F("tag_uid")] = rfid->get_tag_uid(id).to_string(); // as string we are not using
        }
    }

    config[F("PIN")][F("pin")] = pin.get_pin();
    config[F("PIN")][F("enabled")] = pin.is_enabled();

    config[F("Fingerprint")][F("enabled")] = fingerprint->is_enabled();

    auto system_ref = config[F("system")];
    system_ref[F("locking_period")] = lock.get_locking_period();
    system_ref[F("allowed_unauthorized_unlock_tries")] = lock.get_allowed_unauthorized_unlock_tries();
    system_ref[F("relock_timer")] = lock.get_relock_timer();
    system_ref[F("screen_timeout")] = m_window.get_fall_aspleep_timer();
    system_ref[F("logging_level")] = logger.get_logging_level();

    // creating the config string
    serializeJson(config, _config_str);
    return _config_str;
}

const char *Config::reset_config()
{
    return "{\"Fingerprint\":{\"enabled\":false},\"PIN\":{\"enabled\":false,\"pin\":\"0\"},\"RFID\":{\"enabled\":false,\"RFID_tags\":[]},\"system\":{\"allowed_unauthorized_unlock_tries\":10,\"locking_period\":60,\"screen_timeout\":60,\"relock_timer\":10,\"logging_level\":5}}";
}