#pragma once
#include "Window.hpp"

namespace UI
{
    class Window;
    /*
        the display-window. everytime one window can be displayed on the main-window
    */
    class MainWindow
    {
    public:
        MainWindow() {}
        virtual ~MainWindow() {}

        // String get_input() { return this->keypad.get_buffer(); }

        /*
            removes also the focused widget(sets it to nullptr)
        */
        void set_active_window(Window *_win);
        Window *get_active_window();

        void freeze_focus() { this->focus_frozen = true; }
        void unfreeze_focus() { this->focus_frozen = false; }
        bool get_focus_frozen() { return this->focus_frozen; }

        /*
            @param _widget request focus for _widget to get keyboard_input
            @return true if the request was accepted else false
        */
        bool request_focus(Widget *_widget);

    protected:
        void _handle_keypad_input(char _input_data);

    private:
        bool focus_frozen; // one widget has the focus and we are not allowed to change the focus
        Widget *focused_widget;
        Window *active_window;
    };
} // namespace UI