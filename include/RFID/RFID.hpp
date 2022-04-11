#pragma once

#include <Arduino.h>
#include <MFRC522.h>
#include "../Unlock_Object.hpp"
#include "../Helper.hpp"
#include "UID.hpp"

/*
RFID:
    explanation video: https://www.youtube.com/watch?v=gTQmLkwnUxk
*/

namespace RFID
{
    constexpr unsigned short NUM_OF_TAGS = 10;

    class RFID : public Unlock_Object
    {
    public:
        RFID(byte chipSelectPin, byte resetPowerDownPin, Lock::unlock_token *_utoken, bool _enabled = true);
        virtual ~RFID() { this->rfid.PCD_SoftPowerDown(); }

        void begin();

        /*
            func to call in a loop - it reads for a tag and if there is a card present it searches in
            allowed tags for a match. If a match was found it makes a unlock_request to the lock
        */
        virtual void loop() override;

        /*
            read for a tag id
            @param async false=wait until a tag is present. true=read 1 time and return imidiately
            @return the tag id if a tag was present else an empty string
        */
        UID read_Tag_UID(bool async = true);

        /*
            reads from the RFID_module for a tag and adds it to database at id id
            !!! this function reads only one time - if there was no tag it will return without doing nothing !!!
            Maybe you want to call this function in a loop to ensure that a tag was added
            @param id id where to store in the database - it overwrites if there exists already a tag
            @return true if a tag was added else false
        */
        bool read_add_tag(unsigned short id);
        /*
            adds a tag to the allowed_tags list
        */
        void add_tag(unsigned short id, UID tag_uid);
        /*
            removes tag from the tag with id id from the allowed_tags
            throws an error if the id id is out of range
        */
        void remove_tag(unsigned short id);
        /*
            removes tag from the tag with uidid tag_uid from the allowed_tags
            @return true if one or more tags where removed - else false
        */
        bool remove_tag(UID tag_uid);
        /*
            removes all saved tags from the allowed_tags_database
        */
        void clear_database();

        /*
            @return the tag uid if the tag is set else it throws an error
        */
        UID &get_tag_uid(unsigned short id);
        /*
            @param the tag_uid to get the id from - has to be with whitespaces removed from left
                    and right and has to be in the format like the get_tag_uid() returns("xx xx xx xx")
            @return if a matching tag was found the tag_uid(>0). if no tag was found -1
        */
        int get_tag_id(UID tag_uid);
        // get_used_ids
        bool id_used(unsigned short id) const;
        // bool in_database(String uid);
    private:
        MFRC522 rfid;
        UID allowed_tags[NUM_OF_TAGS];
    };

} // namespace RFID

// ------------ Implementations ------------

RFID::RFID::RFID(byte chipSelectPin, byte resetPowerDownPin,
                 Lock::unlock_token *_utoken, bool _enabled = true) : rfid(chipSelectPin, resetPowerDownPin),
                                                                      Unlock_Object(_utoken, _enabled)
{
    for (unsigned short i = 0; i < NUM_OF_TAGS; ++i)
    {
        this->allowed_tags[i].clear();
    }
}

void RFID::RFID::begin()
{
    SPI.begin();                          // Arduino interface which is necessarily for RFID(SPI is a global variable from Arduino)
    this->rfid.PCD_Init();                // starting and initialising rfid
    this->rfid.PCD_DumpVersionToSerial(); // printing RFID_Version to serial
    if (this->rfid.PCD_PerformSelfTest())
    {
        Serial.println("passed self test");
    }
    else
    {
        Serial.println("Self test failed");
    }
    Serial.println("ready...");
}

void RFID::RFID::loop()
{
    if (this->is_enabled())
    {
        UID tag_uid = this->read_Tag_UID(true);
        if (tag_uid)
        {
            Serial.print("Read tag with UID: ");
            Serial.println(tag_uid.to_string());
            bool matching_tag_found = false; // if the tag is authorized
            for (unsigned short i = 0; i < NUM_OF_TAGS; ++i)
            {
                if (this->allowed_tags[i] == tag_uid)
                {
                    matching_tag_found = true;
                    this->utoken->request_unlock();
                    break;
                }
            }
            if (!matching_tag_found)
            {
                this->utoken->report_unathorized_unlock_try();
            }
        }
        delay(500);
    }
}

bool RFID::RFID::id_used(unsigned short id) const
{
    if (id > NUM_OF_TAGS - 1)
    {
        DEBUG_PRINT("id is out of range")
        // throw an error
    }
    return this->allowed_tags[id].operator bool();
}

bool RFID::RFID::read_add_tag(unsigned short id)
{
    if (id > NUM_OF_TAGS - 1)
    {
        Serial.println("id is out of range");
        // throw an error
    }
    UID tag_uid = this->read_Tag_UID();
    if (tag_uid)
    {
        this->allowed_tags[id] = tag_uid;
        return true;
    }
    return false;
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
        Serial.println("id is out of range");
        // throw an error
    }
    this->allowed_tags[id].clear();
}

bool RFID::RFID::remove_tag(UID tag_uid)
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
        Serial.println("id is out of range");
        return;
        // throw an error
    }
    if (!this->allowed_tags[id])
    {
        Serial.println("tag isnt set");
        return;
        // throw error tag isnt set...
    }
    return this->allowed_tags[id];
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

RFID::UID RFID::RFID::read_Tag_UID(bool async)
{
    UID uid;
    String uid_str;
    if (!async) // if we dont run in async mode wait until an new card is present
    {
        while (!this->rfid.PICC_IsNewCardPresent()) // returns true if a card is present
            delay(10);
    }
    else
    {
        if (!this->rfid.PICC_IsNewCardPresent()) // if no card is "online"
        {
            auto err_code = rfid.PICC_HaltA(); // halt the reader in order to not read the same card again and again
            if (err_code != MFRC522::StatusCode::STATUS_OK)
            {
                Serial.println("Error halting");
            }
            return UID(); // return a empty uid
        }
    }

    // a new card is present...
    if (rfid.PICC_ReadCardSerial())
    { // starting read the card
        uid = this->rfid.uid;
    }
    rfid.PICC_HaltA(); // halt the reader in order to not read the same card again and again
    return uid;
}
