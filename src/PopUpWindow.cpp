#include "UI/lib/PopUp_Window.hpp"

UI::PopUp_Window::PopUp_Window(WindowBase *_parent_window, uint16_t _width, uint16_t _height,
                               uint16_t _close_button_size = 22) : UI::WindowBase({(_parent_window->_get_display()->getDisplayXSize() / 2 - _width / 2), (_parent_window->_get_display()->getDisplayYSize() / 2 - _height / 2)},
                                                                                  {(_parent_window->_get_display()->getDisplayXSize() / 2 + _width / 2), (_parent_window->_get_display()->getDisplayYSize() / 2 + _height / 2)}),
                                                                   parent_window(_parent_window), close_button_size(_close_button_size),
                                                                   close_button(this, {this->width() - this->close_button_size, 0}, close_button_size, this)
{
    this->close_button.on_release = &this->_exit_pop_up_window;

    // UTFT *display = this->_get_display();
    // URTouch *touch = this->_get_touch();
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

    // draw the background color
    display->setColor(this->background_color);
    display->fillRect(this->upper_left.x_pos, this->upper_left.y_pos, this->lower_right.x_pos, this->lower_right.y_pos);

    // draw the border
    display->setColor(this->border_color);
    display->drawRect(this->upper_left.x_pos - 1, this->upper_left.y_pos - 1, this->lower_right.x_pos + 1, this->lower_right.y_pos + 1);

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

void UI::PopUp_Window::_exit_pop_up_window(Touch_Widget<PopUp_Window> *_widget)
{
    this->parent_window->hide_pop_up_window();
}
