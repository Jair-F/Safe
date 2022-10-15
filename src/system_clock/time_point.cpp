#include "system_clock/time_point.hpp"

Clock::time_point::time_point() : year(0), month(0), m_day(0), w_day(0),
                                  hour(0), minute(0), second(0)
{
}

Clock::time_point::time_point(DateTime &dt) : year(dt.year()), month(dt.month()), m_day(dt.day()), w_day(dt.dayOfTheWeek()),
                                              hour(dt.hour()), minute(dt.minute()), second(dt.second())
{
}

Clock::time_point::time_point(const time_point &tp) : year(tp.year), month(tp.month), m_day(tp.m_day), w_day(tp.w_day),
                                                      hour(tp.hour), minute(tp.minute), second(tp.second)
{
}

bool Clock::time_point::isPM() const
{
    return this->hour > 12;
}

uint8_t Clock::time_point::twelveHour() const
{
    return this->hour > 12 ? this->hour - 12 : this->hour;
}

Clock::time_point &Clock::time_point::operator=(const time_point &t_p)
{
    this->year = t_p.year;
    this->month = t_p.month;
    this->m_day = t_p.m_day;
    this->w_day = t_p.w_day;

    this->hour = t_p.hour;
    this->minute = t_p.minute;
    this->second = t_p.second;
    return *this;
}

bool Clock::time_point::operator<(const time_point &t_p) const
{
    return (this->year < t_p.year ||
            (this->year == t_p.year &&
             (this->month < t_p.month ||
              (this->month == t_p.month &&
               (this->m_day < t_p.m_day ||
                (this->m_day == t_p.m_day &&
                 (this->hour < t_p.hour ||
                  (this->hour == t_p.hour &&
                   (this->minute < t_p.minute || (this->minute == t_p.minute && this->second < t_p.second))))))))));
}

bool Clock::time_point::operator>(const time_point &t_p) const
{
    return !(this->operator<(t_p));
}
bool Clock::time_point::operator==(const time_point &t_p) const
{
    return this->year == t_p.year &&
           this->month == t_p.month &&
           this->m_day == t_p.m_day &&
           this->w_day == t_p.w_day &&
           this->hour == t_p.hour &&
           this->minute == t_p.minute &&
           this->second == t_p.second;
}

Clock::time_point &Clock::time_point::operator+=(const duration &t_p)
{
    this->add_seconds(t_p.total_seconds());
    return *this;
}

Clock::time_point Clock::time_point::operator+(const duration &t_p)
{
    time_point ret(*this);
    ret += t_p;
    return ret;
}

Clock::time_point Clock::time_point::operator-(const duration &t_p)
{
    time_point ret(*this);
    ret -= t_p;
    return ret;
}

Clock::time_point &Clock::time_point::operator-=(const duration &t_p)
{
    this->remove_seconds(t_p.total_seconds());
    return *this;
}

uint8_t Clock::time_point::_get_days_in_month(uint8_t _month, uint16_t _year)
{
    switch (_month)
    {
    case 1: // January
    {
        return 31;
        break;
    }
    case 2: // February
    {
        if (_year % 4 == 0) // leap year
            return 29;
        else
            return 28;
        break;
    }
    case 3: // March
    {
        return 31;
        break;
    }
    case 4: // April
    {
        return 30;
        break;
    }
    case 5: // May
    {
        return 31;
        break;
    }
    case 6: // June
    {
        return 30;
        break;
    }
    case 7: // July
    {
        return 31;
        break;
    }
    case 8: // August
    {
        return 31;
        break;
    }
    case 9: // September
    {
        return 30;
        break;
    }
    case 10: // October
    {
        return 31;
        break;
    }
    case 11: // November
    {
        return 30;
        break;
    }
    case 12: // December
    {
        return 31;
        break;
    }
    default:
    {
        // error month is out of range...
        Serial.println("_get_days_in_month: month is out of range!!");
        break;
    }
    }
}

void Clock::time_point::add_days(uint32_t num_of_days)
{

    // add the days day by day to ensure to consider every leap year and days in month and adjusts the day of week
    while (num_of_days > 0)
    {
        uint8_t days_in_current_month = this->_get_days_in_month(this->month, this->year);

        if (this->m_day == days_in_current_month) // if we are at the last day of the current month
        {
            this->m_day = 1;
            if (this->month == 12)
            {
                this->month = 1;
                ++this->year;
            }
            else
                ++this->month;
        }
        else
        {
            ++this->m_day;
        }

        // we are adding day by day - adjust the weekday accordingly
        if (this->w_day == 6)
        {
            this->w_day = 0;
        }
        else
        {
            ++this->w_day;
        }

        --num_of_days;
    }
}

