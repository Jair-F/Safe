#include "UI/UI.hpp"

UI::WindowBase::WindowBase(const position &_upper_left, const position &_lower_right,
                           uint8_t _border_weight) : upper_left(_upper_left), lower_right(_lower_right),
                                                     border_weight(_border_weight), border_color(VGA_WHITE),
                                                     registered_widgets(), active_pop_up_window(nullptr),
                                                     last_focused_widget(nullptr)
{
}

UI::position UI::WindowBase::_calc_absolute_pos(const position &_pos) const
{
    position ret_pos = this->get_content_upper_left();
    ret_pos.x_pos += _pos.x_pos;
    ret_pos.y_pos += _pos.y_pos;

    // while (!Serial.available())
    //     Serial.begin(9600);

    /*
        Serial.println("start calc absolute pos");
        Serial.println(_pos.x_pos);
        Serial.println(_pos.x_pos);
        Serial.println(this->upper_left.x_pos);
        Serial.println(this->upper_left.y_pos);
        Serial.println(ret_pos.x_pos);
        Serial.println(ret_pos.y_pos);
        Serial.println("end calc absolute pos");
    */

    return ret_pos;
}

void UI::WindowBase::_register_widget(Widget *_widget)
{
    this->registered_widgets.push_back(_widget);
}

UI::Widget *UI::WindowBase::handle_touch_clicked(const position &_touch_data)
{
    if (this->active_pop_up_window != nullptr)
    {
        return this->active_pop_up_window->handle_touch_clicked(_touch_data);
    }
    else
    {
        auto widget_iterator = this->registered_widgets.begin();
        while (widget_iterator != this->registered_widgets.end())
        {
            if (!widget_iterator->is_hidden())
            {
                if (widget_iterator->_check_pos(_touch_data))
                {
                    this->last_focused_widget = widget_iterator.data();
                    // touch-data is on the widget - clicked
                    widget_iterator->_touch(_touch_data);
                    return widget_iterator.data(); // return the touched widget
                    break;
                }
            }
            ++widget_iterator;
        }
        return nullptr; // no visible widget matches
    }
}

void UI::WindowBase::handle_touch_released(const position &_touch_data)
{
    if (this->active_pop_up_window != nullptr)
    {
        this->active_pop_up_window->handle_touch_released(_touch_data);
    }
    else
    {
        SinglyListNodeIterator<Widget *> widget_iterator = this->registered_widgets.begin();
        bool touch_data_same_widget = false; /* if touch_data is on the same widget from the click*/
        while (widget_iterator != this->registered_widgets.end())
        {
            if (!widget_iterator->is_hidden())
            {
                // check if we have released on the same widget which we have touched on or if we touched on "whitespace"
                if (widget_iterator->_check_pos(_touch_data) && last_focused_widget == widget_iterator.data())
                {
                    touch_data_same_widget = true;
                    // this->last_focused_widget = widget_on_touch_pos;
                    break;
                }
            }
            ++widget_iterator;
        }

        /*
            in every case we must to reset the old widget(draw released) - whether we have a widget on this pos
            or not
        */
        if (touch_data_same_widget)
        {
            if (!this->last_focused_widget->is_hidden())
            {
                // click was in the widget - not swiped out...
                this->last_focused_widget->_release(_touch_data);
            }
        }
        else
        {
            if (!this->last_focused_widget->is_hidden())
            {
                // widget was clicked but not released on this position
                if (this->last_focused_widget != nullptr)
                    this->last_focused_widget->_reset_touch();
            }
        }
    }
}

void UI::WindowBase::show_pop_up_window(PopUp_Window *_pop_up_window)
{
    if (this->active_pop_up_window != nullptr)
    {
        this->active_pop_up_window->hide();
    }
    this->active_pop_up_window = _pop_up_window;
    this->active_pop_up_window->show();
}

void UI::WindowBase::hide_pop_up_window()
{
    this->active_pop_up_window->hide();
    this->active_pop_up_window = nullptr;

    this->_redraw_window(); // redraw the parent window
}

void UI::WindowBase::show()
{
    // no pop_up window is shown actually
    if (this->active_pop_up_window == nullptr)
    {
        this->_draw_border();
        this->_draw_background();

        auto iterator = this->registered_widgets.begin();
        while (iterator != this->registered_widgets.end())
        {
            iterator->show();
            ++iterator;
        }
    }
    else
    {
        this->active_pop_up_window->show();
    }
}
void UI::WindowBase::hide()
{
    auto iterator = this->registered_widgets.begin();
    while (iterator != this->registered_widgets.end())
    {
        iterator->hide();
        ++iterator;
    }
}

const UI::position &UI::WindowBase::_get_lower_right_pos() const
{
    return this->lower_right;
}
const UI::position &UI::WindowBase::_get_upper_left_pos() const
{
    return this->upper_left;
}
const UI::position &UI::WindowBase::pos() const
{
    return this->upper_left;
}

void UI::WindowBase::_redraw_window()
{
    /*
    UTFT *display = this->_get_display();

    display->setColor(this->background_color);
    display->fillRect(this->upper_left.x_pos, this->upper_left.y_pos, this->lower_right.x_pos, this->lower_right.y_pos);
    */
    this->show();
}

void UI::WindowBase::loop()
{
    if (this->active_pop_up_window == nullptr) // if there is actually no pop up window on the current window - call the widget loop
    {
        auto iterator = this->registered_widgets.begin();
        while (iterator != this->registered_widgets.end())
        {
            if (!(iterator.data()->is_hidden()))
            {
                iterator.data()->loop();
            }
            ++iterator;
        }
    }
    else
    {
        this->active_pop_up_window->loop();
    }
}

UI::position UI::WindowBase::get_content_upper_left() const
{
    if (this->get_draw_border())
    {
        return {this->upper_left.x_pos + this->border_weight,
                this->upper_left.y_pos + this->border_weight};
    }
    else
    {
        return this->upper_left;
    }
}

UI::position UI::WindowBase::get_content_lower_right() const
{
    if (this->get_draw_border())
    {
        return {this->lower_right.x_pos - this->border_weight,
                this->lower_right.y_pos - this->border_weight};
    }
    else
    {
        return this->lower_right;
    }
}

uint16_t UI::WindowBase::get_content_height() const
{
    if (this->get_draw_border())
    {
        return this->lower_right.y_pos - this->upper_left.y_pos - this->border_weight * 2;
    }
    else
    {
        return this->height();
    }
}

uint16_t UI::WindowBase::get_content_width() const
{
    if (this->get_draw_border())
    {
        return this->lower_right.x_pos - this->upper_left.x_pos - this->border_weight * 2;
    }
    else
    {
        return this->width();
    }
}

void UI::WindowBase::_draw_background()
{
    UTFT *display = this->_get_display();
    position content_upper_left = this->get_content_upper_left();
    position content_lower_right = this->get_content_lower_right();

    display->setColor(this->background_color);
    display->fillRect(content_upper_left.x_pos, content_upper_left.y_pos,
                      content_lower_right.x_pos, content_lower_right.y_pos);
}

void UI::WindowBase::_draw_border()
{
    UTFT *display = this->_get_display();

    display->setColor(this->border_color);
    for (uint8_t i = 0; i < this->border_weight; ++i)
    {
        display->drawRect(this->upper_left.x_pos + i, this->upper_left.y_pos + i,
                          this->lower_right.x_pos - i, this->lower_right.y_pos - i);
    }
}
