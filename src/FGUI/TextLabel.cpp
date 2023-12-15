#include "FGUI/FGUI.hpp"

FGUI::TextLabel::TextLabel(WindowBase *_parent,
                           const position _upper_left,
                           uint16_t _width,
                           String _text, uint8_t *_text_font,
                           uint8_t _border_weight,
                           uint8_t _border_to_text_gap) : Widget(_parent,
                                                                 _upper_left,
                                                                 _width - _border_weight * 2 - _border_to_text_gap * 2,
                                                                 _upper_left.y_pos + _border_weight * 2 + _border_to_text_gap * 2 + 1,
                                                                 _border_weight, _border_to_text_gap),
                                                          text(_text), text_font(_text_font),
                                                          text_align(text_alignment::AL_LEFT), line_spacing(0),
                                                          max_chars_in_line(0), text_lines(0),
                                                          fixed_size(false),
                                                          lines_to_print(0),
                                                          need_recalculate(true)
{
    this->display->setFont(text_font);
    uint8_t font_height = this->display->getFontYsize();
    uint8_t font_width = this->display->getFontXsize();

    // set the gap between the begin of the widget and the begin of the text
    this->set_content_border_gap((font_height > font_width ? font_height : font_width) * 0.5); // take the bigger one of both (height or width)

    // is called in set_content_border_gap
    // this->_calc_widget();
}

FGUI::TextLabel::TextLabel(WindowBase *_parent,
                           const position _upper_left,
                           uint16_t _width,
                           uint16_t _height,
                           String _text, uint8_t *_text_font,
                           uint8_t _border_weight,
                           uint8_t _border_to_text_gap) : Widget(_parent,
                                                                 _upper_left,
                                                                 _width - _border_weight * 2 - _border_to_text_gap * 2,
                                                                 _height - _border_weight * 2 - _border_to_text_gap * 2,
                                                                 _border_weight, _border_to_text_gap),
                                                          text(_text), text_font(_text_font),
                                                          text_align(text_alignment::AL_LEFT), line_spacing(0),
                                                          max_chars_in_line(0), text_lines(0),
                                                          fixed_size(true),
                                                          lines_to_print(0),
                                                          need_recalculate(true)
{
}

void FGUI::TextLabel::_draw_widget()
{
    if (!this->is_hidden())
    {
        if (this->need_recalculate)
        {
            // clear the space before recalculating cause the size will change if its not fixed size
            this->_clear_widget_space();
            this->_calc_widget();
            this->need_recalculate = false;
        }
        if (this->get_draw_border())
            this->_draw_border(Widget::w_status::S_RELEASED);
        // else
        // this->_clear_border_space();
        this->_draw_background(Widget::w_status::S_RELEASED);

        this->display->setFont(text_font);
        this->display->setBackColor(this->released_background_color);
        this->display->setColor(this->released_text_color);

        position text_starting_pos = this->get_content_upper_left();
        uint8_t printed_line_counter = 0;

        // String line_to_print = this->text.substring(i * this->max_chars_in_line, (i + 1) * this->max_chars_in_line);
        String line_to_print = "";
        for (uint16_t str_index = 0; str_index < this->text.length(); ++str_index)
        {
            if (this->text[str_index] != '\n' && str_index < this->text.length())
            {
                line_to_print += this->text[str_index];
            }
            if (this->text[str_index] == '\n' || str_index >= this->text.length() - 1) // if newline or end of text
            {
                // remaining space at the sides if the text length and the gap at the side is considered (!! sum of the space of both sides !!)
                uint16_t remaining_space = (this->get_content_width() - line_to_print.length() * this->display->getFontXsize());

                // adjusting the x_pos of the text_starting_pos to move it that it is aligned correctly
                switch (this->text_align)
                {
                case text_alignment::AL_LEFT:
                {
                    // if left oriented the default is good
                    break;
                }
                case text_alignment::AL_CENTER:
                {
                    text_starting_pos.x_pos = this->get_content_upper_left().x_pos + remaining_space / 2;
                    break;
                }
                case text_alignment::AL_RIGHT:
                {
                    text_starting_pos.x_pos = this->get_content_upper_left().x_pos + remaining_space;
                    break;
                }
                }
                this->display->print(line_to_print, text_starting_pos.x_pos, text_starting_pos.y_pos);

                // pushing the next text line down by the size of the text_font and the vertical gap between the lines
                text_starting_pos.y_pos = text_starting_pos.y_pos + this->display->getFontYsize() + line_spacing;

                line_to_print = "";
                ++printed_line_counter;
                if (this->fixed_size == true && printed_line_counter >= lines_to_print) // if we print all lines we can print in fixed_size
                    break;                                                              // hop out and stop printing to not exceed the widget size
            }
        }
    }
}

void FGUI::TextLabel::set_text(String _text)
{
    this->text = _text;
    this->need_recalculate = true;
}

void FGUI::TextLabel::set_font(uint8_t *_font)
{
    this->text_font = _font;
    this->need_recalculate = true;
}

void FGUI::TextLabel::set_line_spacing(uint8_t _line_spacing)
{
    this->line_spacing = _line_spacing;
    this->need_recalculate = true;
}

void FGUI::TextLabel::_calc_widget()
{
    this->display->setFont(text_font);
    uint8_t font_height = this->display->getFontYsize();
    uint8_t font_width = this->display->getFontXsize();

    this->line_spacing = 0.5 * this->display->getFontYsize();
    this->max_chars_in_line = this->get_content_width() / font_width; // cut the digits after the comma - dont print half chars...

    String tmp = "";                  // the text of the label with calculated newlines
    uint16_t present_newline_pos = 0; // relative position in the current line - counts how long the current line is
    this->text_lines = 1;             // counts the text lines

    // adding newlines to text for wrapping the text to the display size - considers user defined newlines
    for (uint16_t i = 0; i < this->text.length(); ++i)
    {
        if (this->text[i] == '\n')
        {
            tmp += this->text[i];
            present_newline_pos = 0;
            ++text_lines;
        }
        else if (present_newline_pos >= max_chars_in_line &&
                 i < this->text.length() - 1 && // not the last char of the text
                 this->text[i + 1] != '\n')
        {
            tmp += '\n';
            tmp += this->text[i];
            present_newline_pos = 0;
            ++text_lines;
        }
        else
        {
            tmp += this->text[i];
            ++present_newline_pos;
        }
    }
    swap(String, this->text, tmp);

    // adjust only the size of the textlabel if its not fixed size
    if (this->fixed_size == false)
    {
        uint16_t widget_height = this->text_lines * font_height +
                                 this->line_spacing * (this->text_lines - 1) + // between the lines
                                 this->get_content_border_gap() * 2 +          // top and bottom
                                 this->get_border_weight() * 2;

        this->set_size_shift_lower_right(this->width(), widget_height); // adjust size of the widget
    }
    else // the num of lines to print
    {
        this->lines_to_print = this->get_content_height() /
                               (font_height + this->line_spacing); // height of the line + spacing to the next line

        Serial.print("lines to print: ");
        Serial.println(this->lines_to_print);
    }
}

void FGUI::TextLabel::set_border_weight(uint8_t _border_weight)
{
    FGUI::Widget::set_border_weight(_border_weight);
    this->need_recalculate = true;
}

void FGUI::TextLabel::set_content_border_gap(uint8_t _border_content_gap)
{
    FGUI::Widget::set_content_border_gap(_border_content_gap);
    this->need_recalculate = true;
}
