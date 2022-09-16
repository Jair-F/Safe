#pragma once

#include <Arduino.h>
#include "Widget.hpp"

extern uint8_t SmallFont[];
extern uint8_t BigFont[];

namespace FGUI
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
        TextLabel(Window *_parent, const position _upper_left, const position _lower_right, String _text = "");
        TextLabel(Window *_parent, const position _upper_left, unsigned int _width, unsigned int _height, String _text = "");
        */

        /*
            adjust the widget height according to the text_length
        */
        TextLabel(WindowBase *_parent, const position _upper_left, uint16_t _width, String _text = "", uint8_t *_font = SmallFont);
        virtual ~TextLabel() {}

        void set_font(uint8_t *_font);
        /*
            \n are interpreted as newlines!!
            no tabs(\t) or other special caracters
        */
        void set_text(String _text);

        /*
            @param _border true if the text field should have a border, false if not
        */
        void set_border(bool _border) { this->draw_border = _border; }
        inline bool has_border() const { return this->draw_border; }

        void set_text_alignment(text_alignment _text_alignment) { this->text_align = _text_alignment; }
        inline text_alignment get_text_alignment() const { return this->text_align; }

        /*
            the color values are RGB-565 values(16-bit value)
            RGB-565 color picker: https://chrishewett.com/blog/true-rgb565-colour-picker/
        */
        unsigned int released_text_color = VGA_WHITE;

    protected:
        void _draw_widget() override;

        // those two are doing the same - call _draw_widget
        void _draw_content(Widget::w_status _st) override { this->_draw_widget(); }

        /*
            calculate the positions and text gaps according to the upper_left_pos, lower_right_pos,
            the text, the font and the newlines defined by the user
            and store the values
        */
        void _calc_widget();

    private:
        String text;
        uint8_t *text_font = SmallFont;
        text_alignment text_align; // text_alignment

        uint8_t text_gap_height;   // gap between the lines of text in pixels
        uint8_t text_gap_length;   // gap between the begin of the widget and begin of the text and end of the widget and end of the text in pixels
        uint8_t max_chars_in_line; // number of characters in one line
        uint8_t text_lines;        // number of text lines

        bool draw_border;
    };

} // namespace FGUI
