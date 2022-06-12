#pragma once

#include "Widget.hpp"
#include <UTFT.h>
#include <URTouch.h>

extern uint8_t SmallFont[];
extern uint8_t BigFont[];

class Button : public Widget
{
public:
    Button(position &__upper_left_pos_pos, position &_lower_right_pos, UTFT &_display, URTouch &_touch) : Widget(__upper_left_pos_pos, _lower_right_pos, _display, _touch)
    {
        this->_draw_released_widget();
    }

    void setFont(uint8_t *_font) { this->text_font = _font; }

    void setText(String _text)
    {
        text = _text;
        this->_draw_released_widget();
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
#define PRESSED_BORDER_COLOR 0xffff
#define PRESSED_TEXT_COLOR 0x0000

    void _draw_released_widget() override
    {
        // clear the space where the button is(background-color)
        this->display.setColor(RELEASED_BACKGROUND_COLOR);
        this->display.fillRoundRect(_upper_left_pos.x_pos, _upper_left_pos.y_pos,
                                    _lower_right_pos.x_pos, _lower_right_pos.y_pos);

        // draw the border
        this->display.setColor(RELEASED_BORDER_COLOR);
        this->display.drawRoundRect(this->_upper_left_pos.x_pos, this->_upper_left_pos.y_pos,
                                    this->_lower_right_pos.x_pos, this->_lower_right_pos.y_pos);

        unsigned short font_height = this->display.getFontYsize();
        unsigned short font_width = this->display.getFontXsize();

        // print the button-label
        this->display.setFont(this->text_font);
        this->display.setBackColor(RELEASED_BACKGROUND_COLOR);
        this->display.setColor(RELEASED_TEXT_COLOR);
        this->display.print(this->text, this->_upper_left_pos.x_pos + (this->width() / 2) - (font_width * text.length() / 2), this->_lower_right_pos.y_pos - (this->height() / 2) - font_height / 2);
    }

    void _draw_pressed_widget() override
    {
        // clear teh space where the button is(background-color)
        this->display.setColor(PRESSED_BACKGROUND_COLOR);
        this->display.fillRoundRect(_upper_left_pos.x_pos, _upper_left_pos.y_pos,
                                    _lower_right_pos.x_pos, _lower_right_pos.y_pos);

        // draw the border
        this->display.setColor(PRESSED_BORDER_COLOR);
        this->display.drawRoundRect(this->_upper_left_pos.x_pos, this->_upper_left_pos.y_pos,
                                    this->_lower_right_pos.x_pos, this->_lower_right_pos.y_pos);

        unsigned short font_height = this->display.getFontYsize();
        unsigned short font_width = this->display.getFontXsize();

        // print the button-label
        this->display.setFont(this->text_font);
        this->display.setColor(PRESSED_TEXT_COLOR);
        this->display.setBackColor(PRESSED_BACKGROUND_COLOR);
        this->display.print(this->text, this->_upper_left_pos.x_pos + (this->width() / 2) - (font_width * text.length() / 2), this->_lower_right_pos.y_pos - (this->height() / 2) - font_height / 2);
    }

private:
    String text;
    uint8_t *text_font = SmallFont;
};