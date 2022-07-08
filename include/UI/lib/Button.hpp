#pragma once

#include "Touch_Widget.hpp"
#include <UTFT.h>
#include <URTouch.h>

extern uint8_t SmallFont[];
extern uint8_t BigFont[];

namespace UI
{
    template <typename CALL_OBJECT_TYPE>
    class Button : public Touch_Widget<CALL_OBJECT_TYPE>
    {
    public:
        Button(Window *parent, const position _upper_left_pos_pos, const position _lower_right_pos,
               CALL_OBJECT_TYPE *_call_object) : Touch_Widget<CALL_OBJECT_TYPE>(parent, parent->_calc_absolute_pos(_upper_left_pos_pos),
                                                                                parent->_calc_absolute_pos(_lower_right_pos), _call_object)
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

    protected:
/*
    the color values are RGB-565 values(16-bit value)
    RGB-565 color picker: https://chrishewett.com/blog/true-rgb565-colour-picker/
*/
#define RELEASED_BACKGROUND_COLOR VGA_BLACK
#define RELEASED_BORDER_COLOR VGA_WHITE
#define RELEASED_TEXT_COLOR VGA_WHITE

#define PRESSED_BACKGROUND_COLOR VGA_WHITE
#define PRESSED_BORDER_COLOR VGA_WHITE
#define PRESSED_TEXT_COLOR VGA_BLACK

        virtual void _draw_released_widget() override;
        virtual void _draw_pressed_widget() override;
        virtual void _draw_widget() override;

    private:
        String text;
        uint8_t *_text_font = SmallFont;
    };

}

// ------------- template implementation -------------

template <typename CALL_OBJECT_TYPE>
void UI::Button<CALL_OBJECT_TYPE>::_draw_pressed_widget()
{
    this->_clear_widget_space();

    // draw background-color
    this->display->setColor(PRESSED_BACKGROUND_COLOR);
    this->display->fillRect(this->_upper_left_pos.x_pos, this->_upper_left_pos.y_pos,
                            this->_lower_right_pos.x_pos, this->_lower_right_pos.y_pos);

    // draw the border
    this->display->setColor(PRESSED_BORDER_COLOR);
    this->display->drawRect(this->_upper_left_pos.x_pos, this->_upper_left_pos.y_pos,
                            this->_lower_right_pos.x_pos, this->_lower_right_pos.y_pos);

    uint8_t font_height = this->display->getFontYsize();
    uint8_t font_width = this->display->getFontXsize();

    // print the button-label
    this->display->setFont(this->_text_font);
    this->display->setColor(PRESSED_TEXT_COLOR);
    this->display->setBackColor(PRESSED_BACKGROUND_COLOR);
    this->display->setFont(this->_text_font);
    this->display->print(this->text, this->_upper_left_pos.x_pos + (this->width() / 2) - (font_width * text.length() / 2), this->_lower_right_pos.y_pos - (this->height() / 2) - font_height / 2);
}

template <typename CALL_OBJECT_TYPE>
void UI::Button<CALL_OBJECT_TYPE>::_draw_released_widget()
{

    this->_clear_widget_space();

    // draw background-color
    this->display->setColor(RELEASED_BACKGROUND_COLOR);
    this->display->fillRect(this->_upper_left_pos.x_pos, this->_upper_left_pos.y_pos,
                            this->_lower_right_pos.x_pos, this->_lower_right_pos.y_pos);

    // draw the border
    this->display->setColor(RELEASED_BORDER_COLOR);
    this->display->drawRect(this->_upper_left_pos.x_pos, this->_upper_left_pos.y_pos,
                            this->_lower_right_pos.x_pos, this->_lower_right_pos.y_pos);

    uint8_t font_height = this->display->getFontYsize();
    uint8_t font_width = this->display->getFontXsize();

    // print the button-label
    this->display->setFont(this->_text_font);
    this->display->setBackColor(RELEASED_BACKGROUND_COLOR);
    this->display->setColor(RELEASED_TEXT_COLOR);
    this->display->setFont(this->_text_font);
    this->display->print(this->text, this->_upper_left_pos.x_pos + (this->width() / 2) - (font_width * text.length() / 2), this->_lower_right_pos.y_pos - (this->height() / 2) - font_height / 2);
}

template <typename CALL_OBJECT_TYPE>
void UI::Button<CALL_OBJECT_TYPE>::_draw_widget()
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