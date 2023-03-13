#include "Pin.hpp"

Pin::Pin(Lock *_lock, String _pin, bool enabled) : Unlock_Object(_lock, enabled),
                                                   pin(_pin)
{
}

Pin::Pin(Lock *_lock, bool enabled) : Unlock_Object(_lock, enabled),
                                      pin("")
{
}

void Pin::set_pin(String _pin)
{
    this->pin = _pin;
}

void Pin::clear_pin()
{
    this->pin = "";
}

void Pin::append(char _input)
{
    this->input_buffer += _input;
}

void Pin::pop_back()
{
    if (this->input_buffer.length() > 1)
        this->input_buffer.remove(this->input_buffer.length() - 2);
}

void Pin::clear_input_buffer()
{
    this->input_buffer = "";
}

void Pin::set_input_buffer(String _in_buffer)
{
    this->input_buffer = _in_buffer;
}

Unlock_Object::unlock_authentication_reports Pin::read()
{
    if (this->input_buffer == this->pin)
    {
        return Unlock_Object::unlock_authentication_reports::AUTHORIZED_UNLOCK_OBJECT;
    }
    else if (this->input_buffer == "")
    {
        return Unlock_Object::unlock_authentication_reports::NO_UNLOCK_OBJECT_PRESENT;
    }
    else
    {
        return Unlock_Object::unlock_authentication_reports::UNAUTHORIZED_UNLOCK_OBJECT;
    }
}

bool Pin::authorized_unob_database_empty()
{
    return this->pin == "";
}