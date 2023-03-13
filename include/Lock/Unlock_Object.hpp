#pragma once

#include "Lock.hpp"

class Lock;

class Unlock_Object
{
public:
    Unlock_Object(Lock *_lock, bool _enabled = true);
    Unlock_Object(const Unlock_Object &) = delete;
    Unlock_Object &operator=(const Unlock_Object &) = delete;
    virtual ~Unlock_Object();
    /*
        enable this unlock method
    */
    virtual void enable() { this->enabled = true; }
    /*
        Disable this unlock method
    */
    virtual void disable() { this->enabled = false; }
    bool is_enabled() { return this->enabled; }

    inline uint16_t get_unlock_object_id() const { return this->unlock_object_id; }

    enum class unlock_authentication_reports
    {
        UNLOCK_OBJECT_DISABLED,     // if the unlock_object which should run the task is disabled return this error like report
        UNLOCK_OBJECT_READ_ERROR,   // error at reading the unlock_object
        NO_UNLOCK_OBJECT_PRESENT,   // for example if there is no finger on the sensor
        UNAUTHORIZED_UNLOCK_OBJECT, // authorize failure
        AUTHORIZED_UNLOCK_OBJECT    // successfull authorization
    };

    /*
        read for a valid unlock input or valid fingerprint or RFID tag
    */
    virtual unlock_authentication_reports read() = 0;

    /**
     * @return true if there is no unlock_object saved in the authorized
     * unlock_object database
     */
    virtual bool authorized_unob_database_empty() = 0;

    /*
        the loop, where the unlock_object scans for verification(scans
        for fingerprint/rfid/pin) and pushes a unlock_request...
    */
    // virtual void loop() = 0; // need to be overwritten by inherrited classes
private:
    bool enabled; // wether we want to use this unlock_method/unlock_object
    uint16_t unlock_object_id;
};
