#include "FGUI/FGUI.hpp"

FGUI::PopUp_Window::PopUp_Window(WindowBase *_parent_window, uint16_t _width, uint16_t _height, uint8_t _border_weight, bool _show_close_button,
                                 uint16_t _close_button_size) : FGUI::WindowBase({static_cast<uint16_t>((_parent_window->_get_display()->getDisplayXSize() / 2 - _width / 2)),
                                                                                  static_cast<uint16_t>((_parent_window->_get_display()->getDisplayYSize() / 2 - _height / 2))},

                                                                                 {static_cast<uint16_t>((_parent_window->_get_display()->getDisplayXSize() / 2 + _width / 2)),
                                                                                  static_cast<uint16_t>((_parent_window->_get_display()->getDisplayYSize() / 2 + _height / 2))},
                                                                                 _parent_window, _border_weight),
                                                                close_button(this, {static_cast<uint16_t>(this->get_content_width() - _close_button_size), 0},
                                                                             _close_button_size, this)
{
    if (!_show_close_button)
        this->close_button.hide();
    this->close_button.on_release = &PopUp_Window::_exit_pop_up_window;
}

FGUI::PopUp_Window::~PopUp_Window()
{
    MainWindow *main_window = this->_get_main_window();
    if (main_window->get_active_window() == this)
    {
        main_window->set_active_window(this->_get_parent_window());
    }
}

FGUI::MainWindow *FGUI::PopUp_Window::_get_main_window() const
{
    return this->_get_parent_window()->_get_main_window();
}
UTFT *FGUI::PopUp_Window::_get_display() const
{
    return this->_get_parent_window()->_get_display();
}
URTouch *FGUI::PopUp_Window::_get_touch() const
{
    return this->_get_parent_window()->_get_touch();
}
bool FGUI::PopUp_Window::request_focus(Widget *_widget)
{
    this->_get_parent_window()->request_focus(_widget);
}
FGUI::Widget *FGUI::PopUp_Window::get_focused_widget() const
{
    return this->_get_parent_window()->get_focused_widget();
}

void FGUI::PopUp_Window::hide_close_button()
{
    this->close_button.hide();
}
void FGUI::PopUp_Window::show_close_button()
{
    this->close_button.show();
}
bool FGUI::PopUp_Window::is_close_button_hidden()
{
    return this->close_button.is_hidden();
}

/*
void FGUI::PopUp_Window::show()
{
    // UTFT *display = this->_get_display();
    //  URTouch *touch = this->_get_touch();

    // draw the background color
    // display->setColor(this->background_color);
    // display->fillRect(this->upper_left.x_pos, this->upper_left.y_pos, this->lower_right.x_pos, this->lower_right.y_pos);

    // draw the border
    // display->setColor(this->border_color);
    // display->drawRect(this->upper_left.x_pos - 1, this->upper_left.y_pos - 1, this->lower_right.x_pos + 1, this->lower_right.y_pos + 1);

    WindowBase::show();
}

void FGUI::PopUp_Window::hide()
{
    // UTFT *display = this->_get_display();
    // URTouch *touch = this->_get_touch();

    // display->setColor(VGA_WHITE);
    // display->fillRect(this->upper_left.x_pos, this->upper_left.y_pos, this->lower_right.x_pos, this->lower_right.y_pos);
    // display->drawRect(this->upper_left.x_pos, this->upper_left.y_pos, this->lower_right.x_pos, this->lower_right.y_pos);

    WindowBase::hide();
}
*/

void FGUI::PopUp_Window::_exit_pop_up_window(Touch_Widget<PopUp_Window> *_widget)
{
    this->_get_parent_window()->hide_pop_up_window();
}
