#pragma once

#include <Arduino.h>
#include "Widget.hpp"

namespace UI
{
    class TextField
    {
    public:
    private:
        String _text;
        uint8_t *_text_font = SmallFont;
    };

} // namespace UI
