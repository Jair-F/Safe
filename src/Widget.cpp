#include "UI/UI.hpp"

// ---------------- Widget ----------------

UI::Widget::Widget(Window *_parent, const position _upper_left_pos, const position _lower_right_pos) : hidden(true), _parent_window(_parent),
                                                                                                       _upper_left_pos(_upper_left_pos), _lower_right_pos(_lower_right_pos)
{
    this->display = this->_parent_window->_get_display();
    this->touch = this->_parent_window->_get_touch();
    this->_parent_window->_register_widget(this);
}

UI::Widget::Widget(Window *_parent, const position &_upper_left_pos, unsigned int _width, unsigned int _height) : hidden(true), _parent_window(_parent),
                                                                                                                  _upper_left_pos(_upper_left_pos), _lower_right_pos(_lower_right_pos)
{
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
bool UI::Widget::_check_pos(const position &_pos)
{
    return (_pos.x_pos >= this->_upper_left_pos.x_pos && _pos.y_pos >= this->_upper_left_pos.y_pos) &&
           (_pos.x_pos <= this->_lower_right_pos.x_pos && _pos.y_pos <= this->_lower_right_pos.y_pos);
}
