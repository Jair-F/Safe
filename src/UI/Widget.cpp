#include "UI/UI.hpp"

// ---------------- Widget ----------------

UI::Widget::Widget(WindowBase *_parent, const position &_upper_left, const position &_lower_right,
                   uint8_t _border_weight) : hidden(true), border_weight(_border_weight), parent_window(_parent),
                                             upper_left(parent_window->_calc_absolute_pos(_upper_left)), lower_right(parent_window->_calc_absolute_pos(_lower_right))
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

    if (this->border_weight > 0)
    {
        this->set_draw_border(true);
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

    this->display = this->parent_window->_get_display();
    this->touch = this->parent_window->_get_touch();
    this->parent_window->_register_widget(this);
}

UI::Widget::Widget(WindowBase *_parent, const position &_upper_left, uint16_t _width, uint16_t _height,
                   uint8_t _border_weight) : hidden(true), border_weight(_border_weight), parent_window(_parent),
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

    if (this->border_weight > 0)
    {
        this->set_draw_border(true);
    }

    this->display = this->parent_window->_get_display();
    this->touch = this->parent_window->_get_touch();
    this->parent_window->_register_widget(this);
}

UI::Widget::~Widget()
{
    this->_get_parent_window()->_unregister_widget(this);
}

void UI::Widget::hide()
{
    if (!this->hidden)
    {
        this->_clear_widget_space();
        this->hidden = true;
    }
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
    if (!this->is_hidden())
    {
        // clear the space where the widget is(background-color)
        this->display->setColor(this->parent_window->get_background_color());
        this->display->fillRect(this->upper_left.x_pos, this->upper_left.y_pos,
                                this->lower_right.x_pos, this->lower_right.y_pos);
    }
}

void UI::Widget::set_size(uint16_t _width, uint16_t _height)
{
    this->_clear_widget_space();
    this->lower_right.x_pos = this->upper_left.x_pos + _width;
    this->lower_right.y_pos = this->upper_left.y_pos + _height;
    this->update_widget();
}

void UI::Widget::update_widget()
{
    if (!this->is_hidden())
    {
        this->_draw_widget();
    }
}

void UI::Widget::_draw_widget()
{
    if (this->get_draw_border())
    {
        this->_draw_released_border();
    }
    this->_draw_released_background();
    this->_draw_released_content();
}

void UI::Widget::_draw_released_background()
{
    position background_upper_left = this->get_content_upper_left();
    position background_lower_right = this->get_content_lower_right();

    this->display->setColor(this->released_background_color);
    this->display->fillRect(background_upper_left.x_pos, background_upper_left.y_pos, background_lower_right.x_pos, background_lower_right.y_pos);
}

void UI::Widget::_draw_released_border()
{
    this->display->setColor(this->released_border_color);
    for (uint8_t i = 0; i < this->border_weight; ++i)
    {
        this->display->drawRect(this->upper_left.x_pos + i, this->upper_left.y_pos + i, this->lower_right.x_pos - i, this->lower_right.y_pos - i);
    }
    // this->display->drawLine()
}

UI::position UI::Widget::get_content_upper_left() const
{
    if (this->get_draw_border())
        return {this->upper_left.x_pos + this->border_weight, this->upper_left.y_pos + this->border_weight};
    else
        return this->upper_left;
}

UI::position UI::Widget::get_content_lower_right() const
{
    if (this->get_draw_border())
        return {this->lower_right.x_pos - this->border_weight, this->lower_right.y_pos - this->border_weight};
    else
        return this->lower_right;
}

uint16_t UI::Widget::get_content_height() const
{
    if (this->get_draw_border())
        return this->lower_right.y_pos - this->upper_left.y_pos - this->border_weight * 2;
    else
        return this->height();
}

uint16_t UI::Widget::get_content_width() const
{
    if (this->get_draw_border())
        return this->lower_right.x_pos - this->upper_left.x_pos - this->border_weight * 2;
    else
        return this->width();
}

void UI::Widget::set_border_weight(uint8_t _border_weight)
{
    if (_border_weight * 2 > this->width())
        this->border_weight = this->width() / 2;
    else if (_border_weight * 2 > this->height())
        this->border_weight = this->height() / 2;
    else
        this->border_weight = _border_weight;
}
