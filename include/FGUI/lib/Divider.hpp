#pragma once
#include "Widget.hpp"

namespace FGUI
{
    class Divider : public Widget
    {
    public:
        enum d_alignment
        {
            AL_HORIZONTAL = 0,
            AL_VERTICAL
        };
        Divider(WindowBase *_parent, position _start_position, uint16_t _length,
                uint8_t width, d_alignment _alignment, unsigned int _color);

        /*
            calculate the width no matter how the Divider is aligned
        */
        inline uint8_t get_width() const
        {
            if (this->alignment == d_alignment::AL_HORIZONTAL)
                return this->height() + 1;
            else
                return this->width() + 1;
        }

        /*
            wrapper for released_background_color
            the Divider uses the background_color of the widget as its color value
        */
        void set_color(unsigned int color) { this->released_background_color = color; }

    protected:
    private:
        d_alignment alignment;
    };
}
