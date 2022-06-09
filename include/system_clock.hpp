#pragma once

#include <Arduino.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>
#include "GlobalConstants.hpp"
#include "GlobalVariables.hpp"

/*
    Documentation: https://github.com/Makuna/Rtc/wiki
*/

namespace Clock
{

    template <class WIRING_METHOD>
    class Clock : public RtcDS1302<WIRING_METHOD>
    {
    public:
        Clock(WIRING_METHOD &wire) : wiring(wire), RtcDS1302<WIRING_METHOD>(wire)
        {
            this->start_clock();
            if (this->lost_power())
            {
                // logger needs system-clock - systemclock cant log on failure...
                // logger.log(F("SYSTEM_CLOCK: RTC-Module lost power - replace battary"), Log::log_level::L_WARNING);
            }
            // logger.log(F("Clock module startup"), Log::log_level::L_INFO);
        }
        Clock(unsigned short data, unsigned short clk, unsigned short rst) : wiring(data, clk, rst), RtcDS1302<WIRING_METHOD>(wiring)
        {
            this->start_clock();
        }

        /*
            @return true if the clock lost power
        */
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
            }
        }

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

    String time_string(const RtcDateTime &tm)
    {
        String ret;

        switch (tm.DayOfWeek())
        {
        case 0:
        {
            ret += F("Sun");
            break;
        }
        case 1:
        {
            ret += F("Mon");
            break;
        }
        case 2:
        {
            ret += F("Tue");
            break;
        }
        case 3:
        {
            ret += F("Wed");
            break;
        }
        case 4:
        {
            ret += F("Thu");
            break;
        }
        case 5:
        {
            ret += F("Fri");
            break;
        }
        case 6:
        {
            ret += F("Sat");
            break;
        }
        default:
            break;
        }

        ret += ' ';

        ret += tm.Day();
        ret += '/';
        ret += tm.Month();
        ret += '/';
        ret += tm.Year();

        ret += ' ';

        ret += tm.Hour();
        ret += ':';
        ret += tm.Minute();
        ret += ':';
        ret += tm.Second();

        return ret;
    }

}
/*
    Base-Clock-Module:
        ThreeWire rtc_wire(RTC_DATA, RTC_CLK, RTC_RST); // DATA/IO, CLK/SCLK, RST/CE
        RtcDS1302<ThreeWire> rtc(rtc_wire);
*/

/*
    ready setup clock-object
    to ensure the battery of the clock is not empty check the lost_power method in the setup.
*/
Clock::Clock<ThreeWire> system_clock(RTC_DATA, RTC_CLK, RTC_RST);
