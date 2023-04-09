#include "FGUI/FGUI.hpp"

// ---------------- Widget ----------------

FGUI::Widget::Widget(WindowBase *_parent, const position &_upper_left, const position &_lower_right,
                     uint8_t _border_weight, uint8_t _border_content_gap) : parent_window(_parent),
                                                                            upper_left(_parent->_calc_absolute_pos(_upper_left)),
                                                                            lower_right(_parent->_calc_absolute_pos(_lower_right)),
                                                                            border_weight(_border_weight),
                                                                            border_content_gap(_border_content_gap)
{
    this->set_draw_border(true);

    this->display = this->parent_window->_get_display();
    this->touch = this->parent_window->_get_touch();
    this->parent_window->_register_widget(this);
}

FGUI::Widget::Widget(WindowBase *_parent,
                     const position &_upper_left,
                     uint16_t _content_width, uint16_t _content_height,
                     uint8_t _border_weight,
                     uint8_t _border_content_gap) : parent_window(_parent),
                                                    upper_left(_parent->_calc_absolute_pos(_upper_left)),
                                                    border_weight(_border_weight),
                                                    border_content_gap(_border_content_gap)
{
    this->lower_right.x_pos = this->upper_left.x_pos + _content_width - 1 + // -1 because the first pixel of upper_left is included
                              (uint16_t)_border_weight * 2 + (uint16_t)border_content_gap * 2;
    this->lower_right.y_pos = this->upper_left.y_pos + _content_height - 1 + // -1 because the first pixel of upper_left is included
                              (uint16_t)_border_weight * 2 + (uint16_t)border_content_gap * 2;

    this->set_draw_border(true);

    this->display = this->parent_window->_get_display();
    this->touch = this->parent_window->_get_touch();
    this->parent_window->_register_widget(this);
}
FGUI::Widget::Widget(WindowBase *_parent, uint16_t _content_width, uint16_t _content_height, const position &_lower_right,
                     uint8_t _border_weight, uint8_t _border_content_gap) : parent_window(_parent),
                                                                            lower_right(_parent->_calc_absolute_pos(_lower_right)),
                                                                            border_weight(_border_weight),
                                                                            border_content_gap(_border_content_gap)
{
    this->upper_left.x_pos = this->lower_right.x_pos - border_weight * 2 -
                             border_content_gap * 2 - _content_width - 1; // -1 because the first pixel of lower_right is included
    this->upper_left.y_pos = this->lower_right.y_pos - border_weight * 2 -
                             border_content_gap * 2 - _content_height - 1; // -1 because the first pixel of lower_right is included

    this->set_draw_border(true);

    this->display = this->parent_window->_get_display();
    this->touch = this->parent_window->_get_touch();
    this->parent_window->_register_widget(this);
}

FGUI::Widget::~Widget()
{
    this->_get_parent_window()->_unregister_widget(this);
}

void FGUI::Widget::hide()
{
    if (!this->hidden)
    {
        this->_clear_widget_space();
        this->hidden = true;
    }
}
void FGUI::Widget::show()
{
    this->hidden = false;
    this->_draw_widget();
}
bool FGUI::Widget::_check_pos(const position &_pos) const
{
    return (_pos.x_pos >= this->upper_left.x_pos && _pos.y_pos >= this->upper_left.y_pos) &&
           (_pos.x_pos <= this->lower_right.x_pos && _pos.y_pos <= this->lower_right.y_pos);
}

void FGUI::Widget::_clear_widget_space()
{
    if (!this->is_hidden())
    {
        // clear the space where the widget is with parent window background-color
        this->display->setColor(this->parent_window->get_background_color());
        this->display->fillRect(this->upper_left.x_pos, this->upper_left.y_pos,
                                this->lower_right.x_pos, this->lower_right.y_pos);
    }
}

void FGUI::Widget::_clear_border_space()
{
    this->display->setColor(this->_get_parent_window()->get_background_color());
    for (uint8_t i = 0; i < this->border_weight; ++i)
    {
        this->display->drawRect(this->upper_left.x_pos + i, this->upper_left.y_pos + i,
                                this->lower_right.x_pos - i, this->lower_right.y_pos - i);
    }
}

void FGUI::Widget::clear_content_space()
{
    this->display->setColor(this->released_background_color);
    this->display->fillRect(this->get_content_upper_left().x_pos,
                            this->get_content_upper_left().y_pos,
                            this->get_content_lower_right().x_pos,
                            this->get_content_lower_right().y_pos);
}

