#pragma once

#include <Arduino.h>
#include <MFRC522.h>
#include <Adafruit_PN532.h>
#include "../Unlock_Object.hpp"
#include "../Helper.hpp"
#include "UID.hpp"
#include "logging/Log.hpp"

extern Log::Log logger;

/*
RFID:
    explanation video: https://www.youtube.com/watch?v=gTQmLkwnUxk
*/

namespace RFID
{
    constexpr unsigned short NUM_OF_TAGS = 10;
    // get according error_message to error_code
    const char *error_message(MFRC522::StatusCode error_code);

    class RFID : public Unlock_Object
    {
    public:
        /*
            @param reset not connected by default
        */
        RFID(byte irq, byte reset, Lock *_lock, bool _enabled = true);
        virtual ~RFID() {}

        /*
            @return true if the RFID-PN532 was setup successfully - else false
        */
        bool begin();

        /*
            read for a tag id
            @return the tag id if a tag was present else an empty UID-object
        */
        UID read_Tag_UID();

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
        bool remove_tag(const UID &tag_uid);
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

        /*
            read for uids and return a succes if a uid was read that matches to one of the database
        */
        unlock_authentication_reports read() override;

    private:
        Adafruit_PN532 rfid;
        UID allowed_tags[NUM_OF_TAGS];
    };

} // namespace RFID
