#include "UI/UI.hpp"

// ---------------- Widget ----------------

UI::Widget::Widget(Window *_parent, const position &_upper_left_pos, const position &_lower_right_pos) : hidden(true), _parent_window(_parent),
                                                                                                         _upper_left_pos(_parent_window->_calc_absolute_pos(_upper_left_pos)), _lower_right_pos(_parent_window->_calc_absolute_pos(_lower_right_pos))
{
    // adjusting the positions in case they not set correctly - if the _upper_left is for example the _lower_right position
    decltype(_upper_left_pos.x_pos) tmp;
    if (this->_lower_right_pos.x_pos < this->_upper_left_pos.x_pos)
    {
        tmp = this->_lower_right_pos.x_pos;
        this->_lower_right_pos.x_pos = this->_upper_left_pos.x_pos;
        this->_upper_left_pos.x_pos = tmp;
    }
    if (this->_lower_right_pos.y_pos < this->_upper_left_pos.y_pos)
    {
        tmp = this->_lower_right_pos.y_pos;
        this->_lower_right_pos.y_pos = this->_upper_left_pos.y_pos;
        this->_upper_left_pos.y_pos = tmp;
    }

    this->display = this->_parent_window->_get_display();
    this->touch = this->_parent_window->_get_touch();
    this->_parent_window->_register_widget(this);
}

UI::Widget::Widget(Window *_parent, const position &_upper_left_pos, uint16_t _width, uint16_t _height) : hidden(true), _parent_window(_parent),
                                                                                                          _upper_left_pos(_upper_left_pos)
{
    this->_lower_right_pos.x_pos = this->_upper_left_pos.x_pos + _width;
    this->_lower_right_pos.y_pos = this->_upper_left_pos.y_pos + _height;

    this->display = this->_parent_window->_get_display();
    this->touch = this->_parent_window->_get_touch();
    this->_parent_window->_register_widget(this);
}

void UI::Widget::hide()
{
    this->hidden = true;
    this->_clear_widget_space();
}
void UI::Widget::show()
{
    this->hidden = false;
    this->_draw_widget();
}
bool UI::Widget::_check_pos(const position &_pos) const
{
    return (_pos.x_pos >= this->_upper_left_pos.x_pos && _pos.y_pos >= this->_upper_left_pos.y_pos) &&
           (_pos.x_pos <= this->_lower_right_pos.x_pos && _pos.y_pos <= this->_lower_right_pos.y_pos);
}

void UI::Widget::_clear_widget_space()
{
    // clear the space where the widget is(background-color)
    this->display->setColor(this->_parent_window->get_background_color());
    this->display->fillRect(this->_upper_left_pos.x_pos, this->_upper_left_pos.y_pos,
                            this->_lower_right_pos.x_pos, this->_lower_right_pos.y_pos);
    // this->display->drawRoundRect(asd, )
}

void UI::Widget::set_size(uint16_t _width, uint16_t _height)
{
    this->_lower_right_pos.x_pos = this->_upper_left_pos.x_pos + _width;
    this->_lower_right_pos.y_pos = this->_upper_left_pos.y_pos + _height;
}