#pragma once
#include <Arduino.h>
#include <RTClib.h> // for the conversion constructor of DateTime to time_point
#include "duration.hpp"

namespace Clock
{
    /**
     * @brief allows storing and calculating time points from 2000 to 2130
     *
     * @details a class for holding dates and times and shifting them around with
     *          durations to make a more abstract handeling of time and date to be able to
     *          switch easily to another system_clock module if required
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
        inline bool isPM() const;
        /**
         * @return a string representation of the time point
         */
        String to_string() const;
        /**
         * @return a string representation of the time point but only the date
         *         and time - not day of week
         */
        String to_string_time_date() const;
        /**
         * @return a string representation of the time point but only the date
         */
        String to_string_date() const;
        /**
         * @return a string representation of the time point but only the time
         */
        String to_string_time() const;
        /**
         * @return the hours in twelve hour format
         */
        inline uint8_t twelveHour() const;

        bool operator>(const time_point &t_p) const;
        bool operator<(const time_point &t_p) const;
        bool operator==(const time_point &t_p) const;

        /**
         * @note adding or subtracting time pionts doesnt considers also the leap years -
         *
         * now = 11/10/2023 12:24:30
         * if we add now 1 year and one day and the next year(2024) is a leap year we will get: 11/10/2024 12:24:30 (due to the 1 day more in February...)
         * !! and not 12/10/2024 12:24:30 !!
         *
         * if its not a leap year - its like normal: 12/10/2024 12:24:30
         *
         */
        time_point &operator+=(const duration &t_p);
        time_point operator+(const duration &t_p);
        time_point &operator-=(const duration &t_p);
        time_point operator-(const duration &t_p);

        time_point &operator=(const time_point &t_p);

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

        void remove_days(uint32_t num_of_days);
        void remove_hours(uint32_t num_of_hours);
        void remove_minutes(uint32_t num_of_minutes);
        void remove_seconds(uint32_t num_of_seconds);
    };
} // namespace Clock
