#pragma once
#include <Arduino.h>
#include "Lock/Unlock_Object.hpp"

#define _MAX_PIN_LN_ 8

class Pin : public Unlock_Object
{
public:
    Pin(Lock *_lock, String _pin, bool enabled = true);
    Pin(Lock *_lock, bool enabled = true);
    virtual ~Pin() {}

    void set_pin(String _pin);
    void clear_pin();
    String get_pin() const { return this->pin; }
    /**
     * @return true if the pin is set
     */
    bool check_pin_set() const { return pin != ""; }

    uint8_t get_max_pin_ln() const { return _MAX_PIN_LN_; }

    /**
     * @details appends only if input_bufferis not filled up to max_pin_ln
     */
    void append(char _input);
    /**
     * @details pops only back if at least 1 digit is in input_buffer
     */
    void pop_back();
    const String &get_input_buffer() const { return this->input_buffer; }

    /**
     * @details checks the input buffer with the set pin
     * @return true if the input buffer matches with the pin
     */
    bool _check_input_buffer() const;
    void clear_input_buffer();
    /**
     * @details if _in_buffer is longer than max_pin_ln it wil take
     *          only the first chars until max_pin_ln
     */
    void set_input_buffer(String _in_buffer);

    /**
     * @details same as _check_input_buffer but responds in unlock_authentication_reports
     *          like every unlock_object
     */
    virtual unlock_authentication_reports read() override;

    /**
     * @details checks if the pin is set
     * @return true if a pin is set
     */
    virtual bool authorized_unob_database_empty() override;

private:
    String pin = ""; // if pin is "" - its not set - no pin
    String input_buffer;
};