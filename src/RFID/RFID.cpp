#include "RFID/RFID.hpp"

RFID::RFID::RFID(byte irq, byte reset, Lock *_lock, bool _enabled) : Unlock_Object(_lock, _enabled), rfid(irq, reset)
{
    for (unsigned short i = 0; i < NUM_OF_TAGS; ++i)
    {
        this->allowed_tags[i].clear();
    }
}

bool RFID::RFID::begin()
{
    this->rfid.begin();
    uint32_t firmware_version = this->rfid.getFirmwareVersion();
    if (!firmware_version)
    {
        logger.log("Failded to get firmware-version of the PN532-RFID", Log::log_level::L_WARNING);
        return false;
    }
    else
    {
        Serial.print("PN532-Version: ");
        Serial.println((firmware_version >> 24) & 0xFF, HEX);
        Serial.print("Firmware ver. ");
        Serial.print((firmware_version >> 16) & 0xFF, DEC);
        Serial.print('.');
        Serial.println((firmware_version >> 8) & 0xFF, DEC);
    }

    bool success = this->rfid.SAMConfig();
    if (!success)
    {
        logger.log("Failed to configure the PN532-RFID", Log::log_level::L_WARNING);
        return false;
    }

    return true;
}

Unlock_Object::unlock_authentication_reports RFID::RFID::read()
{
    if (!this->is_enabled())
    {
        return Unlock_Object::unlock_authentication_reports::UNLOCK_OBJECT_DISABLED;
    }

    uint8_t read_uid[MAX_UID_BLOCKS];
    uint8_t uid_length = 0;
    bool read_success = false;

    read_success = rfid.readPassiveTargetID(PN532_MIFARE_ISO14443A, read_uid, &uid_length);
    if (!read_success)
    {
        return Unlock_Object::unlock_authentication_reports::NO_UNLOCK_OBJECT_PRESENT;
    }

    UID tmp_uid(read_uid, uid_length);
    for (uint8_t i = 0; i < NUM_OF_TAGS; ++i)
    {
        if (this->allowed_tags[i] == tmp_uid)
            return Unlock_Object::unlock_authentication_reports::AUTHORIZED_UNLOCK_OBJECT;
    }

    return Unlock_Object::unlock_authentication_reports::UNAUTHORIZED_UNLOCK_OBJECT;
}

bool RFID::RFID::id_used(unsigned short id) const
{
    if (id > NUM_OF_TAGS - 1)
    {
        DEBUG_PRINT(F("id is out of range"))
        // throw an error
    }
    return this->allowed_tags[id].is_set();
}

bool RFID::RFID::read_add_tag(unsigned short id)
{
    if (id > NUM_OF_TAGS - 1)
    {
        DEBUG_PRINTLN(F("id is out of range"));
        // throw an error
    }
    UID tag_uid = this->read_tag_UID();
    if (tag_uid.is_set())
    {
        this->allowed_tags[id] = tag_uid;
        return true;
    }
    return false;
}

bool RFID::RFID::tag_present()
{
    uint8_t read_uid[MAX_UID_BLOCKS];
    uint8_t uid_length = 0;
    return this->rfid.readDetectedPassiveTargetID(read_uid, &uid_length);
}

void RFID::RFID::add_tag(unsigned short id, UID tag_uid)
{
    if (id > NUM_OF_TAGS - 1)
    {
        // throw an error
    }
    this->allowed_tags[id] = tag_uid;
}

void RFID::RFID::remove_tag(unsigned short id)
{
    if (id > NUM_OF_TAGS - 1)
    {
        DEBUG_PRINTLN(F("id is out of range"));
        // throw an error
    }
    this->allowed_tags[id].clear();
}

bool RFID::RFID::remove_tag(const UID &tag_uid)
{
    bool least_one_tag_removed = false;
    for (unsigned short i = 0; i < NUM_OF_TAGS; ++i)
    {
        if (this->allowed_tags[i] == tag_uid)
        {
            this->allowed_tags[i].clear();
            least_one_tag_removed = true;
        }
    }
    return least_one_tag_removed;
}

void RFID::RFID::clear_database()
{
    for (unsigned short i = 0; i < NUM_OF_TAGS; ++i)
    {
        this->allowed_tags[i].clear();
    }
}

RFID::UID &RFID::RFID::get_tag_uid(unsigned short id)
{
    if (id > NUM_OF_TAGS - 1)
    {
        DEBUG_PRINTLN(F("id is out of range"));
        // return;
        // throw an error
    }

    else
    {
        // return this->allowed_tags[id].operator bool();
        return this->allowed_tags[id];
    }
}

int RFID::RFID::get_tag_id(UID tag_uid)
{
    for (unsigned short i = 0; i < NUM_OF_TAGS; ++i)
    {
        if (this->allowed_tags[i] == tag_uid)
        {
            return i;
        }
    }
    return -1;
}

RFID::UID RFID::RFID::read_tag_UID()
{
    uint8_t read_uid[MAX_UID_BLOCKS];
    uint8_t uid_length = 0;
    bool read_success = false;

    read_success = rfid.readPassiveTargetID(PN532_MIFARE_ISO14443A, read_uid, &uid_length);
    if (!read_success)
    {
        return UID();
    }

    return UID(read_uid, uid_length);
}

const char *RFID::error_message(MFRC522::StatusCode error_code)
{
    const char *err_msg = nullptr;
    switch (error_code)
    {
    case MFRC522::StatusCode::STATUS_COLLISION:
    {
        err_msg = "Collission detected";
        break;
    }
    case MFRC522::StatusCode::STATUS_CRC_WRONG:

    {
        err_msg = "The CRC_A does not match";
        break;
    }
    case MFRC522::StatusCode::STATUS_ERROR:
    {
        err_msg = "Error in communication";
        break;
    }
    case MFRC522::StatusCode::STATUS_INTERNAL_ERROR:
    {
        err_msg = "Internal error in the code(in RFID-library). Should not happen ;-)";
        break;
    }
    case MFRC522::StatusCode::STATUS_INVALID:
    {
        err_msg = "Invalid argument.";
        break;
    }
    case MFRC522::StatusCode::STATUS_MIFARE_NACK:
    {
        err_msg = "A MIFARE PICC responded with NAK.";
        break;
    }
    case MFRC522::StatusCode::STATUS_NO_ROOM:
    {
        err_msg = "A buffer is not big enough.";
        break;
    }
    case MFRC522::StatusCode::STATUS_OK:
    {
        err_msg = "Success";
        break;
    }
    case MFRC522::StatusCode::STATUS_TIMEOUT:
    {
        err_msg = "Timeout in communication.";
        break;
    }
    default:
    {
        err_msg = "Unknown ERROR";
        break;
    }
    }
    return err_msg;
}
