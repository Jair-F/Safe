#pragma once
#include <Arduino.h>
#include <RTClib.h>

#include "duration.hpp"
#include "time_point.hpp"

/*
    Documentation: https://github.com/Makuna/Rtc/wiki
*/

namespace Clock
{

    class Clock
    {
    public:
        Clock();
        ~Clock();

        /**
         * @return false if the clock was stopped due to power loss
         */
        bool lost_power();

        /**
         * @return true if starting the clock and testing the clock
         * connection was successful
         */
        bool begin();

        time_point now();
        /**
         * @return true on success
         */
        bool set_date_time(time_point tm_point);

    protected:
        void start_clock();

    private:
        RTC_DS3231 *rtc = nullptr;
    };

/*
    - The RTC-Module has a "RAM" where we can store data while the battery in the clock module isnt empty.
    if the battery is removed the data will be lost.
    - the indexes of the memory are 0-30 each a uint8_t=char
*/
#define SYSTEM_CLOCK_EEPROM_LAST_ADRESS 4095 // 0-4095 addresses - every address is one byte/8-bits
    // String time_string(const RtcDateTime &tm);
}
