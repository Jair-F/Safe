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
                uint8_t _thickness, d_alignment _alignment, unsigned int _color);

        inline uint8_t get_thickness() const
        {
            return this->thickness;
        }

    protected:
        void _draw_released_widget() override;
        void _draw_pressed_widget() override { this->_draw_released_widget(); }
        void _draw_widget() override { this->_draw_released_widget(); }

    private:
        uint8_t thickness;
        d_alignment alignment;
        unsigned int color;
    };
}
