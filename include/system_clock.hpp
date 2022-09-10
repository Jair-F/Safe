#pragma once

#include <Arduino.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>
#include "GlobalConstants.hpp"

/*
    Documentation: https://github.com/Makuna/Rtc/wiki
*/

namespace Clock
{

    template <class WIRING_METHOD>
    class Clock : public RtcDS1302<WIRING_METHOD>
    {
    public:
        Clock(WIRING_METHOD &wire);
        Clock(unsigned short data, unsigned short clk, unsigned short rst);

        /*
            @return true if the clock lost power
        */
        bool lost_power() { return lostPower; }

        virtual ~Clock() {}

    protected:
        void start_clock();

    private:
        WIRING_METHOD wiring;
        bool lostPower = false; // true if the battery of the rtc_clock_module is empty
    };

/*
    - The RTC-Module has a "RAM" where we can store data while the battery in the clock module isnt empty.
    if the battery is removed the data will be lost.
    - the indexes of the memory are 0-30 each a uint8_t=char
*/
#define SYSTEM_CLOCK_MEMORY_LENGTH 30
    String time_string(const RtcDateTime &tm);
}

// --------------------- Implementation ---------------------

template <class WIRING_METHOD>
Clock::Clock<WIRING_METHOD>::Clock(unsigned short data, unsigned short clk, unsigned short rst) : wiring(data, clk, rst), RtcDS1302<WIRING_METHOD>(wiring)
{
    this->start_clock();
    if (this->GetIsRunning())
    {
        this->SetIsRunning(true);
    }
}

template <class WIRING_METHOD>
Clock::Clock<WIRING_METHOD>::Clock(WIRING_METHOD &wire) : wiring(wire), RtcDS1302<WIRING_METHOD>(wire)
{
    this->start_clock();
}

template <class WIRING_METHOD>
void Clock::Clock<WIRING_METHOD>::start_clock()
{
    this->Begin();

    if (this->GetIsWriteProtected())
        this->SetIsWriteProtected(false);
    if (!this->GetIsRunning())
        this->SetIsRunning(true);

    if (!this->IsDateTimeValid())
    {
        this->lostPower = true;
    }
}
