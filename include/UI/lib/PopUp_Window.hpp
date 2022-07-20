#pragma once

#include "MainWindow.hpp"
#include "WindowBase.hpp"

namespace UI
{
    class MainWindow;

    /*
        a pop up window which will be centerd in the display
    */
    class PopUp_Window : public WindowBase
    {
    public:
        PopUp_Window(WindowBase *_parent_window, uint16_t _width, uint16_t _height);
        ~PopUp_Window();

        void show() override;
        void hide() override;
        Widget *get_focused_widget() const override;
        bool request_focus(Widget *_widget) override;

        UTFT *_get_display() const override;
        URTouch *_get_touch() const override;
        MainWindow *_get_main_window() const override;

        void set_border_color(unsigned int _border_color) { this->border_color = _border_color; }
        unsigned int get_border_color() const { return this->border_color; }

    protected:
        unsigned int border_color = VGA_WHITE;

    private:
        WindowBase *parent_window;
    };
} // namespace UI
