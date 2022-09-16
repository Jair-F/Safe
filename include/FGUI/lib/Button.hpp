#pragma once

#include "ButtonBase.hpp"
#include <UTFT.h>
#include <URTouch.h>

extern uint8_t SmallFont[];
extern uint8_t BigFont[];

namespace FGUI
{
    template <typename CALL_OBJECT_TYPE>
    class Button : public ButtonBase<CALL_OBJECT_TYPE>
    {
    public:
        Button(WindowBase *parent, position _upper_left, position _lower_right,
               CALL_OBJECT_TYPE *_call_object) : ButtonBase<CALL_OBJECT_TYPE>(parent, _upper_left, _lower_right, _call_object) {}
        Button(WindowBase *parent, position _upper_left, uint16_t _width, uint16_t _height,
               CALL_OBJECT_TYPE *_call_object) : ButtonBase<CALL_OBJECT_TYPE>(parent, _upper_left,
                                                                              {_upper_left.x_pos + _width, _upper_left.y_pos + _height},
                                                                              _call_object) {}

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
        }

        /*
            the color values are RGB-565 values(16-bit value)
            RGB-565 color picker: https://chrishewett.com/blog/true-rgb565-colour-picker/
        */
        unsigned int released_text_color = VGA_WHITE;
        unsigned int touched_text_color = VGA_BLACK;
        unsigned int disabled_text_color = VGA_GRAY;

    protected:
        void _draw_content(Widget::w_status _st) override;

    private:
        String text;
        uint8_t *_text_font = SmallFont;
    };

}

// ------------- template implementation -------------

template <typename CALL_OBJECT_TYPE>
void FGUI::Button<CALL_OBJECT_TYPE>::_draw_content(Widget::w_status _st)
{
    switch (_st)
    {
    case Widget::w_status::S_DISABLED:
    {
        this->display->setColor(this->disabled_text_color);
        this->display->setBackColor(this->disabled_background_color);
        break;
    }
    case Widget::w_status::S_TOUCHED:
    {
        this->display->setColor(this->touched_text_color);
        this->display->setBackColor(this->touched_background_color);
        break;
    }
    case Widget::w_status::S_RELEASED:
    {
        this->display->setColor(this->released_text_color);
        this->display->setBackColor(this->released_background_color);
        break;
    }
    default:
        break;
    }

    this->display->setFont(this->_text_font);
    uint8_t font_height = this->display->getFontYsize();
    uint8_t font_width = this->display->getFontXsize();

    // print the button-label

    this->display->print(this->text, this->upper_left.x_pos + (this->width() / 2) - (font_width * text.length() / 2),
                         this->lower_right.y_pos - (this->height() / 2) - font_height / 2);
}
