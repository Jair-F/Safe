#include "FGUI/FGUI.hpp"

FGUI::Divider::Divider(WindowBase *_parent,
                       position _start_position,
                       uint16_t _length,
                       uint8_t _width,
                       d_alignment _alignment,
                       unsigned int _color) : Widget(_parent,
                                                     _start_position,
                                                     _length, _width,
                                                     0, 0), // no border
                                              alignment(_alignment)
{
    this->released_background_color = _color;

    switch (this->alignment)
    {

    case d_alignment::AL_HORIZONTAL:
    {
        // if alignment is horizontal the constructor did already everthing right
        // this->lower_right.x_pos = _length;
        // this->lower_right.y_pos = this->upper_left.y_pos + _width - 1;
        break;
    }
    case d_alignment::AL_VERTICAL:
    {
        this->lower_right.x_pos = this->upper_left.x_pos + _width - 1;
        this->lower_right.y_pos = this->upper_left.y_pos + _length - 1;
        break;
    }
    }
    this->set_draw_border(false);
}