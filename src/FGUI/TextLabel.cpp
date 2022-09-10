#include "FGUI/FGUI.hpp"

/*
UI::TextLabel::TextLabel(Window *_parent, const position _upper_left,
                         const position _lower_right, String _text = "") : Widget(_parent, _upper_left, _lower_right),
                                                                               text(_text)
{
}

UI::TextLabel::TextLabel(Window *_parent, const position _upper_left,
                         unsigned int _width, unsigned int _height, String _text = "") : Widget(_parent, _upper_left, _width, _height),
                                                                                         text(_text)
{
}
*/

UI::TextLabel::TextLabel(Window *_parent, const position _upper_left, uint16_t _width,
                         String _text, uint8_t *_text_font) : Widget(_parent, _upper_left, {_width, _upper_left.y_pos + 1}),
                                                              background_color(VGA_BLACK), text_color(VGA_WHITE), border_color(VGA_WHITE),
                                                              text(_text), text_font(_text_font), text_align(text_alignment::AL_LEFT), draw_border(false)
{
    this->display->setFont(text_font);
    uint8_t font_height = this->display->getFontYsize();
    uint8_t font_width = this->display->getFontXsize();

    this->text_gap_height = 0.5 * font_height; // 1/2 of the height - in pixels
    this->text_gap_length = font_width * 0.5;

    this->_calc_widget();
}

void UI::TextLabel::_draw_widget()
{
    if (!this->is_hidden())
    {
        if (this->get_draw_border())
        {
            this->_draw_released_border();
        }
        this->_draw_released_background();

        this->display->setFont(text_font);
        this->display->setBackColor(this->background_color);
        this->display->setColor(this->text_color);

        // this->_calc_widget();

        // print text to display
        for (uint16_t i = 0; i < this->text.length(); ++i)
        {
            if (this->text[i] != '\n')
            {
                // Serial.print(this->text[i]);
            }
            else
            {
                // Serial.print("\\n");
            }
        }

        position text_starting_pos = {this->upper_left.x_pos, this->upper_left.y_pos + text_gap_height};

        // String line_to_print = this->text.substring(i * this->max_chars_in_line, (i + 1) * this->max_chars_in_line);
        String line_to_print = "";
        for (uint16_t str_index = 0; str_index < this->text.length(); ++str_index)
        {
            if (this->text[str_index] != '\n' && str_index < this->text.length())
            {
                line_to_print += this->text[str_index];
            }
            if (this->text[str_index] == '\n' || str_index == this->text.length() - 1)
            {
                // Serial.print("Printing on display: ");
                // Serial.println(line_to_print);
                //  adjusting the x_pos of the text_starting_pos to move it that it is aligned correctly
                switch (this->text_align)
                {
                case text_alignment::AL_LEFT:
                {
                    text_starting_pos.x_pos = this->upper_left.x_pos + this->text_gap_length;
                    break;
                }
                case text_alignment::AL_CENTER:
                {
                    // remaining space at the sides if the text length and the gap at the side is considered (!! sum of the space of both sides !!)
                    uint16_t remaining_space = ((this->width() - 2 * text_gap_length) - line_to_print.length() * this->display->getFontXsize());
                    text_starting_pos.x_pos = this->upper_left.x_pos + this->text_gap_length + remaining_space / 2;
                    break;
                }
                case text_alignment::AL_RIGHT:
                {
                    // remaining space at the sides if the text length and the gap at the side is considered (!! sum of the space of both sides !!)
                    uint16_t remaining_space = ((this->width() - 2 * text_gap_length) - line_to_print.length() * this->display->getFontXsize());
                    text_starting_pos.x_pos = this->upper_left.x_pos + this->text_gap_length + remaining_space;
                    break;
                }
                }
                this->display->print(line_to_print, text_starting_pos.x_pos, text_starting_pos.y_pos);

                // pushing the next text line down by the size of the text_font and the vertical gap between the lines
                text_starting_pos.y_pos = text_starting_pos.y_pos + this->display->getFontYsize() + text_gap_height;

                line_to_print = "";
            }
        }
    }
}

void UI::TextLabel::set_text(String _text)
{
    this->text = _text;
    this->_clear_widget_space();

    this->_calc_widget();

    this->_draw_widget();
}

void UI::TextLabel::set_font(uint8_t *_font)
{
    this->text_font = _font;
    this->_calc_widget();
    this->_draw_widget();
}

void UI::TextLabel::_calc_widget()
{
    this->display->setFont(text_font);
    uint8_t font_height = this->display->getFontYsize();
    uint8_t font_width = this->display->getFontXsize();

    this->max_chars_in_line = (this->width() - 2 * this->text_gap_length) / font_width; // cut the digits after the comma - dont print half chars...

    String tmp = ""; // the text of the label with calculated newlines
    uint16_t last_newline_pos = 0;
    for (uint16_t i = 0; i < this->text.length(); ++i)
    {
        if (this->text[i] == '\n')
        {
            // Serial.print("Text-Label Debug-Print-newlines: ");
            // Serial.println(this->text.substring(last_newline_pos, i));

            tmp += this->text.substring(last_newline_pos, i);
            tmp += '\n';
            last_newline_pos = i + 1; // +1 - otherwise the '\n' would be inserted
        }
        else if (i - last_newline_pos >= this->max_chars_in_line && this->text[i + 1] != '\n')
        {
            // only add a newline if there is no newline defined by the user

            // Serial.print("Text-Label Debug-Print-newlines: ");
            // Serial.println(this->text.substring(last_newline_pos + 1, i + 1));
            tmp += this->text.substring(last_newline_pos, i + 1);
            tmp += '\n';
            last_newline_pos = i + 1;
        }
    }

    // if we didnt added the last part of the text or the text is only i char long
    if (last_newline_pos < this->text.length() - 1 || this->text.length() == 1)
    {
        tmp += this->text.substring(last_newline_pos, this->text.length());
        // Serial.print("Text-Label Debug-Print-newlines: ");
        // Serial.println(this->text.substring(last_newline_pos, this->text.length()));
    }
    swap(String, this->text, tmp);

    // count the newlines
    this->text_lines = 0;
    for (uint16_t i = 0; i < this->text.length(); ++i)
    {
        if (this->text[i] == '\n')
            ++this->text_lines;
    }
    if (this->text[this->text.length() - 1] != '\n')
        ++this->text_lines;

    // this->text_lines = this->text.length() / this->max_chars_in_line; // columns of text...

    // if ((this->text.length() % this->max_chars_in_line) != 0)
    //     ++this->text_lines; // int cuts the values after the comma - if we have for example 5.5 lines we add to the 5 lines a sixt - which is not filled fully...

    uint16_t widget_height = this->text_lines * font_height + text_gap_height * this->text_lines + text_gap_height;
    this->set_size(this->width(), widget_height);
}