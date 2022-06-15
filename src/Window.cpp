#include "UI/Window.hpp"
#include "UI/Widget.hpp"

bool UI::Window::request_focus(UI::Widget *_widget)
{
    return this->main_window->request_focus(_widget);
}
