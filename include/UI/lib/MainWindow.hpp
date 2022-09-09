#pragma once

#include <UTFT.h>
#include <URTouch.h>

#include "Window.hpp"
#include "Widget.hpp"

namespace UI
{
    class Window;
    class Widget;
    class WindowBase;
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
        MainWindow(UTFT *_display, URTouch *_touch, position _window_upper_left, position _window_lower_right);
        virtual ~MainWindow() {}

        // String get_input() { return this->keypad.get_buffer(); }

        /*
            removes also the focused widget(sets it to nullptr)
        */
        void set_active_window(WindowBase *_win);
        WindowBase *get_active_window() const;

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
        void send_backspace();
        void send_enter();

        /*
            @param _fall_asleep_timer set the fall_asleep_timer in seconds
        */
        inline void set_fall_asleep_timer(uint16_t _fall_asleep_timer) { this->fall_asleep_timer = _fall_asleep_timer; }
        /*
            @return fall_asleep_timer in seconds
        */
        inline uint16_t get_fall_aspleep_timer() const { return this->fall_asleep_timer; }
        /*
            @return true if in sleep_mode
        */
        inline bool get_in_sleep_mode() const { return this->in_sleep_mode; }

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

        /*
            clear the display and send the display to sleep - but only if the display
            is not waked up already
        */
        void _wake_up();
        /*
            wake the display up and print the current window again - but only if the display
            is not send to sleep already
        */
        void _send_sleep();

    private:
        bool focus_frozen; // one widget has the focus and we are not allowed to change the focus
        Widget *focused_widget;
        WindowBase *active_window;

        UTFT *display;
        URTouch *touch;

        /*
            size of the "normal windows" which are shown on the main_window - the windows grab the size in their window-constructor
        */
        position window_upper_left, window_lower_right;

        /*
            seconds which have to pass until the display falls in a sleep mode -
            the display will turn black until the next touch
        */
        uint16_t fall_asleep_timer = 60;
        /*
            last time point the user interacted with the display/keypad
        */
        uint32_t last_interact_time_pt = 0;
        bool in_sleep_mode = false;
    };
} // namespace UI
