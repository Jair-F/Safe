#pragma once

#include "ButtonBase.hpp"
#include <UTFT.h>
#include <URTouch.h>

extern uint8_t SmallFont[];
extern uint8_t BigFont[];

namespace UI
{
    template <typename CALL_OBJECT_TYPE>
    class Button : public ButtonBase<CALL_OBJECT_TYPE>
    {
    public:
        Button(WindowBase *parent, position _upper_left, position _lower_right,
               CALL_OBJECT_TYPE *_call_object) : ButtonBase<CALL_OBJECT_TYPE>(parent, _upper_left, _lower_right, _call_object) {}

        virtual ~Button() {}

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
            this->_draw_widget();
        }

        /*
            the color values are RGB-565 values(16-bit value)
            RGB-565 color picker: https://chrishewett.com/blog/true-rgb565-colour-picker/
        */
        unsigned int released_text_color = VGA_WHITE;

        unsigned int pressed_text_color = VGA_BLACK;

    protected:
        virtual void _draw_released_widget() override;
        virtual void _draw_pressed_widget() override;

    private:
        String text;
        uint8_t *_text_font = SmallFont;
    };

}

// ------------- template implementation -------------

template <typename CALL_OBJECT_TYPE>
void UI::Button<CALL_OBJECT_TYPE>::_draw_pressed_widget()
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

    this->display->setFont(this->_text_font);
    uint8_t font_height = this->display->getFontYsize();
    uint8_t font_width = this->display->getFontXsize();

    // print the button-label
    this->display->setColor(this->pressed_text_color);
    this->display->setBackColor(this->pressed_background_color);
    this->display->print(this->text, this->upper_left.x_pos + (this->width() / 2) - (font_width * text.length() / 2), this->lower_right.y_pos - (this->height() / 2) - font_height / 2);
}

template <typename CALL_OBJECT_TYPE>
void UI::Button<CALL_OBJECT_TYPE>::_draw_released_widget()
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

    this->display->setFont(this->_text_font);
    uint8_t font_height = this->display->getFontYsize();
    uint8_t font_width = this->display->getFontXsize();

    // print the button-label
    this->display->setBackColor(this->released_background_color);
    this->display->setColor(this->released_text_color);
    this->display->print(this->text,
                         this->upper_left.x_pos + (this->width() / 2) - (font_width * text.length() / 2),
                         this->lower_right.y_pos - (this->height() / 2) - font_height / 2);
}
