#pragma once

#include "Touch_Widget.hpp"

namespace UI
{
    template <typename CALL_OBJECT_TYPE>
    class ButtonBase : public Touch_Widget<CALL_OBJECT_TYPE>
    {
    public:
        ButtonBase(WindowBase *parent, position _upper_left, position _lower_right,
                   CALL_OBJECT_TYPE *_call_object) : Touch_Widget<CALL_OBJECT_TYPE>(parent, _upper_left, _lower_right, _call_object) {}

        virtual ~ButtonBase() {}

        /*
            the color values are RGB-565 values(16-bit value)
            RGB-565 color picker: https://chrishewett.com/blog/true-rgb565-colour-picker/
        */
        unsigned int released_background_color = VGA_BLACK;
        unsigned int released_border_color = VGA_WHITE;

        unsigned int pressed_background_color = VGA_WHITE;
        unsigned int pressed_border_color = VGA_WHITE;

    protected:
        virtual void _draw_widget() override;
        virtual void _draw_released_widget() override;
        virtual void _draw_pressed_widget() override;

    private:
    };
}

// ------------- template implementation -------------

template <typename CALL_OBJECT_TYPE>
void UI::ButtonBase<CALL_OBJECT_TYPE>::_draw_pressed_widget()
{
    // this->_clear_widget_space();

    // draw background-color
    this->display->setColor(this->pressed_background_color);
    this->display->fillRect(this->upper_left.x_pos, this->upper_left.y_pos,
                            this->lower_right.x_pos, this->lower_right.y_pos);

    // draw the border
    this->display->setColor(this->pressed_border_color);
    this->display->drawRect(this->upper_left.x_pos, this->upper_left.y_pos,
                            this->lower_right.x_pos, this->lower_right.y_pos);
}

template <typename CALL_OBJECT_TYPE>
void UI::ButtonBase<CALL_OBJECT_TYPE>::_draw_released_widget()
{

    // this->_clear_widget_space();

    // draw background-color
    this->display->setColor(this->released_background_color);
    this->display->fillRect(this->upper_left.x_pos, this->upper_left.y_pos,
                            this->lower_right.x_pos, this->lower_right.y_pos);

    // draw the border
    this->display->setColor(this->released_border_color);
    this->display->drawRect(this->upper_left.x_pos, this->upper_left.y_pos,
                            this->lower_right.x_pos, this->lower_right.y_pos);
}

template <typename CALL_OBJECT_TYPE>
void UI::ButtonBase<CALL_OBJECT_TYPE>::_draw_widget()
{
    if (!this->is_hidden())
    {
        if (this->is_touched())
        {
            this->_draw_pressed_widget();
        }
        else
        {
            this->_draw_released_widget();
        }
    }
}