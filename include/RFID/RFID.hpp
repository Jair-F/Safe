#pragma once

#include <Arduino.h>
#include <MFRC522.h>
#include "../Unlock_Object.hpp"
#include "../Helper.hpp"
#include "tag_storage.hpp"

/*
RFID:
    explanation video: https://www.youtube.com/watch?v=gTQmLkwnUxk
*/

namespace RFID
{
    constexpr unsigned short NUM_OF_TAGS = 127;


    class RFID : public Unlock_Object {
    public:
        RFID(byte chipSelectPin, byte resetPowerDownPin, Lock::unlock_token* _utoken, bool _enabled = true);
        ~RFID() { this->rfid.PCD_SoftPowerDown(); }

        void begin() {
            SPI.begin(); // Arduino interface which is necessarily for RFID(SPI is a global variable from Arduino)
            this->rfid.PCD_Init();  // starting and initialising rfid
            this->rfid.PCD_DumpVersionToSerial();
            //this->rfid.PCD_PerformSelfTest();
        }


        /*
            func to call in a loop - it reads for a tag and if there is a card present it searches in
            allowed tags for a match. If a match was found it makes a unlock_request to the lock
        */
        void loop();

        /*
            read for a tag id
            @param async true=wait until a tag is present. false=read 1 time and return imidiately
            @return the tag id if a tag was present else an empty string
        */
        String read_Tag_ID(bool async = true);

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
        void add_tag(unsigned short id, String tag_uid);
        /*
            removes tag from the tag with id id from the allowed_tags
            throws an error if the id id is out of range
        */
        void remove_tag(unsigned short id);
        /*
            removes tag from the tag with uidid tag_uid from the allowed_tags
            @return true if one or more tags where removed - else false
        */
        bool remove_tag(String tag_uid);

        /*
            @return the tag uid if the tag is set else it throws an error
        */
        String get_tag_uid(unsigned short id)const;
        //get_used_ids
        bool id_used(unsigned short id)const;
        //bool in_database(String uid);
    private:
        MFRC522 rfid;
        tag_storage allowed_tags[NUM_OF_TAGS];
    };

} // namespace RFID


// ------------ Implementations ------------

RFID::RFID::RFID(byte chipSelectPin, byte resetPowerDownPin, Lock::unlock_token* _utoken, bool _enabled = true) : rfid(chipSelectPin, resetPowerDownPin), Unlock_Object(_utoken, _enabled) {
    for (unsigned short i = 0; i < NUM_OF_TAGS; ++i) {
        this->allowed_tags[i].set_tag_uid("");
    }
}

void RFID::RFID::loop() {
    String tag_uid = this->read_Tag_ID();
    if (tag_uid.length() > 0) {
        for (unsigned short i = 0; i < NUM_OF_TAGS; ++i) {
            if (this->allowed_tags[i] == tag_uid) {
                this->utoken->request_unlock();
            }
        }
    }
}

bool RFID::RFID::id_used(unsigned short id) const {
    if (id > NUM_OF_TAGS - 1) {
        // throw an error
    }
    return this->allowed_tags[id].get_tag_uid() == "";
}

bool RFID::RFID::read_add_tag(unsigned short id) {
    if (id > NUM_OF_TAGS - 1) {
        // throw an error
    }
    String tag_uid = this->read_Tag_ID();
    if (tag_uid.length() > 0) {
        this->allowed_tags[id].set_tag_uid(tag_uid);
        return true;
    }
    return false;
}

void RFID::RFID::add_tag(unsigned short id, String tag_uid) {
    if (id > NUM_OF_TAGS - 1) {
        // throw an error
    }
    this->allowed_tags[id].set_tag_uid(tag_uid);
}

void RFID::RFID::remove_tag(unsigned short id) {
    if (id > NUM_OF_TAGS - 1) {
        // throw an error
    }
    this->allowed_tags[id].set_tag_uid("");
}

bool RFID::RFID::remove_tag(String tag_uid) {
    bool least_one_tag_removed = false;
    for (unsigned short i = 0; i < NUM_OF_TAGS; ++i) {
        if (this->allowed_tags[i].get_tag_uid() == tag_uid) {
            this->allowed_tags[i].set_tag_uid("");
            least_one_tag_removed = true;
        }
    }
    return least_one_tag_removed;
}

String RFID::RFID::get_tag_uid(unsigned short id) const {
    if (id > NUM_OF_TAGS - 1) {
        // throw an error
    }
    if (this->allowed_tags[id].get_tag_uid() == "") {
        // throw error tag isnt set...
    }
    return this->allowed_tags[id].get_tag_uid();
}

String RFID::RFID::read_Tag_ID(bool async) {
    String uid_str;
    if (this->rfid.PICC_IsNewCardPresent()) {   // if a card is "online"
        if (rfid.PICC_ReadCardSerial()) {       // starting read the card

            for (unsigned short i = 0; i < rfid.uid.size; ++i) {
                unsigned short dec_num = bin_to_dec(rfid.uid.uidByte[i]);
                String tmp(dec_num, HEX);  // convert to hex-string
                uid_str += tmp + ' ';
            }
            uid_str.remove(uid_str.length() - 1); // remove last char - its a ' '
        }
    }
    rfid.PICC_HaltA();  // halt the reader in order to not read the same card again and again

    return uid_str;
}

