#include "FGUI/FGUI.hpp"
#include <URTouch.h>
#include <UTFT.h>

UI::Window::Window(UI::MainWindow *main_window) : WindowBase(main_window->_get_window_upper_left(), main_window->_get_window_lower_right()),
                                                  _main_window(main_window)
{
    // this->upper_left = _main_window->_get_window_upper_left();
    // this->lower_right = _main_window->_get_window_lower_right();
}

bool UI::Window::request_focus(UI::Widget *_widget)
{
    return this->_main_window->request_focus(_widget);
}

UTFT *UI::Window::_get_display() const
{
    return this->_main_window->_get_display();
}

URTouch *UI::Window::_get_touch() const
{
    return this->_main_window->_get_touch();
}

UI::Widget *UI::Window::get_focused_widget() const
{
    return this->_main_window->get_focused_widget();
}
