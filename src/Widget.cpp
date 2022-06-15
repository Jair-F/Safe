#include "UI/Window.hpp"
#include "UI/Widget.hpp"

// ---------------- Widget ----------------

UI::Widget::Widget(position &_upper_left_pos, position &_lower_right_pos,
                   UTFT &_display, URTouch &_touch) : touch(_touch),
                                                      display(_display), _upper_left_pos(_upper_left_pos),
                                                      _lower_right_pos(_lower_right_pos), hidden(false)
{
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

                on_release();
                this->_draw_released_widget();

                // request focus
                this->focused = this->parent_window->request_focus(this);
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
