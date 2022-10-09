#pragma once
#include <MFRC522.h>
#include <Arduino.h>
#include "../Helper.hpp"

namespace RFID
{
    // max number of the UID-blocks
    constexpr unsigned short MAX_UID_BLOCKS = 7;
    class UID
    {
    public:
        UID() : uid_length(0), uid(nullptr) {}
        /**
         * @details makes a deep copy of _uid
         */
        UID(uint8_t *_uid, uint8_t uid_length);
        UID(const UID &_uid);
        UID(UID &&_uid)
        noexcept;
        ~UID();
        UID &operator=(const UID &_uid);

        bool operator==(const UID &_uid) const;

        bool operator!=(const UID &_uid) const;

        String to_string() const;

        /*
            @return true if the UID is set - if it has stored a UID in
        */
        operator bool();

        bool is_set() const;

        uint8_t get_uid_length() const { return this->uid_length; }

        /*
            pointer to the uid array - size is either 4 or 7.
            get the size with the get_uid_length() func.
        */
        uint8_t *get_uid() { return this->uid; }
        /*
            return a const pointer of the uid
        */
        const uint8_t *cget_uid() const { return this->uid; }

        // clear the UID
        void clear();

    private:
        uint8_t *uid;
        uint8_t uid_length = 0;
    };
} // namespace RFID
