#include "Pin.hpp"

Pin::Pin(Lock *_lock, String _pin, bool enabled) : Unlock_Object(_lock, enabled), pin(_pin)
{
}

Pin::Pin(Lock *_lock, bool enabled) : Unlock_Object(_lock, enabled), pin("")
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
    if (this->input_buffer.length() < _MAX_PIN_LN_)
        this->input_buffer += _input;
}

void Pin::pop_back()
{
    if (this->input_buffer.length() > 0)
        this->input_buffer.remove(this->input_buffer.length() - 1);
}

bool Pin::_check_input_buffer() const
{
    if (this->pin.length() == 0)
        return false;

    return this->pin == this->input_buffer;
}

void Pin::clear_input_buffer()
{
    this->input_buffer = "";
}

void Pin::set_input_buffer(String _in_buffer)
{
    if (_in_buffer.length() <= _MAX_PIN_LN_)
    {
        this->input_buffer = _in_buffer;
    }
    else
    {
        this->input_buffer = _in_buffer.substring(0, _MAX_PIN_LN_);
    }
}

Unlock_Object::unlock_authentication_reports Pin::read()
{
    if (!this->is_enabled())
    {
        return Unlock_Object::unlock_authentication_reports::UNLOCK_OBJECT_DISABLED;
    }

    if (this->_check_input_buffer()) // if entered correctly or if no pin is set
    {
        this->input_buffer = ""; // resetting the buffer to not unlock in loop
        return Unlock_Object::unlock_authentication_reports::AUTHORIZED_UNLOCK_OBJECT;
    }
    else if (this->input_buffer == "" || this->input_buffer.length() < this->pin.length())
    {
        return Unlock_Object::unlock_authentication_reports::NO_UNLOCK_OBJECT_PRESENT;
    }
    else
    {
        this->input_buffer = ""; // resetting the buffer to not unlock in loop
        return Unlock_Object::unlock_authentication_reports::UNAUTHORIZED_UNLOCK_OBJECT;
    }
}

bool Pin::authorized_unob_database_empty()
{
    return this->pin == "";
}