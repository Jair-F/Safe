#pragma once

namespace RFID
{
    class tag_storage
    {
        /*
            class for storing and handeling rfid_tag_uids and checking :|
        */
    public:
        tag_storage(String _tag_uid = "") : tag_uid(_tag_uid) {}
        ~tag_storage() {}

        tag_storage(const tag_storage &) = delete;
        tag_storage &operator=(const tag_storage &) = delete;

        bool operator==(const tag_storage &tag) const;
        bool operator==(const String &_tag_uid) const;
        bool operator==(const char *_tag_uid) const;
        bool operator!=(const tag_storage &tag) const;
        bool operator!=(const String &_tag_uid) const;
        bool operator!=(const char *_tag_uid) const;

        /*
            set the rfid_tag_uid - the id from the rfid-tag
        */
        void set_tag_uid(String _tag_uid) { tag_uid = _tag_uid; }
        /*
            get the rfid_tag_uid - the id from the rfid-tag
        */
        String get_tag_uid() const { return tag_uid; }

    private:
        String tag_uid; // unique id from the rfid_tag
    };
} // namespace RFID

// -------------------- Implementations --------------------

bool RFID::tag_storage::operator==(const char *_tag_uid) const
{
    return this->tag_uid == _tag_uid;
}
bool RFID::tag_storage::operator==(const String &_tag_uid) const
{
    return *this == _tag_uid.c_str();
}
bool RFID::tag_storage::operator==(const tag_storage &tag) const
{
    return *this == tag.get_tag_uid();
}
bool RFID::tag_storage::operator!=(const char *_tag_uid) const
{
    return !(*this == _tag_uid);
}
bool RFID::tag_storage::operator!=(const String &_tag_uid) const
{
    return *this != _tag_uid.c_str();
}
bool RFID::tag_storage::operator!=(const tag_storage &tag) const
{
    return *this != tag.get_tag_uid();
}
