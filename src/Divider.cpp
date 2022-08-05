#include "UI/UI.hpp"

UI::Divider::Divider(WindowBase *_parent, position _start_position, uint16_t _length,
                     uint8_t width, d_alignment _alignment, unsigned int _color) : Widget(_parent, _start_position, {_start_position.x_pos, _start_position.y_pos}),
                                                                                   alignment(_alignment)
{
    this->released_background_color = _color;

    switch (this->alignment)
    {

    case d_alignment::AL_HORIZONTAL:
    {
        this->lower_right.x_pos = _length;
        this->lower_right.y_pos = this->upper_left.y_pos + width - 1;
        break;
    }
    case d_alignment::AL_VERTICAL:
    {
        this->lower_right.x_pos = this->upper_left.x_pos + width - 1;
        this->lower_right.y_pos = this->upper_left.y_pos + _length;
        break;
    }

    default:
        break;
    }
    this->set_draw_border(false);
}