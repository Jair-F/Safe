#include "system_clock/system_clock.hpp"
#include "system_clock/time_point.hpp"

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
