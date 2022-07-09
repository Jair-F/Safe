#pragma once

#include <Arduino.h>
#include "Widget.hpp"

extern uint8_t SmallFont[];
extern uint8_t BigFont[];

namespace UI
{
    class TextLabel : public Widget
    {
    public:
        enum text_alignment
        {
            AL_CENTER,
            AL_LEFT,
            AL_RIGHT
        };

        /*
        TextLabel(Window *_parent, const position _upper_left_pos, const position _lower_right_pos, String _text = "");
        TextLabel(Window *_parent, const position _upper_left_pos, unsigned int _width, unsigned int _height, String _text = "");
        */

        /*
            adjust the widget height according to the text_length
        */
        TextLabel(Window *_parent, const position _upper_left_pos, uint16_t _width, String _text, uint8_t *_font = SmallFont);

        void set_font(uint8_t *_font) { this->text_font = _font; }
        void set_text(String _text);

        /*
            @param _border true if the text field should have a border, false if not
        */
        void set_border(bool _border) { this->draw_border = _border; }
        bool has_border() { return this->draw_border; }

        void set_text_alignment(text_alignment _text_alignment) { this->text_align = _text_alignment; }
        text_alignment get_text_alignment() { return this->text_align; }

        /*
            the color values are RGB-565 values(16-bit value)
            RGB-565 color picker: https://chrishewett.com/blog/true-rgb565-colour-picker/
        */
        unsigned int background_color;
        unsigned int text_color;
        unsigned int border_color;

    protected:
        void
        _draw_widget();

        // those two are doing the same - call _draw_widget
        void _draw_released_widget() { this->_draw_widget(); }
        void _draw_pressed_widget() { this->_draw_widget(); }

        /*
            calculate the positions and text gaps according to the upper_left_pos, lower_right_pos, the text and the font
            and store the values
        */
        void _calc_widget();

    private:
        String text;
        uint8_t *text_font = SmallFont;
        text_alignment text_align; // text_alignment

        uint8_t text_gap_height; // gap between the lines of text in pixels
        uint8_t text_gap_length; // gap between the begin of the widget and begin of the text and end of the widget and end of the text in pixels
        uint8_t chars_in_line;   // number of characters in one line
        uint8_t text_lines;      // number of text lines

        bool draw_border;
    };

} // namespace UI
