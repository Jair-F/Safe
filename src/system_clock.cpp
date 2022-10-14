#include "system_clock.hpp"

// ------------- duration -------------

#define SECSONDS_IN_YEAR 31536000 // 365 * 24 * 60 * 60
#define SECONDS_IN_DAY 86400      // 24 * 60 * 60
#define SECONDS_IN_HOUR 360       // 60 * 60
#define SECONDS_IN_MINUTE 60

Clock::duration::duration(uint32_t _minutes, uint32_t _secs) : secs(_minutes * SECONDS_IN_MINUTE + _secs)
{
}

Clock::duration::duration(uint32_t _hours, uint32_t _minutes, uint32_t _secs) : secs((_hours * SECONDS_IN_HOUR) +
                                                                                     (_minutes * SECONDS_IN_MINUTE) +
                                                                                     _secs)
{
    this->secs = _secs;
    this->secs += _minutes * 60;
    this->secs += _hours * 60 * 60;
}

Clock::duration::duration(uint16_t _days, uint32_t _hours, uint32_t _minutes, uint32_t _secs) : secs(_days * SECONDS_IN_DAY +
                                                                                                     _hours * SECONDS_IN_HOUR +
                                                                                                     _minutes * SECONDS_IN_MINUTE +
                                                                                                     secs)
{
    this->secs = _secs;
    this->secs += _minutes * 60;
    this->secs += _hours * 60 * 60;
    this->secs += _days * 60 * 60 * 24;
}

Clock::duration::duration(uint8_t _years, uint16_t _days, uint32_t _hours, uint32_t _minutes, uint32_t _secs) : secs(_years * SECSONDS_IN_YEAR +
                                                                                                                     _days * SECONDS_IN_DAY +
                                                                                                                     _hours * SECONDS_IN_HOUR +
                                                                                                                     _minutes * SECONDS_IN_MINUTE + secs)
{
    this->secs = _secs;
    this->secs += _minutes * 60;
    this->secs += _hours * 60 * 60;
    this->secs += _days * 60 * 60 * 24;
    this->secs += _years * 60 * 60 * 24 * 365;
}

uint32_t Clock::duration::total_minutes() const
{
    return this->secs / 60;
}
uint32_t Clock::duration::total_hours() const
{
    return this->secs / 60 * 60;
}
uint16_t Clock::duration::total_days() const
{
    return this->secs / 60 * 60 * 24;
}
uint8_t Clock::duration::total_years() const
{
    return this->secs / 60 * 60 * 24 * 365;
}

uint8_t Clock::duration::years() const
{
    return this->secs / SECSONDS_IN_YEAR;
}
uint8_t Clock::duration::days() const
{
    return (this->secs / SECONDS_IN_DAY) % 365;
}
uint8_t Clock::duration::hours() const
{
    return (this->secs / SECONDS_IN_HOUR) % 24;
}
uint8_t Clock::duration::minutes() const
{
    return (this->secs / SECONDS_IN_MINUTE) % 60;
}
uint8_t Clock::duration::seconds() const
{
    return this->secs % SECONDS_IN_MINUTE;
}

/*






*/

Clock::Clock::Clock() : rtc(nullptr)
{
    // this->begin();
}

Clock::Clock::~Clock()
{
    if (this->rtc != nullptr)
        delete rtc;
}

bool Clock::Clock::begin()
{
    // Wire.begin();
    delay(100);
    if (this->rtc == nullptr)
    {
        this->rtc = new RTC_DS3231();
        return this->rtc->begin();
    }
}

bool Clock::Clock::lost_power()
{
    if (this->rtc == nullptr)
        return true;
    else
        return this->rtc->lostPower();
}

Clock::time_point Clock::Clock::now()
{
    DateTime tm_now = this->rtc->now();
    time_point ret(tm_now);

    return ret;
}

bool Clock::Clock::set_date_time(time_point tm_point)
{
    DateTime set_tm_pt(tm_point.year, tm_point.month, tm_point.m_day,
                       tm_point.hour, tm_point.minute, tm_point.second);
    this->rtc->adjust(set_tm_pt);
}

/*






*/

// ------------- time_point -------------
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

        // we are adding here day by day - adjust the weekday accordingly
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

uint8_t Clock::time_point::day_of_week() const
{
    // 1. January 2000 was a Saturday...
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
