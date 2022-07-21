#include "UI/lib/PopUp_Window.hpp"

UI::PopUp_Window::PopUp_Window(WindowBase *_parent_window, uint16_t _width, uint16_t _height) : parent_window(_parent_window), UI::WindowBase({0, 0}, {1, 1})
{

    UTFT *display = this->_get_display();
    URTouch *touch = this->_get_touch();

    this->upper_left.x_pos = display->getDisplayXSize() / 2 - _width / 2;
    this->upper_left.y_pos = display->getDisplayYSize() / 2 - _height / 2;
    this->lower_right.x_pos = this->upper_left.x_pos + _width;
    this->lower_right.y_pos = this->upper_left.y_pos + _height;
}

UI::PopUp_Window::~PopUp_Window()
{
    parent_window->_get_main_window()->set_active_window(parent_window);
}

UI::MainWindow *UI::PopUp_Window::_get_main_window() const
{
    return this->parent_window->_get_main_window();
}
UTFT *UI::PopUp_Window::_get_display() const
{
    return this->parent_window->_get_display();
}
URTouch *UI::PopUp_Window::_get_touch() const
{
    return this->parent_window->_get_touch();
}
bool UI::PopUp_Window::request_focus(Widget *_widget)
{
    this->parent_window->request_focus(_widget);
}
UI::Widget *UI::PopUp_Window::get_focused_widget() const
{
    return this->parent_window->get_focused_widget();
}

void UI::PopUp_Window::show()
{
    UTFT *display = this->_get_display();
    URTouch *touch = this->_get_touch();

    display->setColor(this->background_color);
    display->fillRect(this->upper_left.x_pos, this->upper_left.y_pos, this->lower_right.x_pos, this->lower_right.y_pos);

    display->setColor(this->border_color);
    display->drawRect(this->upper_left.x_pos, this->upper_left.y_pos, this->lower_right.x_pos, this->lower_right.y_pos);

    WindowBase::show();
}

void UI::PopUp_Window::hide()
{
    UTFT *display = this->_get_display();
    URTouch *touch = this->_get_touch();

    // display->setColor(VGA_WHITE);
    // display->fillRect(this->upper_left.x_pos, this->upper_left.y_pos, this->lower_right.x_pos, this->lower_right.y_pos);
    // display->drawRect(this->upper_left.x_pos, this->upper_left.y_pos, this->lower_right.x_pos, this->lower_right.y_pos);

    WindowBase::hide();
}