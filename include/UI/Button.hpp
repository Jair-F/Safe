#pragma once

#include "Widget.hpp"
#include <UTFT.h>
#include <URTouch.h>

extern uint8_t SmallFont[];
extern uint8_t BigFont[];

namespace UI
{
    class Button : public Widget
    {
    public:
        Button(Window *parent, const position _upper_left_pos_pos, const position _lower_right_pos, UTFT &_display, URTouch &_touch) : Widget(parent, parent->_calc_absolute_pos(_upper_left_pos_pos), parent->_calc_absolute_pos(_lower_right_pos), _display, _touch)
        {
            // this->_draw_released_widget();
        }

        void setFont(uint8_t *_font)
        {
            this->_text_font = _font;
        }

        /*
            set text of the button - without updating a already displayed widget
            - need to call update();
        */
        void setText(String _text)
        {
            text = _text;
            // this->_draw_released_widget();
        }

        void update()
        {
            if (!this->is_hidden())
            {
                this->_draw_released_widget();
            }
        }

    protected:
/*
    the color values are RGB-565 values(16-bit value)
    RGB-565 color picker: https://chrishewett.com/blog/true-rgb565-colour-picker/
*/
#define RELEASED_BACKGROUND_COLOR 0x0000
#define RELEASED_BORDER_COLOR 0xffff
#define RELEASED_TEXT_COLOR 0xffff

#define PRESSED_BACKGROUND_COLOR 0xffff
#define PRESSED_BORDER_COLOR 0x0000
#define PRESSED_TEXT_COLOR 0x0000

        void _draw_released_widget() override;

        void _draw_pressed_widget() override;
        void _clear_widget_space() override;

    private:
        String text;
        uint8_t *_text_font = SmallFont;
    };

}