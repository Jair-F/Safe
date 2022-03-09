#pragma once

#include "Lock.hpp"

class Unlock_Object {
public:
    Unlock_Object(Lock::unlock_token* _utoken, bool _enabled = true);
    Unlock_Object(const Unlock_Object&) = delete;
    Unlock_Object& operator=(const Unlock_Object&) = delete;
    virtual ~Unlock_Object();
    void enable() { this->enabled = true; }
    /*
        Disable this unlock method
    */
    void disable() { this->enabled = false; }
    /*
        enable this unlock method
    */
    bool is_enabled() { return this->enabled; }

    /*
        the loop, where the unlock_object scans for verification(scans
        for fingerprint/rfid/pin) and pushes a unlock_request...
    */
    virtual void loop() = 0; // need to be overwritten by inherrited classes
protected:
    Lock::unlock_token* utoken; // for requesting to unlock the Lock
private:
    bool enabled;   // wether we want to use this unlock_method/unlock_object
};

// ------------- Implementation -------------

Unlock_Object::Unlock_Object(Lock::unlock_token* _utoken, bool _enabled) :utoken(_utoken), enabled(_enabled) { }

Unlock_Object::~Unlock_Object() {
    delete utoken;
}

