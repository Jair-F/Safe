#pragma once
#include "Widget.hpp"

namespace UI
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

        inline uint8_t get_weight() const
        {
            if (this->alignment == d_alignment::AL_HORIZONTAL)
                return this->get_content_height();
            else
                return this->get_content_width();
            // return this->thickness;
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
