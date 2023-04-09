#include "Config.hpp"
#include "system_clock/system_clock.hpp"

#include <uEEPROMLib.h>

extern uEEPROMLib system_clock_eeprom;
extern Clock::Clock system_clock;

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
    return (!static_cast<bool>(system_clock_eeprom.eeprom_read(LOCKED_UNTIL_TM_PT_INDICATOR_ADDR))) &&
           (!system_clock.lost_power());
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
        system_clock_eeprom.eeprom_read(reading_addr);
    } while (read != '\0' && reading_addr < CONFIG_FILE_END_ADDR);
    return _config_str;
}

bool Config::write_config(String *_config_str)
{
    if (_config_str == nullptr)
        return false;
    if (!check_config_length(*_config_str))
        return false;

    bool success = system_clock_eeprom.eeprom_write(CONFIG_FILE_START_ADDR,
                                                    _config_str,
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