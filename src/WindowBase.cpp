#include "UI/UI.hpp"

UI::position UI::WindowBase::_calc_absolute_pos(const position &_pos) const
{
    position ret_pos;
    ret_pos.x_pos = this->upper_left.x_pos + _pos.x_pos;
    ret_pos.y_pos = this->upper_left.y_pos + _pos.y_pos;

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

void UI::WindowBase::handle_touch_released(const position &_touch_data)
{
    SinglyListNodeIterator<Widget *> widget_iterator = this->registered_widgets.begin();
    bool touch_data_same_widget = false; /* if touch_data is on the same widget from the click*/
    while (widget_iterator != this->registered_widgets.end())
    {
        if (!widget_iterator->is_hidden())
        {
            // check if we have a widget with this position or if we clicked on "whitespace"
            if (widget_iterator->_check_pos(_touch_data))
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