void Clock::time_point::add_hours(uint32_t num_of_hours)
{
    this->add_days(num_of_hours / 24);
    uint8_t remaining_hours = num_of_hours % 24; // hours left to after added the days

    // if the actual hours with the remaining hours make another day add the day
    if (this->hour + remaining_hours >= 24)
    {
        remaining_hours = this->hour + remaining_hours - 24; // hours left after added the day
        this->hour = 0;
        this->add_days(1);
    }
    this->hour += remaining_hours;
}

void Clock::time_point::add_minutes(uint32_t num_of_minutes)
{
    this->add_hours(num_of_minutes / 60);
    uint8_t remaining_minutes = num_of_minutes % 60; // minutes left to after added the hours

    // if the actual minutes with the remaining minutes make another hour add the hour
    if (this->minute + remaining_minutes >= 60)
    {
        remaining_minutes = this->minute + remaining_minutes - 60;
        this->minute = 0;
        this->add_hours(1);
    }
    this->minute += remaining_minutes;
}

void Clock::time_point::add_seconds(uint32_t num_of_seconds)
{
    this->add_minutes(num_of_seconds / 60);
    uint8_t remaining_seconds = num_of_seconds % 60; // seconds left to after added the minutes

    if (this->second + remaining_seconds >= 60)
    {
        remaining_seconds = this->second + remaining_seconds - 60;
        this->second = 0;
        this->add_minutes(1);
    }
    this->second += remaining_seconds;
}

void Clock::time_point::remove_days(uint32_t num_of_days)
{
    // remove the days day by day to ensure to consider every leap year and days in month and adjusts the day of week
    while (num_of_days > 0)
    {

        if (this->m_day > 1) // if we arent at the first day of a month
        {
            --this->m_day;
        }
        else
        {
            // we are at the first day of the month - need to switch the month
            if (this->month > 1)
                --this->month;
            else
            {
                // if we are at the begin of the year - January
                --this->year;
                this->month = 12;
            }

            this->m_day = this->_get_days_in_month(this->month, this->year);
        }

        // we are subtracting day by day - adjust the weekday accordingly
        if (this->w_day == 0)
        {
            this->w_day = 6;
        }
        else
        {
            --this->w_day;
        }

        --num_of_days;
    }
}

void Clock::time_point::remove_hours(uint32_t num_of_hours)
{
    this->remove_days(num_of_hours / 24);
    uint8_t remaining_hours = num_of_hours % 24;

    if (remaining_hours > this->hour)
    {
        remaining_hours = remaining_hours - this->hour;
        this->hour = 23;
        this->remove_hours(1);
    }
    this->hour -= remaining_hours;
}

void Clock::time_point::remove_minutes(uint32_t num_of_minutes)
{
    this->remove_hours(num_of_minutes / 60);
    uint8_t remaining_minutes = num_of_minutes % 60;

    if (remaining_minutes > this->minute)
    {
        remaining_minutes = remaining_minutes - this->minute;
        this->minute = 59;
        this->remove_hours(1);
    }
    this->minute -= remaining_minutes;
}

void Clock::time_point::remove_seconds(uint32_t num_of_seconds)
{
    this->remove_minutes(num_of_seconds / 60);
    uint8_t remaining_seconds = num_of_seconds % 60;

    if (remaining_seconds > this->second) // we also need to subtract a minute
    {
        remaining_seconds = remaining_seconds - this->second;
        this->second = 59;
        this->remove_minutes(1);
    }
    this->second -= remaining_seconds;
}

String Clock::time_point::to_string() const
{
    String ret;

    switch (this->w_day)
    {
    case 0:
    {
        ret += F("Sunday");
        break;
    }
    case 1:
    {
        ret += F("Monday");
        break;
    }
    case 2:
    {
        ret += F("Tuesday");
        break;
    }
    case 3:
    {
        ret += F("Wednesday");
        break;
    }
    case 4:
    {
        ret += F("Thursday");
        break;
    }
    case 5:
    {
        ret += F("Friday");
        break;
    }
    case 6:
    {
        ret += F("Saturday");
        break;
    }
    default:
    {
        ret += F("DAY_OUT_OF_RANGE");
        break;
    }
    }

    ret += ' ';

    ret += this->m_day;
    ret += '/';
    ret += this->month;
    ret += '/';
    ret += this->year;

    ret += ' ';

    ret += this->hour;
    ret += ':';
    ret += this->minute;
    ret += ':';
    ret += this->second;

    return ret;
}
