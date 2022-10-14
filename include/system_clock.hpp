#pragma once
#include <Arduino.h>
#include <RTClib.h>

/*
    Documentation: https://github.com/Makuna/Rtc/wiki
*/

namespace Clock
{

    /**
     * @note maximum amount of time is 136 years...
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
         * @brief  Number of years in the duration
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

    /**
     * @brief allows storing and calculating time points from 2000 to 2130
     */
    class time_point
    {
    public:
        /**
         * @brief initialize everything with 0
         */
        time_point();
        time_point(DateTime &dt); // constructor for the DS3231 DateTime format
        time_point(const time_point &tp);
        ~time_point() {}

        /**
         * @return true if PM(13-24)
         */
        inline bool isPM() const { return this->hour > 12; }
        /**
         * @return a string representation of the time point
         */
        String to_string() const;
        /**
         * @return the hours in twelve hour format
         */
        inline uint8_t twelveHour() const { return this->hour > 12 ? this->hour - 12 : this->hour; }

        bool operator>(const time_point &t_p) const;
        bool operator<(const time_point &t_p) const;
        bool operator==(const time_point &t_p) const;

        time_point &operator+=(const duration &t_p);
        time_point operator+(const duration &t_p);
        time_point &operator-=(const duration &t_p);

        time_point &operator=(const time_point &t_p);

        uint8_t day_of_week() const;

        // set these values private and make getters

        uint16_t year; // real year - not a offste - example: 2022
        uint8_t month; // 1-12
        uint8_t m_day; // day of month
        uint8_t w_day; // week day(0-Sunday, 6-Saturday)

        uint8_t hour;   // 0-23
        uint8_t minute; // 0-60
        uint8_t second; // 0-60

    protected:
        uint8_t _get_days_in_month(uint8_t month, uint16_t _year);

        // add the duration type and adjust the date correctly - increment month and dates correctly to month
        void add_days(uint32_t num_of_days);
        void add_hours(uint32_t num_of_hours);
        void add_minutes(uint32_t num_of_minutes);
        void add_seconds(uint32_t num_of_seconds);
    };

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
#define SYSTEM_CLOCK_EEPROM_LAST_ADRESS 4095 // 0-4095 addresses - every address is one byte
    // String time_string(const RtcDateTime &tm);
}
