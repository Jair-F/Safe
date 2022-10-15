#pragma once
#include <Arduino.h>

namespace Clock
{
    /**
     * @note maximum amount of time duration can hold is 136 years...
     */
    class duration
    {
    public:
        duration(uint32_t _secs) : secs(_secs) {}
        duration(uint32_t _minutes, uint32_t _secs);
        duration(uint32_t _hours, uint32_t _minutes, uint32_t _secs);
        duration(uint16_t _days, uint32_t _hours, uint32_t _minutes, uint32_t _secs);
        duration(uint8_t _years, uint16_t _days, uint32_t _hours, uint32_t _minutes, uint32_t _secs);

        /**
         * @brief total seconds/minutes... in duration - only ful time sections - not 1.2 years - 1 year
         */
        uint32_t total_seconds() const { return this->secs; }
        uint32_t total_minutes() const;
        uint32_t total_hours() const;
        uint16_t total_days() const;
        uint8_t total_years() const;

        /**
         * @brief  Number of years in the duration(365 days one year)
         *         e.g. 4
         * @return uint8_t years
         * @note day time of leap years are ignored
         */
        uint8_t years() const;
        /**
         * @brief  Number of days in the duration
         *         This is not the total days, it includes the years
         *         e.g. 4 years, 3 days - NOT 1463
         * @return uint8_t days
         * @note day time of leap years are ignored
         */
        uint8_t days() const;
        /**
         * @brief  Number of hours in the duration
         *         This is not the total hours, it includes the years,days
         *         e.g. 4 years, 3 days, 3 hours - NOT 35115
         * @return uint8_t hours
         * @note day time of leap years are ignored
         */
        uint8_t hours() const;
        /**
         * @brief  Number of minutes in the duration
         *         This is not the total minutes, it includes the years,days,hours
         *         e.g. 4 years, 3 days, 3 hours, 2 minutes- NOT 2106902
         * @return uint8_t minutes
         * @note day time of leap years are ignored
         */
        uint8_t minutes() const;
        /**
         * @brief  Number of seconds in the duration
         *         This is not the total seconds, it includes the years,days,hours,minutes
         *         e.g. 4 years, 3 days, 3 hours, 2 minutes, 3 seconds- NOT 2106905
         * @return uint8_t seconds
         * @note day time of leap years are ignored
         */
        uint8_t seconds() const;

    private:
        uint32_t secs; // can store a little less than 136.2 years...
    };
} // namespace Clock
