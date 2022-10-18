#include "system_clock/duration.hpp"

#define SECSONDS_IN_YEAR 31536000 // 365 * 24 * 60 * 60
#define SECONDS_IN_DAY 86400      // 24 * 60 * 60
#define SECONDS_IN_HOUR 360       // 60 * 60
#define SECONDS_IN_MINUTE 60

Clock::duration::duration(uint32_t _minutes, uint32_t _secs) : secs(_minutes * SECONDS_IN_MINUTE + _secs)
{
}

Clock::duration::duration(uint32_t _hours, uint32_t _minutes, uint32_t _secs) : secs(0)
{
    this->secs = _secs;
    this->secs += _minutes * 60;
    this->secs += _hours * 60 * 60;
}

Clock::duration::duration(uint16_t _days, uint32_t _hours, uint32_t _minutes, uint32_t _secs) : secs(0)
{
    this->secs = _secs;
    this->secs += _minutes * 60;
    this->secs += _hours * 60 * 60;
    this->secs += _days * 60 * 60 * 24;
}

Clock::duration::duration(uint8_t _years, uint16_t _days, uint32_t _hours, uint32_t _minutes, uint32_t _secs) : secs(0)
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
