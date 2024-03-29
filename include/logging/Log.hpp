#pragma once
#include <Arduino.h>
#include "log_message.hpp"
#include "LinkedList.hpp"
#include "system_clock/system_clock.hpp"

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

        byte get_logging_level() const { return this->logging_level; }
        /**
         * @details sets the logging level to the given level
         * @note all previous logs stay as they are - are not removed or added - for that restart
         * @param _loggin_level the logging level to set
         */
        void set_logging_level(byte _logging_level) { this->logging_level = _logging_level; }

    private:
        SinglyLinkedList<log_message> _log_list;
        byte logging_level;
    };

} // namespace Log
