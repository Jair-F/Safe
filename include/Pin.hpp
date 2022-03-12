#pragma once

#include "Unlock_Object.hpp"

namespace Pin
{
    class Pin : public Unlock_Object
    {
    public:
        Pin(Lock::unlock_token *_utoken, bool enabled = true) : Unlock_Object(_utoken, enabled) {}
        ~Pin() {}

        virtual void loop() override {}

        /*

        */
        char get_input();

    private:
        String unlock_pin;
    };

}
