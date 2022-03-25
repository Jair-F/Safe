#pragma once
#include <MFRC522.h>
#include <Arduino.h>
#include "../Helper.hpp"

namespace RFID
{
    //                                      F = 0b1111 - 4 bytes
    // the size of one hex_block in the uid(eg. 1F from "1F 84 AD B3")
    constexpr unsigned short UID_HEX_BLOCK_SIZE = 2; // sizeof(byte) / 4
    // how many blocks of UID_HEX_BLOCK_SIZE are in the UID
    constexpr unsigned short UID_BLOCKS = 4; // sizeof(MFRC522::Uid::uidByte) / sizeof(byte)
    class UID
    {
    public:
        UID();
        //  UID(MFRC522::Uid &_uid);
        UID(String _uid);
        UID(const UID &_uid);
        ~UID() {}

        bool operator==(const UID &_uid) const;
        bool operator==(String &_uid) const;
        bool operator==(const char *_uid) const;

        bool operator!=(const UID &_uid) const;
        bool operator!=(const String &_uid) const;
        bool operator!=(const char *_uid) const;

        String to_string() const;

        /*
            @return true if the UID is set - if it has stored a UID in
        */
        operator bool();

        // delete the
        void clear();

    protected:
        /*
            checks the format and adjusts it if needed
            @return true if the format is ok-could be adjusted - else false
        */
        bool _check_adjust_format(String &_uid) const; // maybe add also err_codes

    private:
        char uid[UID_BLOCKS][UID_HEX_BLOCK_SIZE];
    };
} // namespace RFID

/*
RFID::UID::UID(MFRC522::Uid &_uid)
{
    for (unsigned short i = 0; i < _uid.size; ++i)
    {
        unsigned short dec_num = bin_to_dec(_uid.uidByte[i]);
        String tmp = String(dec_num, HEX);
        for (unsigned short a = 0; a < UID_HEX_BLOCK_SIZE; ++a)
        {
            this->uid[i][a] = tmp[a];
        }
    }
}
*/
RFID::UID::UID()
{
    this->clear();
}
RFID::UID::UID(const UID &_uid)
{
    *this = _uid.to_string();
}
RFID::UID::UID(String _uid)
{
    if (!this->_check_adjust_format(_uid))
    {
        Serial.println("format-ERROR...");
        return; // format-error
    }
    // format is correct...
    unsigned short block_counter = 0;
    for (unsigned short i = 0; i < _uid.length();)
    {
        for (unsigned short a = 0; a < UID_HEX_BLOCK_SIZE; ++a)
        {
            this->uid[block_counter][a] = _uid[i++];
        }
        ++i; // jump over the ' ' between the hex-chars
        block_counter++;
    }
}

String RFID::UID::to_string() const
{
    String ret = "";
    for (unsigned short i = 0; i < UID_BLOCKS; ++i)
    {
        for (unsigned short a = 0; a < UID_HEX_BLOCK_SIZE; ++a)
        {
            ret += this->uid[i][a];
        }
        ret += ' ';
    }
    return ret;
}

bool RFID::UID::operator==(const RFID::UID &_uid) const
{
    return this->to_string() == _uid.to_string();
}
bool RFID::UID::operator==(String &_uid) const
{
    UID tmp(_uid);
    return *this == tmp;
}
bool RFID::UID::operator==(const char *_uid) const
{
    return *this == String(_uid);
}
bool RFID::UID::operator!=(const UID &_uid) const
{
    return !(*this == _uid);
}
bool RFID::UID::operator!=(const String &_uid) const
{
    return !(*this == _uid);
}
bool RFID::UID::operator!=(const char *_uid) const
{
    return !(*this == _uid);
}

bool RFID::UID::_check_adjust_format(String &_uid) const
{
    _uid.toUpperCase();
    // remove whitespaces at the beginning and end
    while (_uid[0] == ' ' || _uid[0] == '\t')
    {
        Serial.println("removing whitespaces at beginning...");
        _uid.remove(0);
    }
    while (_uid[_uid.length() - 1] == ' ' || _uid[_uid.length() - 1] == '\t')
    {
        Serial.println("removing whitespaces at end...");
        _uid.remove(_uid.length() - 1);
    }

    // check the length of the string - could be bigger than exactly formated UID - could contain whitespaces...
    if (!(_uid.length() == UID_HEX_BLOCK_SIZE * UID_BLOCKS + UID_BLOCKS - 1))
    {
        Serial.println("UID_ERROR: Length not correct...");
        return false; // uid_str_length_err
    }

    // check the format of _uid. Format is: "FF FF FF FF"
    for (unsigned short i = 1; i <= _uid.length(); ++i)
    {
        if (i % 3 == 0) // should be a whitespace
        {
            if (_uid[i - 1] != ' ')
            {
                Serial.println("whitespace-ERROR");
                return false;
            }
        }
        else // i%2 & "i%1"
        {
            if (!isHexadecimalDigit(_uid[i - 1]))
            {
                Serial.println("HEX-ERROR");
                return false;
            }
        }
    }
    return true;
}

RFID::UID::operator bool()
{
    for (unsigned short i = 0; i < UID_BLOCKS; ++i)
    {
        for (unsigned short a = 0; a < UID_HEX_BLOCK_SIZE; ++a)
        {
            if (this->uid[i][a] != ' ') // if one char is set we consider it as set
                return true;
        }
    }
    Serial.println();
    return false;
}

void RFID::UID::clear()
{
    for (unsigned short i = 0; i < UID_BLOCKS; ++i)
    {
        for (unsigned short a = 0; a < UID_HEX_BLOCK_SIZE; ++a)
        {
            this->uid[i][a] = ' ';
        }
    }
}
