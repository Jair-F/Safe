#include <MFRC522.h>
#include "RFID/RFID.hpp"

RFID::RFID::RFID(byte ss, byte rst, Lock *_lock, bool _enabled) : Unlock_Object(_lock, _enabled), rfid(ss, rst)
{
    for (unsigned short i = 0; i < NUM_OF_TAGS; ++i)
    {
        this->allowed_tags[i].clear();
    }
}

bool RFID::RFID::begin()
{
    this->rfid.PCD_Init();
    this->rfid.PCD_DumpVersionToSerial();
    if (this->rfid.PCD_PerformSelfTest())
    {
        Serial.println("Selftest of RFID was sucessful");
    }
    else
    {
        Serial.println("Selftest of RFID failed");
    }
    this->rfid.PCD_Init(); // initialize the rfid again - after the test it wont work
    this->rfid.PCD_SoftPowerUp();
}

bool RFID::RFID::authorized_unob_database_empty()
{
    bool least_one_set = false;
    for (unsigned short i = 0; i < NUM_OF_TAGS; ++i)
    {
        if (this->allowed_tags[i].is_set())
        {
            least_one_set = true;
            break;
        }
    }
    return least_one_set;
}

Unlock_Object::unlock_authentication_reports RFID::RFID::read()
{
    if (!this->is_enabled())
    {
        return Unlock_Object::unlock_authentication_reports::UNLOCK_OBJECT_DISABLED;
    }

    if (!this->tag_present())
    {
        return Unlock_Object::unlock_authentication_reports::NO_UNLOCK_OBJECT_PRESENT;
    }

    UID read_uid = this->read_tag_UID();
    if (read_uid.is_set())
    {
        Serial.print("Read tag with UID: ");
        Serial.println(read_uid.to_string());

        for (uint8_t i = 0; i < NUM_OF_TAGS; ++i)
        {
            if (this->allowed_tags[i] == read_uid)
                return Unlock_Object::unlock_authentication_reports::AUTHORIZED_UNLOCK_OBJECT;
        }
    }
    else
    {
        return Unlock_Object::unlock_authentication_reports::UNLOCK_OBJECT_READ_ERROR;
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
    this->unread_tag_present = this->rfid.PICC_IsNewCardPresent();

    return this->unread_tag_present;
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
        return;
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
    if (!this->unread_tag_present) // if we didnt checked for a tag or if there is no tag present - check again
    {
        if (this->rfid.PICC_IsNewCardPresent()) // if a tag is present
        {
            this->unread_tag_present = true;
        }
    }

    if (this->unread_tag_present)
    {
        Serial.println("reading the tag UID...");
        if (this->rfid.PICC_ReadCardSerial()) // if the UID could be read
        {
            Serial.println("read rfid tag uid");
            // holds the reader until the card is "removed"
            this->rfid.PICC_HaltA(); // halt the reader in order to not read the same card again and again

            this->unread_tag_present = false;
            return UID(this->rfid.uid.uidByte, this->rfid.uid.size);
        }
    }

    this->unread_tag_present = false;
    return UID();
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
