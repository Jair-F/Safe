#pragma once

#include <ThreeWire.h>
#include <RtcDS1302.h>
#include "GlobalConstants.hpp"

/*
    Documentation: https://github.com/Makuna/Rtc/wiki
*/

template <class WIRING_METHOD>
class Clock : public RtcDS1302<WIRING_METHOD>
{
public:
    Clock(WIRING_METHOD &wire) : RtcDS1302<WIRING_METHOD>(wire), wiring(wire)
    {
        this->start_clock();
    }
    Clock(unsigned short data, unsigned short clk, unsigned short rst) : wiring(data, clk, rst), RtcDS1302<WIRING_METHOD>(wiring)
    {
        this->start_clock();
    }

    bool lost_power() { return lostPower; }

    virtual ~Clock() {}

protected:
    void start_clock()
    {
        this->Begin();

        if (this->GetIsWriteProtected())
            this->SetIsWriteProtected(false);
        if (!this->GetIsRunning())
            this->SetIsRunning(true);

        if (!this->IsDateTimeValid())
        {
            this->lostPower = true;
            Serial.println("lost power");
        }
    }

private:
    WIRING_METHOD wiring;
    bool lostPower = false; // true if the battery of the rtc_clock_module is empty
};

/*
    Base-Clock-Module:
        ThreeWire rtc_wire(RTC_DATA, RTC_CLK, RTC_RST); // DATA/IO, CLK/SCLK, RST/CE
        RtcDS1302<ThreeWire> rtc(rtc_wire);
*/

/*
    ready setup clock-object
    to ensure the battery of the clock is not empty check the lost_power method in the setup.
*/
Clock<ThreeWire> system_clock(RTC_DATA, RTC_CLK, RTC_RST);

/*
    - The RTC-Module has a "RAM" where we can store data while the battery in the clock module isnt empty.
    if the battery is removed the data will be lost.
    - the indexes of the memory are 0-30 each a uint8_t=char
*/
#define SYSTEM_CLOCK_MEMORY_LENGTH 30
