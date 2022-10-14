#include "FGUI/FGUI.hpp"
#include <URTouch.h>
#include <UTFT.h>

FGUI::Window::Window(FGUI::MainWindow *main_window) : WindowBase(main_window->_get_window_upper_left(), main_window->_get_window_lower_right()),
                                                      _main_window(main_window)
{
    // this->upper_left = _main_window->_get_window_upper_left();
    // this->lower_right = _main_window->_get_window_lower_right();
}

FGUI::Window::Window(FGUI::WindowBase *_parent_window) : WindowBase(_parent_window->_get_main_window()->_get_window_upper_left(), _parent_window->_get_main_window()->_get_window_lower_right(), _parent_window),
                                                         _main_window(_parent_window->_get_main_window())
{
    // this->upper_left = _main_window->_get_window_upper_left();
    // this->lower_right = _main_window->_get_window_lower_right();
}

bool FGUI::Window::request_focus(FGUI::Widget *_widget)
{
    return this->_main_window->request_focus(_widget);
}

UTFT *FGUI::Window::_get_display() const
{
    return this->_main_window->_get_display();
}

URTouch *FGUI::Window::_get_touch() const
{
    return this->_main_window->_get_touch();
}

FGUI::Widget *FGUI::Window::get_focused_widget() const
{
    return this->_main_window->get_focused_widget();
}
