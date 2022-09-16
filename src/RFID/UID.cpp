#include "RFID/UID.hpp"

RFID::UID::UID(uint8_t *_uid, uint8_t _uid_length) : uid_length(_uid_length)
{
    for (uint8_t i = 0; i < _uid_length; ++i)
    {
        this->uid[i] = _uid[i];
    }
}
RFID::UID::UID(const UID &_uid) : uid_length(_uid.get_uid_length())
{
    for (uint8_t i = 0; i < uid_length; ++i)
    {
        this->uid[i] = _uid.cget_uid()[i];
    }
}

String RFID::UID::to_string() const
{
    String ret = "";
    for (uint8_t i = 0; i < this->uid_length; ++i)
    {
        ret += this->uid[i];
        ret += ' ';
    }
    return ret;
}

bool RFID::UID::operator==(const UID &_uid) const
{
    if (this->uid_length != _uid.get_uid_length())
        return false;

    for (uint8_t i = 0; i < _uid.get_uid_length(); ++i)
    {
        if (this->uid[i] != _uid.cget_uid()[i])
            return false;
    }
    return true;
}
bool RFID::UID::operator!=(const UID &_uid) const
{
    return !(*this == _uid);
}

RFID::UID::operator bool()
{
    return this->is_set();
}

bool RFID::UID::is_set() const
{
    return this->uid_length != 0;
}

void RFID::UID::clear()
{
    this->uid_length = 0;
    for (unsigned short i = 0; i < MAX_UID_BLOCKS; ++i)
    {
        this->uid[i] = 0;
    }
}
