#pragma once

#include <MFRC522.h>
#include "Helper.hpp"

/*
RFID:
    explanation video: https://www.youtube.com/watch?v=gTQmLkwnUxk
*/

namespace RFID
{
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
            !!!!!!!!!!!!!!! ------------ need to find out how the tag-id will be transferd...
            read for a tag id
            @param async true=wait until a tag is present. false=read 1 time and return imidiately
            @return the tag id if a tag was present else nullptr
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

} // namespace RFID



