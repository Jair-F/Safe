#include "UI/UI.hpp"

// ---------------- Widget ----------------

UI::Widget::Widget(WindowBase *_parent, const position &_upper_left, const position &_lower_right) : hidden(true), _parent_window(_parent),
                                                                                                     upper_left(_parent_window->_calc_absolute_pos(_upper_left)), lower_right(_parent_window->_calc_absolute_pos(_lower_right))
{
    // adjusting the positions in case they not set correctly - if the upper_left is for example the lower_right position
    decltype(upper_left.x_pos) tmp;
    if (this->lower_right.x_pos < this->upper_left.x_pos)
    {
        tmp = this->lower_right.x_pos;
        this->lower_right.x_pos = this->upper_left.x_pos;
        this->upper_left.x_pos = tmp;
    }
    if (this->lower_right.y_pos < this->upper_left.y_pos)
    {
        tmp = this->lower_right.y_pos;
        this->lower_right.y_pos = this->upper_left.y_pos;
        this->upper_left.y_pos = tmp;
    }

    /*
    // check that the positions are in the window - if not adjust them to the maximum of the window
    if (this->lower_right.y_pos > this->_parent_window->_get_lower_right_pos().y_pos - 1)
    {
        // one pixel smaller than the window that it can be touched...
        this->lower_right.y_pos = this->_parent_window->_get_lower_right_pos().y_pos - 1; //
    }
    if (this->lower_right.x_pos > this->_parent_window->_get_lower_right_pos().x_pos - 1)
    {
        // one pixel smaller than the window that it can be touched...
        this->lower_right.x_pos = this->_parent_window->_get_lower_right_pos().x_pos - 1;
    }
    */

    this->display = this->_parent_window->_get_display();
    this->touch = this->_parent_window->_get_touch();
    this->_parent_window->_register_widget(this);
}

UI::Widget::Widget(WindowBase *_parent, const position &_upper_left, uint16_t _width, uint16_t _height) : hidden(true), _parent_window(_parent),
                                                                                                          upper_left(_upper_left)
{
    this->lower_right.x_pos = this->upper_left.x_pos + _width;
    this->lower_right.y_pos = this->upper_left.y_pos + _height;

    // adjusting the positions in case they not set correctly - if the upper_left is for example the lower_right position
    decltype(upper_left.x_pos) tmp;
    if (this->lower_right.x_pos < this->upper_left.x_pos)
    {
        tmp = this->lower_right.x_pos;
        this->lower_right.x_pos = this->upper_left.x_pos;
        this->upper_left.x_pos = tmp;
    }
    if (this->lower_right.y_pos < this->upper_left.y_pos)
    {
        tmp = this->lower_right.y_pos;
        this->lower_right.y_pos = this->upper_left.y_pos;
        this->upper_left.y_pos = tmp;
    }

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
    return (_pos.x_pos >= this->upper_left.x_pos && _pos.y_pos >= this->upper_left.y_pos) &&
           (_pos.x_pos <= this->lower_right.x_pos && _pos.y_pos <= this->lower_right.y_pos);
}

void UI::Widget::_clear_widget_space()
{
    // clear the space where the widget is(background-color)
    this->display->setColor(this->_parent_window->get_background_color());
    this->display->fillRect(this->upper_left.x_pos, this->upper_left.y_pos,
                            this->lower_right.x_pos, this->lower_right.y_pos);
    // this->display->drawRoundRect(asd, )
}

void UI::Widget::set_size(uint16_t _width, uint16_t _height)
{
    this->lower_right.x_pos = this->upper_left.x_pos + _width;
    this->lower_right.y_pos = this->upper_left.y_pos + _height;
}