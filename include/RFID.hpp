#pragma once

#include <Arduino.h>
#include <MFRC522.h>
#include "Helper.hpp"

/*
RFID:
    explanation video: https://www.youtube.com/watch?v=gTQmLkwnUxk
*/

namespace RFID
{
    class RFID;
    class RFID_tag_storage;
    class RFID {
    public:
        RFID(byte chipSelectPin, byte resetPowerDownPin) : rfid(chipSelectPin, resetPowerDownPin) { }
        virtual ~RFID() { this->rfid.PCD_SoftPowerDown(); }

        void begin() {
            SPI.begin(); // Arduino interface which is necessarily for RFID(SPI is a global variable from Arduino)
            this->rfid.PCD_Init();  // starting and initialising rfid
            this->rfid.PCD_DumpVersionToSerial();
            //this->rfid.PCD_PerformSelfTest();
        }

        /*
            read for a tag id
            @param async true=wait until a tag is present. false=read 1 time and return imidiately
            @return the tag id if a tag was present else an empty string
        */
        String read_Tag_ID(bool async = true);
    private:
        MFRC522 rfid;
    };

    String RFID::read_Tag_ID(bool async) {
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








    class RFID_tag_storage {
    public:
        RFID_tag_storage(unsigned short _id, String _tag_uid) :id(_id), tag_uid(_tag_uid) { }
        RFID_tag_storage(unsigned short _id) :id(_id), tag_uid(String()) { }
        ~RFID_tag_storage() { }

        RFID_tag_storage(const RFID_tag_storage&) = delete;
        RFID_tag_storage& operator=(const RFID_tag_storage&) = delete;

        /*
            set the rfid_tag_uid - the id from the rfid-tag
        */
        void set_tag_uid(String _tag_uid) { tag_uid = _tag_uid; }
        /*
            get the rfid_tag_uid - the id from the rfid-tag
        */
        String get_tag_uid() { return tag_uid; }
        /*
            get the id in the config/database
        */
        unsigned short get_id() { return id; }
    private:
        String tag_uid;     // unique id from the rfid_tag
        unsigned short id;  // id in the database/config-fiel only for identification in the database/config
    };

} // namespace RFID



