#ifndef _LOG_HPP_INCLUDED_
#define _LOG_HPP_INCLUDED_

#include <Arduino.h>
#include "log_message.hpp"
#include "LinkedList.hpp"
#include "system_clock.hpp"

namespace Log
{

    // ---------

    class Log
    {
    public:
        /*
            @param logging_level all logs which are lower(L_INFO < L_CRITICAL) than this level will not be recorded
        */
        Log(byte _logging_level);

        /*
            makes not a deep copy of the _msg
        */
        void log(const String msg, byte _log_level);
        void log(const char *msg, byte _log_level);
        void log(log_message &lm);

        /*
            print all saved logs on serial
        */
        void serial_dump();

        const SinglyLinkedList<log_message> &log_list() const { return this->_log_list; }
        SinglyListNodeIterator<log_message> begin() { return _log_list.begin(); }
        SinglyListNodeIterator<log_message> end() { return _log_list.end(); }

    private:
        SinglyLinkedList<log_message> _log_list;
        byte logging_level;
    };

} // namespace Log

Log::Log::Log(byte _logging_level) : _log_list(), logging_level(_logging_level)
{
}

void Log::Log::log(const String msg, byte _log_level)
{
    if (_log_level >= this->logging_level)
    {
        this->_log_list.push_back(log_message(msg, _log_level));
    }
}
void Log::Log::log(const char *msg, byte _log_level)
{
    if (_log_level >= this->logging_level)
    {
        this->_log_list.push_back(log_message(msg, _log_level));
    }
}

void Log::Log::serial_dump()
{
    Serial.println(F("BEGIN ERROR_OUTPUT:"));
    Serial.println();

    auto log_iterator = this->begin();
    while (log_iterator != this->end())
    {
        String tmp;
        tmp += '[';
        tmp += Clock::time_string(log_iterator.data().time_point());
        tmp += F("] ");
        switch (log_iterator.data().Level())
        {
        case ::Log::log_level::L_DEBUG:
        {
            tmp += F("DEBUG: ");
            break;
        }
        case ::Log::log_level::L_CRITICAL:
        {
            tmp += F("CRITICAL: ");
            break;
        }
        case ::Log::log_level::L_ERROR:
        {
            tmp += F("ERROR: ");
            break;
        }
        case ::Log::log_level::L_INFO:
        {
            tmp += F("INFO: ");
            break;
        }
        case ::Log::log_level::L_WARNING:
        {
            tmp += F("WARNING: ");
            break;
        }

        default:
            // tmp = "!!!!!  ERROR  !!!!! at logging format";
            break;
        }
        tmp += log_iterator.data().message();
        Serial.println(tmp);
        ++log_iterator;
    }
    Serial.println();
    Serial.println(F("END ERROR_OUTPUT:"));
}

#endif // _LOG_HPP_INCLUDED_