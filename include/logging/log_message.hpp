#pragma once
#include <Arduino.h>

namespace Log
{
    enum class log_level
    {
        DEBUG = 1,
        INFO = 2,
        WARNING = 3,
        ERROR = 4,
        CRITICAL = 5
    };

    class log_message
    {
    public:
        /*
            !!! IMPORTANT !!! this saves only the pointer to the beggining from the msg - makes not a deep copy
            please ensure the pointer to the memory is valid as long as the message exist! - for memory-saving
            on the arduino-board
        */
        log_message(const char *_msg, log_level _log_level);
        /*
            makes a deep-copy of the _msg string.
        */
        log_message(const String &_msg, log_level get_log_level);
        log_message(log_message &lm);
        ~log_message();

        const char *get_message() { return msg; }
        log_level get_log_level() { return logLevel; }

    private:
        log_level logLevel;
        char *msg;
        bool msg_deep_copy;
    };

} // namespace Log

Log::log_message::log_message(const char *_msg, log_level _log_level) : msg(const_cast<char *>(_msg)), logLevel(_log_level), msg_deep_copy(false) {}

Log::log_message::log_message(const String &_msg, log_level _log_level) : logLevel(_log_level)
{
    msg = new char[_msg.length() + 1];
    strcpy(msg, _msg.c_str());
}

Log::log_message::~log_message()
{
    if (msg_deep_copy)
    {
        delete[] msg;
    }
    msg = nullptr;
}