void FGUI::Widget::set_size_shift_equally(uint16_t _width, uint16_t _height)
{
    int16_t height_diff = _height - this->height(); // pixels to shift in sum
    int16_t width_diff = _width - this->width();    // pixels to shift in sum

    if (width_diff > 0)
    {
        if (width_diff % 2 == 0) // even?
        {
            this->upper_left.x_pos -= width_diff / 2;
        }
        else
        {
            // if not even shift it one pixel more to left
            this->upper_left.x_pos -= width_diff / 2 + 1;
        }
        this->lower_right.x_pos = this->upper_left.x_pos + width_diff / 2;
    }

    if (height_diff > 0)
    {
        if (height_diff % 2 == 0) // even?
        {
            this->upper_left.y_pos -= height_diff / 2;
        }
        else
        {
            // if not even shift it one pixel more to up
            this->upper_left.y_pos -= height_diff / 2 + 1;
        }
        this->lower_right.y_pos = this->upper_left.y_pos + height_diff / 2;
    }
}

void FGUI::Widget::set_size_shift_lower_right(uint16_t _width, uint16_t _height)
{
    this->lower_right.x_pos = this->upper_left.x_pos + _width - 1;
    this->lower_right.y_pos = this->upper_left.y_pos + _height - 1;
}

void FGUI::Widget::set_size_shift_upper_left(uint16_t _width, uint16_t _height)
{
    this->upper_left.x_pos = this->lower_right.x_pos - _width - 1;
    this->upper_left.y_pos = this->lower_right.y_pos - _height - 1;
}

void FGUI::Widget::draw()
{
    this->_draw_widget();
}

void FGUI::Widget::clear()
{
    this->_clear_widget_space();
}

void FGUI::Widget::_draw_widget()
{
    if (!this->is_hidden())
    {
        if (this->get_draw_border())
            this->_draw_border(w_status::S_RELEASED);
        else
            this->_clear_border_space();
        this->_draw_background(w_status::S_RELEASED);
        this->_draw_content(w_status::S_RELEASED);
    }
}

void FGUI::Widget::_draw_background(w_status _st)
{
    // we ignore the status here - not a touchable widget
    position background_upper_left = {this->upper_left.x_pos + this->border_weight,
                                      this->upper_left.y_pos + this->border_weight};
    position background_lower_right = {this->lower_right.x_pos - this->border_weight,
                                       this->lower_right.y_pos - this->border_weight};

    this->display->setColor(this->released_background_color);
    this->display->fillRect(background_upper_left.x_pos, background_upper_left.y_pos,
                            background_lower_right.x_pos, background_lower_right.y_pos);
}

void FGUI::Widget::_draw_border(w_status _st)
{
    // we ignore the status here - not a touchable widget
    this->display->setColor(this->released_border_color);
    for (uint8_t i = 0; i < this->border_weight; ++i)
    {
        this->display->drawRect(this->upper_left.x_pos + i, this->upper_left.y_pos + i,
                                this->lower_right.x_pos - i, this->lower_right.y_pos - i);
    }
    // this->display->drawLine()
}

FGUI::position FGUI::Widget::get_content_upper_left() const
{
    return {this->upper_left.x_pos + this->border_weight + this->border_content_gap,
            this->upper_left.y_pos + this->border_weight + this->border_content_gap};
}

FGUI::position FGUI::Widget::get_content_lower_right() const
{
    return {this->lower_right.x_pos - this->border_weight - this->border_content_gap,
            this->lower_right.y_pos - this->border_weight - this->border_content_gap};
}

uint16_t FGUI::Widget::get_content_height() const
{
    return this->height() - this->border_weight * 2 - this->border_content_gap * 2;
}

uint16_t FGUI::Widget::get_content_width() const
{
    return this->width() - this->border_weight * 2 - this->border_content_gap * 2;
}

void FGUI::Widget::set_border_weight(uint8_t _border_weight)
{
    // int8_t border_weight_diff = _border_weight - this->border_weight;
    // this->upper_left.x_pos -= border_weight_diff;
    // this->upper_left.y_pos -= border_weight_diff;
    // this->lower_right.x_pos += border_weight_diff;
    // this->lower_right.y_pos += border_weight_diff;

    this->border_weight = _border_weight;
}

void FGUI::Widget::set_content_border_gap(uint8_t _border_content_gap)
{
    // int8_t border_content_gap_diff = _border_content_gap - this->border_content_gap;
    // this->upper_left.x_pos -= border_content_gap_diff;
    // this->upper_left.y_pos -= border_content_gap_diff;
    // this->lower_right.x_pos += border_content_gap_diff;
    // this->lower_right.y_pos += border_content_gap_diff;

    this->border_content_gap = _border_content_gap;
}
