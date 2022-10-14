#pragma once
#include <Arduino.h>
#include "log_message.hpp"
#include "LinkedList.hpp"
#include "system_clock.hpp"

namespace Log
{

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

        const SinglyLinkedList<log_message> &log_list() const;
        SinglyListNodeIterator<log_message> begin();
        SinglyListNodeIterator<log_message> end();

    private:
        SinglyLinkedList<log_message> _log_list;
        byte logging_level;
    };

} // namespace Log
