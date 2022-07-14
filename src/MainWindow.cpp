#include "UI/UI.hpp"

#ifdef DEBUG
#include <avr8-stub.h>
#endif

bool UI::MainWindow::request_focus(UI::Widget *_widget)
{
    if (!this->focus_frozen)
    {
        this->focused_widget = _widget;
        return true;
    }
    return false;
}

UI::Window *UI::MainWindow::get_active_window() const
{
    return active_window;
}

void UI::MainWindow::send_input(char _input_data)
{
    if (this->focused_widget != nullptr)
    {
        this->focused_widget->send_input(_input_data);
    }
}

void UI::MainWindow::set_active_window(Window *_win)
{
    if (_win == nullptr)
    {
        // Serial.println(F("win is nullptr - in set_active _window...!!!!!!!!!!!!!!"));
    }
    else
    {
        // Serial.println("win is not nullptr");
    }
    assert(_win != nullptr);

    // clear space of previous window
    if (this->active_window != nullptr)
    {
        this->active_window->hide();
    }

    this->active_window = _win;
    this->active_window->show();
}

void UI::MainWindow::loop()
{
    if (this->touch->dataAvailable())
    {
        /*
            x and y axis are switched on my display
        */
        position touch_data = this->_read_touch();

#ifdef DEBUG
        String tmp;
        tmp = touch_data.x_pos;
        debug_message(tmp.c_str());
        tmp = touch_data.y_pos;
        debug_message(tmp.c_str());
#endif

        // if the touch-data is in the display
        if (this->_check_in_display(touch_data))
        {

            // call the on_click func
            Widget *clicked_widget = this->active_window->handle_touch_clicked(touch_data);

            if (!this->get_focus_frozen()) // if focus isnt frozen do not change the focused_widget
            {
                if (this->focused_widget != clicked_widget) // focused widget change
                {
                    if (this->focused_widget != nullptr)
                    {
                        this->focused_widget->_focus_lose(); // call focus_lose for the previous focused widget
                    }
#ifndef DEBUG
                    // Serial.println("focused widget changed...");
#endif
                }
                this->focused_widget = clicked_widget;
            }

            if (clicked_widget != nullptr)
            {
                // delay(250);
                while (this->touch->dataAvailable())
                {
                    position tmp = this->_read_touch();
                    if (this->_check_in_display(tmp))
                    {
                        touch_data = tmp;
                    }

#ifndef DEBUG
                    // Serial.print("X_POS: ");
                    // Serial.println(touch_data.x_pos);
                    // Serial.print("Y_POS: ");
                    // Serial.println(touch_data.y_pos);
#endif

                    // delay(100); // bisschen warten, damit die Werte nicht so extrem schwanken...
                }

                //  touch was released at this point...
                this->active_window->handle_touch_released(touch_data);
            }
        }
    }
}

UI::position UI::MainWindow::_read_touch()
{
    position touch_data_ret;
    this->touch->read();
    touch_data_ret.x_pos = 320 - this->touch->getY();
    touch_data_ret.y_pos = 240 - this->touch->getX(); // y-axis is rotated on my display and we have 240 pixels
    return touch_data_ret;
}

bool UI::MainWindow::_check_in_display(const position &_pos) const
{
    return _pos.x_pos <= this->display->getDisplayXSize() ||
           _pos.y_pos <= this->display->getDisplayYSize();
}

const UI::position &UI::MainWindow::_get_window_upper_left() const
{
    return this->window_upper_left;
}

const UI::position &UI::MainWindow::_get_window_lower_right() const
{
    return this->window_lower_right;
}
