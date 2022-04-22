#pragma once

#include <EEPROM.h>

namespace Config
{
    constexpr unsigned int START_OF_EEPROM = 0;
    constexpr unsigned int EEPROM_SIZE = 4096;
    enum error
    {
        NO_ERROR = 0,
        BUFFER_TO_LONG
    };
    /*
        - saves the string data in the EEPROM
        - dont forget to call in the setup-function EEPROM.begin()
        - !!! be carefully it can only be stored one string in the whole eeprom - there is only one config-file!!!
        @param data the data which will be stored in the EEPROM - be carefully the data
                has to be smaller than(EEPROM_SIZE-1)
        @return error if storing was successfully
    */
    error write_config(String data)
    {
        if (data.length() > EEPROM_SIZE - 1)
        {
            return error::BUFFER_TO_LONG;
        }
        String ret;
        unsigned int iterator;
        for (iterator = 0; iterator < data.length() || iterator < EEPROM_SIZE - 1; ++iterator)
        {
            EEPROM.update(iterator, data[iterator]);
        }
        EEPROM.update(iterator, '\0');
        return error::NO_ERROR;
    }

    /*
        reads the config from the EEPROM - if there was no config stored in
        the eeprom it reads the full eeprom
        @return the EEPROM content formated as string
    */
    String read_config()
    {
        String ret;
        char tmp = ' ';
        unsigned int iterator = 0;
        while (tmp != '\0' && iterator < EEPROM_SIZE - 1)
        {
            tmp = EEPROM.read(iterator++);
            ret += tmp;
        }
        return ret;
    }
};
