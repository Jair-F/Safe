#include "logging/Log.hpp"
#include "system_clock/system_clock.hpp"

extern Clock::Clock system_clock;

Log::log_message::log_message() : msg(), level(log_level::L_DEBUG)
{
}

Log::log_message::log_message(const char *_msg, byte _LogLevel) : msg(_msg), level(_LogLevel), tm_point(system_clock.now())
{
}

Log::log_message::log_message(const String &_msg, byte _LogLevel) : msg(_msg), level(_LogLevel), tm_point(system_clock.now())
{
}
Log::log_message &Log::log_message::operator=(const log_message &_lm)
{
    this->msg = _lm.message();
    this->level = _lm.Level();
    this->tm_point = _lm.time_point();
    return *this;
}
Log::log_message::~log_message() {}

const String &Log::log_message::message() const
{
    return this->msg;
}

byte Log::log_message::Level() const
{
    return this->level;
}

bool Log::log_message::operator==(const log_message &_l_msg) const
{
    return (this->msg.length() == _l_msg.message().length()) && (this->level == _l_msg.Level());
}
Log::log_message::log_message(const log_message &_msg)
{
    this->msg = _msg.message();
    this->level = _msg.Level();
}
bool Log::log_message::operator>(const log_message &_l_msg) const
{
    return this->level > _l_msg.Level();
}

const Clock::time_point &Log::log_message::time_point() const
{
    return this->tm_point;
}