#include "UI/UI.hpp"

/*
UI::TextLabel::TextLabel(Window *_parent, const position _upper_left_pos,
                         const position _lower_right_pos, String _text = "") : Widget(_parent, _upper_left_pos, _lower_right_pos),
                                                                               text(_text)
{
}

UI::TextLabel::TextLabel(Window *_parent, const position _upper_left_pos,
                         unsigned int _width, unsigned int _height, String _text = "") : Widget(_parent, _upper_left_pos, _width, _height),
                                                                                         text(_text)
{
}
*/

UI::TextLabel::TextLabel(Window *_parent, const position _upper_left_pos, uint16_t _width,
                         String _text, uint8_t *_text_font) : Widget(_parent, _upper_left_pos, {_width, 1}),
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
    // fill the widget_space with the background_color
    this->display->setColor(background_color);
    this->display->fillRect(this->_upper_left_pos.x_pos, this->_upper_left_pos.y_pos,
                            this->_lower_right_pos.x_pos, this->_lower_right_pos.y_pos);

    this->display->setFont(text_font);
    this->display->setBackColor(this->background_color);
    this->display->setColor(this->text_color);

    this->_calc_widget();

    position text_starting_pos = {this->_upper_left_pos.x_pos, this->_upper_left_pos.y_pos + text_gap_height};

    for (unsigned short i = 0; i < this->text_lines; ++i)
    {
        String line_to_print = this->text.substring(i * this->chars_in_line, (i + 1) * this->chars_in_line);

        // adjusting the x_pos of the text_starting_pos to move it that it is aligned correctly
        switch (this->text_align)
        {
        case text_alignment::AL_LEFT:
        {
            text_starting_pos.x_pos = this->_upper_left_pos.x_pos + this->text_gap_length;
            break;
        }
        case text_alignment::AL_CENTER:
        {
            // remaining space at the sides if the text length and the gap at the side is considered (!! sum of the space of both sides !!)
            uint16_t remaining_space = ((this->width() - 2 * text_gap_length) - line_to_print.length() * this->display->getFontXsize());
            text_starting_pos.x_pos = this->_upper_left_pos.x_pos + this->text_gap_length + remaining_space / 2;
            break;
        }
        case text_alignment::AL_RIGHT:
        {
            // remaining space at the sides if the text length and the gap at the side is considered (!! sum of the space of both sides !!)
            uint16_t remaining_space = ((this->width() - 2 * text_gap_length) - line_to_print.length() * this->display->getFontXsize());
            text_starting_pos.x_pos = this->_upper_left_pos.x_pos + this->text_gap_length + remaining_space;
            break;
        }
        }
        this->display->print(line_to_print, text_starting_pos.x_pos, text_starting_pos.y_pos);

        // pushing the next text line down by the size of the text_font and the vertical gap between the lines
        text_starting_pos.y_pos = text_starting_pos.y_pos + this->display->getFontYsize() + text_gap_height;
    }

    if (this->has_border())
    { // border of the widget
        this->display->setColor(this->border_color);
        this->display->drawHLine(this->_upper_left_pos.x_pos, this->_upper_left_pos.y_pos, this->width());
        this->display->drawHLine(this->_upper_left_pos.x_pos, this->_lower_right_pos.y_pos, this->width());
        this->display->drawVLine(this->_upper_left_pos.x_pos, this->_upper_left_pos.y_pos, this->height());
        this->display->drawVLine(this->_lower_right_pos.x_pos, this->_upper_left_pos.y_pos, this->height());
    }
}

void UI::TextLabel::set_text(String _text)
{
    this->text = _text;
    this->_clear_widget_space();

    this->_calc_widget();

    this->_draw_widget();
}

void UI::TextLabel::_calc_widget()
{
    this->display->setFont(text_font);
    uint16_t font_height = this->display->getFontYsize();
    uint16_t font_width = this->display->getFontXsize();

    this->chars_in_line = (this->width() - 2 * this->text_gap_length) / font_width;

    this->text_lines = this->text.length() / this->chars_in_line; // columns of text...
    if ((this->text.length() % this->chars_in_line) != 0)
        ++this->text_lines; // int cuts the values after the comma - if we have for example 5.5 lines we add to the 5 lines a sixt - which is not filled fully...

    uint16_t widget_height = this->text_lines * font_height + text_gap_height * this->text_lines + text_gap_height;
    this->set_size(this->width(), widget_height);
}