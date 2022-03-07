#pragma once

#include "Lock.hpp"

class Unlock_Object {
public:
    Unlock_Object(unlock_token* _utoken, bool _enabled = true);
    virtual ~Unlock_Object();
    void enable() { this->enabled = true; }
    void disable() { this->enabled = false; }
    bool is_enabled() { return this->enabled; }

    /*
        the loop, where the unlock_object scans for verification(scans
        for fingerprint/rfid/pin) and pushes a unlock_request...
    */
    virtual void loop() = 0; // need to be overwritten by inherrited classes
private:
    bool enabled;   // wether we want to use this unlock_method/unlock_object
    unlock_token* utoken; // for requesting to unlock the Lock
};

// ------------- Implementation -------------

Unlock_Object::Unlock_Object(unlock_token* _utoken, bool _enabled) :utoken(_utoken), enabled(_enabled) { }

Unlock_Object::~Unlock_Object() {
    delete utoken;
}

