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
                uint8_t _thickness, d_alignment _alignment, unsigned int _color) : Widget(_parent, _start_position, {_start_position.x_pos, _start_position.y_pos}),
                                                                                   thickness(_thickness), alignment(_alignment), color(_color)
        {
            switch (this->alignment)
            {

            case d_alignment::AL_HORIZONTAL:
            {
                this->lower_right.x_pos = _length;
                this->lower_right.y_pos = this->upper_left.y_pos + _thickness - 1;
                break;
            }
            case d_alignment::AL_VERTICAL:
            {
                this->lower_right.x_pos = this->upper_left.x_pos + _thickness - 1;
                this->lower_right.y_pos = this->upper_left.y_pos + _length;
                break;
            }

            default:
                break;
            }
        }

        inline uint8_t get_thickness() const
        {
            return this->thickness;
        }

    protected:
        void _draw_released_widget() override
        {
            this->display->setColor(this->color);
            this->display->fillRect(this->upper_left.x_pos, this->upper_left.y_pos, this->lower_right.x_pos, this->lower_right.y_pos);
        }
        void _draw_pressed_widget() override { this->_draw_released_widget(); }
        void _draw_widget() override { this->_draw_released_widget(); }

    private:
        uint8_t thickness;
        d_alignment alignment;
        unsigned int color;
    };
}
