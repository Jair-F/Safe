#include "UI/UI.hpp"

#ifdef DEBUG
#include <avr8-stub.h>
#endif

UI::MainWindow::MainWindow(UTFT *_display, URTouch *_touch, position _window_upper_left, position _window_lower_right) : focus_frozen(false), focused_widget(nullptr),
                                                                                                                         active_window(nullptr), display(_display), touch(_touch),
                                                                                                                         window_upper_left(_window_upper_left), window_lower_right(_window_lower_right)
{
    // Serial.println("Mainwindow window shift: ");
    // Serial.print('(');
    // Serial.print(window_upper_left.x_pos);
    // Serial.print(",");
    // Serial.print(window_upper_left.y_pos);
    // Serial.println(')');

    // Serial.print('(');
    // Serial.print(window_lower_right.x_pos);
    // Serial.print(",");
    // Serial.print(window_lower_right.y_pos);
    // Serial.println(')');
}

bool UI::MainWindow::request_focus(UI::Widget *_widget)
{
    if (!this->focus_frozen)
    {
        if (this->focused_widget != nullptr)
            this->focused_widget->_focus_lose(); // call the focus loose of the previous widget
        this->focused_widget = _widget;
        this->focused_widget->update_widget();
        return true;
    }
    return false;
}

UI::WindowBase *UI::MainWindow::get_active_window() const
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

void UI::MainWindow::send_backspace()
{
    if (this->focused_widget != nullptr)
    {
        this->focused_widget->send_backspace();
    }
}

void UI::MainWindow::send_enter()
{
    if (this->focused_widget != nullptr)
    {
        this->focused_widget->send_enter();
    }
}

void UI::MainWindow::set_active_window(WindowBase *_win)
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

    this->unfreeze_focus(); // at every window change the focus will be reseted
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

            // call the on_touch func
            Widget *clicked_widget = this->active_window->handle_touch_clicked(touch_data);

            if (!this->get_focus_frozen()) // if focus isnt frozen do not change the focused_widget
            {
                if (this->focused_widget != clicked_widget) // focused widget change
                {
                    Widget *last_focused_widget = this->focused_widget;
                    this->focused_widget = clicked_widget;
                    if (last_focused_widget != nullptr)
                    {
                        last_focused_widget->_focus_lose(); // call focus_lose for the previous focused widget
                    }
#ifndef DEBUG
                    // Serial.println("focused widget changed...");
#endif
                }
                // this->focused_widget = clicked_widget;
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

    if (this->active_window != nullptr)
    {
        this->active_window->loop();
    }
}

UI::position UI::MainWindow::_read_touch()
{
    position touch_data_ret;
    this->touch->read();
    if (this->display->orient == PORTRAIT)
    {
        touch_data_ret.x_pos = this->display->getDisplayXSize() - this->touch->getX(); // X-axis is mirrored on my display in Portrait
        touch_data_ret.y_pos = this->touch->getY();
    }
    else
    {
        touch_data_ret.x_pos = this->touch->getX();
        touch_data_ret.y_pos = this->display->getDisplayYSize() - this->touch->getY(); // Y-axis is mirrored on my display in Landscape
    }
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
