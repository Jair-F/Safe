#include "UI/Window.hpp"
#include "UI/Widget.hpp"

// ---------------- Widget ----------------

UI::Widget::Widget(Window *_parent, const position _upper_left_pos, const position _lower_right_pos,
                   UTFT &_display, URTouch &_touch) : focused(false), hidden(true), _parent_window(_parent),
                                                      touch(_touch), display(_display), _upper_left_pos(_upper_left_pos),
                                                      _lower_right_pos(_lower_right_pos)
{
    this->_parent_window->_register_widget(this);
}

void UI::Widget::loop()
{
    if (touch.dataAvailable())
    {
        touch.read();
        /*
            x and y axis are switched on my display
        */
        auto x_pos = 320 - touch.getY();
        auto y_pos = 240 - touch.getX(); // y-axis is rotated on my display and we have 240 pixels
        if (x_pos > display.disp_x_size || y_pos > display.disp_y_size)
        {
            return;
        }
        if (x_pos >= _upper_left_pos.x_pos && x_pos <= _lower_right_pos.x_pos) // x_pos matches
        {
            if (y_pos >= _upper_left_pos.y_pos && y_pos <= _lower_right_pos.y_pos) // y_pos matches to the touch
            {
                // widget was touched
                this->_draw_pressed_widget();
                on_click();
                do
                {
                    touch.read();
                    x_pos = 320 - touch.getY();
                    y_pos = 240 - touch.getX();
                } while (touch.dataAvailable() && (x_pos >= _upper_left_pos.x_pos && x_pos <= _lower_right_pos.x_pos) && (y_pos >= _upper_left_pos.y_pos && y_pos <= _lower_right_pos.y_pos));

                // on_release();
                this->_draw_released_widget();

                // request focus
                this->focused = this->_parent_window->request_focus(this);
            }
        }
        // Serial.println(x_pos);
        // Serial.println(y_pos);
        // Serial.println();
    }
}

void UI::Widget::hide()
{
    this->hidden = true;
    this->_clear_widget_space();
}
void UI::Widget::show()
{
    this->hidden = false;
    this->_draw_released_widget();
}

void UI::Widget::_click()
{
    this->_draw_pressed_widget();
    // this->_parent_window->request_focus(this);
    this->on_click();
}

void UI::Widget::_release()
{
    this->_draw_released_widget();
    this->on_release();
}

void UI::Widget::_reset_click()
{
    this->_draw_released_widget();
}

bool UI::Widget::_check_pos(const position &_pos)
{
    position tmp = _pos;
    position upper_left = this->_upper_left_pos;
    position lower_right = this->_lower_right_pos;
    return (tmp.x_pos >= upper_left.x_pos && tmp.y_pos >= upper_left.y_pos) &&
           (tmp.x_pos <= lower_right.x_pos && tmp.y_pos <= lower_right.y_pos);
}
