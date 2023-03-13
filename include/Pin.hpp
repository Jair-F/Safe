#pragma once
#include <Arduino.h>
#include "Lock/Unlock_Object.hpp"

class Pin : public Unlock_Object
{
public:
    Pin(Lock *_lock, String _pin, bool enabled = true);
    Pin(Lock *_lock, bool enabled = true);
    virtual ~Pin() {}

    void set_pin(String _pin);
    void clear_pin();

    void append(char _input); // to the input_buffer
    void pop_back();          // to the input_buffer

    void clear_input_buffer();
    void set_input_buffer(String _in_buffer);

    virtual unlock_authentication_reports read() override;

    virtual bool authorized_unob_database_empty() override;

private:
    String pin = ""; // if pin is "" - its not set - no pin
    String input_buffer;
};