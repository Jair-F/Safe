#pragma once

#include <UTFT.h>
#include <URTouch.h>

#include "Window.hpp"
#include "Widget.hpp"

namespace UI
{
    class Window;
    class Widget;
    /*
        the display-window. everytime one window can be displayed on the main-window
    */
    class MainWindow
    {
    public:
        MainWindow(UTFT *_display, URTouch *_touch) : focused_widget(nullptr), active_window(nullptr), display(_display), touch(_touch) {}
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

        void loop();

    protected:
        void _handle_keypad_input(char _input_data);

        /*
            check if the given position is in the display-size
            @return true if it is in the display
        */
        bool _check_in_display(const position &_pos);

        /*
            read the touch and convert it to the position corresponding to the display
            !!! use this->touch->dataAvailable() to get if data is available
        */
        position _read_touch();

    private:
        bool focus_frozen; // one widget has the focus and we are not allowed to change the focus
        Widget *focused_widget;
        Window *active_window;

        UTFT *display;
        URTouch *touch;
    };
} // namespace UI
