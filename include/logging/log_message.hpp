#pragma once

#include "system_clock.hpp"

namespace Log
{
    namespace log_level
    {
        /*
            the most less critical level has the lowest value - 0
        */
        constexpr byte L_DEBUG = 0;
        constexpr byte L_INFO = 1;
        constexpr byte L_WARNING = 2;
        constexpr byte L_ERROR = 3;
        constexpr byte L_CRITICAL = 4;
    }

    class log_message
    {
    private:
        String msg;
        byte level;
        RtcDateTime tm_point; // when the log message was created

    public:
        log_message() : msg(), level(log_level::L_DEBUG) {}

        log_message(const char *_msg, byte _LogLevel);
        log_message(const String &_msg, byte _LogLevel);

        log_message(const log_message &lm);
        log_message &operator=(const log_message &);
        ~log_message();

        bool operator==(const log_message &_msg) const;
        bool operator>(const log_message &_msg) const;

        const String &message() const;
        byte Level() const;
        const RtcDateTime &time_point() const;
    };

} // namespace Log

Log::log_message::log_message(const char *_msg, byte _LogLevel) : msg(_msg), level(_LogLevel), tm_point(system_clock.GetDateTime()) {}

Log::log_message::log_message(const String &_msg, byte _LogLevel) : msg(_msg), level(_LogLevel), tm_point(system_clock.GetDateTime()) {}
Log::log_message &Log::log_message::operator=(const log_message &_lm)
{
    this->msg = _lm.message();
    this->level = _lm.Level();
    this->tm_point = system_clock.GetDateTime();
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

const RtcDateTime &Log::log_message::time_point() const
{
    return this->tm_point;
}