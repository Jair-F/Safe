#include "UI/MainWindow.hpp"

bool UI::MainWindow::request_focus(UI::Widget *_widget)
{
    if (!this->focus_frozen)
    {
        this->focused_widget = _widget;
        return true;
    }
    return false;
}

UI::Window *UI::MainWindow::get_active_window()
{
    return active_window;
}

void UI::MainWindow::_handle_keypad_input(char _input_data)
{
    if (this->focused_widget != nullptr)
    {
        this->focused_widget->send_input(_input_data);
    }
}
