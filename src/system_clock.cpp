#include "system_clock.hpp"



    String Clock::time_string(const RtcDateTime &tm)
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
