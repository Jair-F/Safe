#include "UI/Button.hpp"

void UI::Button::_clear_widget_space()
{
    // clear teh space where the button is(background-color)
    this->display.setColor(RELEASED_BACKGROUND_COLOR);
    this->display.fillRect(_upper_left_pos.x_pos, _upper_left_pos.y_pos,
                           _lower_right_pos.x_pos, _lower_right_pos.y_pos);
}

void UI::Button::_draw_pressed_widget()
{
    this->_clear_widget_space();

    // draw background-color
    this->display.setColor(PRESSED_BACKGROUND_COLOR);
    this->display.fillRect(_upper_left_pos.x_pos, _upper_left_pos.y_pos,
                           _lower_right_pos.x_pos, _lower_right_pos.y_pos);

    // draw the border
    this->display.setColor(PRESSED_BORDER_COLOR);
    this->display.drawRect(this->_upper_left_pos.x_pos, this->_upper_left_pos.y_pos,
                           this->_lower_right_pos.x_pos, this->_lower_right_pos.y_pos);

    uint8_t font_height = this->display.getFontYsize();
    uint8_t font_width = this->display.getFontXsize();

    // print the button-label
    this->display.setFont(this->_text_font);
    this->display.setColor(PRESSED_TEXT_COLOR);
    this->display.setBackColor(PRESSED_BACKGROUND_COLOR);
    this->display.print(this->text, this->_upper_left_pos.x_pos + (this->width() / 2) - (font_width * text.length() / 2), this->_lower_right_pos.y_pos - (this->height() / 2) - font_height / 2);
}

void UI::Button::_draw_released_widget()
{

    this->_clear_widget_space();

    // draw background-color
    this->display.setColor(RELEASED_BACKGROUND_COLOR);
    this->display.fillRect(_upper_left_pos.x_pos, _upper_left_pos.y_pos,
                           _lower_right_pos.x_pos, _lower_right_pos.y_pos);

    // draw the border
    this->display.setColor(RELEASED_BORDER_COLOR);
    this->display.drawRect(this->_upper_left_pos.x_pos, this->_upper_left_pos.y_pos,
                           this->_lower_right_pos.x_pos, this->_lower_right_pos.y_pos);

    uint8_t font_height = this->display.getFontYsize();
    uint8_t font_width = this->display.getFontXsize();

    // print the button-label
    this->display.setFont(this->_text_font);
    this->display.setBackColor(RELEASED_BACKGROUND_COLOR);
    this->display.setColor(RELEASED_TEXT_COLOR);
    this->display.print(this->text, this->_upper_left_pos.x_pos + (this->width() / 2) - (font_width * text.length() / 2), this->_lower_right_pos.y_pos - (this->height() / 2) - font_height / 2);
}