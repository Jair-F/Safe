#pragma once

namespace Log
{
    enum log_level
    {
        L_DEBUG = 0,
        L_INFO,
        L_WARNING,
        L_ERROR,
        L_CRITICAL
    };

    class log_message
    {
    private:
        char *msg;
        log_level level;
        bool msg_deep_copy;

    public:
        /*
            !!! IMPORTANT !!! this saves only the pointer to the beggining from the msg - makes not a deep copy
            please ensure the pointer to the memory is valid as long as the message exist! - for memory-saving
            on the arduino-board
        */
        log_message(const char *_msg, log_level _LogLevel);
        /*
            makes a deep-copy of the _msg string.
        */
        log_message(const String &_msg, log_level get_LogLevel);
        log_message(log_message &lm);
        log_message &operator=(const log_message &);
        ~log_message();

        const char *message() const;
        log_level Level() const;
        /*
            @return true if the message is a deep copy
        */
        bool _msg_is_deep_copy() const;
    };

} // namespace Log

Log::log_message::log_message(const char *_msg, log_level _LogLevel) : msg(const_cast<char *>(_msg)), level(_LogLevel), msg_deep_copy(false) {}

Log::log_message::log_message(const String &_msg, log_level _LogLevel) : level(_LogLevel)
{
    msg = new char[_msg.length() + 1];
    strcpy(msg, _msg.c_str());
}
Log::log_message &Log::log_message::operator=(const log_message &_lm)
{
    this->msg = const_cast<char *>(_lm.message());
    this->level = _lm.Level();
    this->msg_deep_copy = _lm._msg_is_deep_copy();
}
Log::log_message::~log_message()
{
    if (msg_deep_copy)
    {
        delete[] msg;
    }
    msg = nullptr;
}

const char *Log::log_message::message() const
{
    return this->msg;
}

Log::log_level Log::log_message::Level() const
{
    return this->level;
}

bool Log::log_message::_msg_is_deep_copy() const
{
    return this->msg_deep_copy;
}
