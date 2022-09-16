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
        log_message();

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
