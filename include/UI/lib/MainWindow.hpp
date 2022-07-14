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
        /*
            @param window_upper_left size - the upper left position of the "normal windows" which are shown on the main_window
            @param window_lower_right size - the lower right position of the "normal windows" which are shown on the main_window
        */
        MainWindow(UTFT *_display, URTouch *_touch, position window_upper_left, position window_lower_right) : focused_widget(nullptr),
                                                                                                               active_window(nullptr), display(_display), touch(_touch) {}
        virtual ~MainWindow() {}

        // String get_input() { return this->keypad.get_buffer(); }

        /*
            removes also the focused widget(sets it to nullptr)
        */
        void set_active_window(Window *_win);
        Window *get_active_window() const;

        Widget *get_focused_widget() const { return this->focused_widget; }

        void freeze_focus() { this->focus_frozen = true; }
        void unfreeze_focus() { this->focus_frozen = false; }
        bool get_focus_frozen() { return this->focus_frozen; }

        /*
            @param _widget request focus for _widget to get keyboard_input
            @return true if the request was accepted else false
        */
        bool request_focus(Widget *_widget);

        /*
            send the _input_data to the focused widget
        */
        void send_input(char _input_data);

        /*
            need to be called continuously in the main loop of the application
            checks for touch data and handles the windows and widgets
        */
        void loop();

        /*
            for the windows to get their size from the main_window
        */
        const position &_get_window_upper_left() const;

        /*
            for the windows to get their size from the main_window
        */
        const position &_get_window_lower_right() const;

        inline UTFT *_get_display() const { return this->display; }
        inline URTouch *_get_touch() const { return this->touch; }

    protected:
        /*
            check if the given position is in the display-size
            @return true if it is in the display
        */
        inline bool _check_in_display(const position &_pos) const;

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

        /*
            size of the "normal windows" which are shown on the main_window - the windows grab the size in their window-constructor
        */
        position _window_lower_right, _window_upper_left;
    };
} // namespace UI
