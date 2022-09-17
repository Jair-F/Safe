#pragma once

#include <UTFT.h>
#include <URTouch.h>

#include "Window.hpp"
#include "Widget.hpp"

namespace FGUI
{
    class Window;
    class Widget;
    class WindowBase;
    /*!
        @details The MainWindow is the virtual connection to the display. It handels the actual window which
        is shown on the display, handles touches and redirects it to the actual window, sends keybord
        inputs to the focused widgets and handles other similar tasks.
        There can be only one instance of MainWindow per display

        @note the loop function of this MainWindow instance need to be called in a loop from the main programm
        from there all the interactions with the touch display will be handled and the call_back functions
        of the widget are called.
    */
    class MainWindow
    {
    public:
        /*!
            @param _display pointer to the UTFT display class instance
            @param _touch pointer to the touch class instance for the display
            @param window_upper_left size - the upper left position of the "normal windows" which are shown on the main_window
            @param window_lower_right size - the lower right position of the "normal windows" which are shown on the main_window

            @details the normal windows can be sized smaller than the real display is - this are the _window_upper_left and _window_lower_right
        */
        MainWindow(UTFT *_display, URTouch *_touch, position _window_upper_left, position _window_lower_right);

        /*!
            @param _display pointer to the UTFT display class instance
            @param _touch pointer to the touch class instance for the display
        */
        MainWindow(UTFT *_display, URTouch *_touch);
        virtual ~MainWindow() {}

        // String get_input() { return this->keypad.get_buffer(); }

        /*!
            @param _win pointer to the window which should be set as active window

            @details the active window is the window which is currently shown on the display
            this active_window will get the inputs which the user sends to the MainWindow
            there can be displayed only one window at the same time on the display!!
        */
        void set_active_window(WindowBase *_win);
        /*!
            @return pointer to the active window
        */
        WindowBase *get_active_window() const;

        /*!
            @return pointer to the widget which has the focus - the last touched widget
        */
        Widget *get_focused_widget() const { return this->focused_widget; }

        /*!
            makes that the focus cant be changed until unfreeze_focus() is called
            widget can be touched while focus is frozen but they will not get the sent input
            because the dont gain the focus despite they are touched
        */
        void freeze_focus() { this->focus_frozen = true; }
        /*!
           unfreezes the focus that widget can gain again the focus
        */
        void unfreeze_focus() { this->focus_frozen = false; }
        /*!
            @return true if the focus is frozen
        */
        bool is_focus_frozen() { return this->focus_frozen; }

        /*!
            @param _widget request focus for _widget to get keyboard_input
            @return true if the request was accepted else false
        */
        bool request_focus(Widget *_widget);

        /*!
            @param _input_data the input_data which should be send to the focused widget
            @details sends the _input_data to the focused widget
        */
        void send_input(char _input_data);
        /*!
            send the backspace command to the focused widget
        */
        void send_backspace();
        /*!
            send enter command to the focused widget
        */
        void send_enter();

        /*!
            @param _fall_asleep_timer set the fall_asleep_timer in seconds

            @details the display will turn to black after this num of seconds have been passed and will
            wake up(show the active_window) if a touch was recognized or any input_data was sent.

            @note !! important - the internal sleep counter is based on the Arduino millis() - which overflows
            after 50 days. - this means after 50 days the display will fall asleep function will behave
            undefined until a touch or an input_sent was recognized(which sets this timer again)
        */
        inline void set_fall_asleep_timer(uint16_t _fall_asleep_timer) { this->fall_asleep_timer = _fall_asleep_timer; }
        /*!
            @return fall_asleep_timer in seconds
        */
        inline uint16_t get_fall_aspleep_timer() const { return this->fall_asleep_timer; }
        /*!
            @return true if in sleep_mode - if the display will fall asleep after x seconds of no touch or input send
        */
        inline bool get_in_sleep_mode() const { return this->in_sleep_mode; }

        /*!
            @note !! need to be called continuously in the main loop of the application

            @details checks for touch data and handles the windows and widgets actions
            class also the window loop function of the active_window and the loop functions
            of the unhidden widgets which are shown in the active_window
        */
        void loop();

        /*!
            @return the upper left position of the windows(active_window) which will be shown on the display.
            @details for the windows to get their size from the main_window
        */
        const position &_get_window_upper_left() const;

        /*!
            @return the lower right position of the windows(active_window) which will be shown on the display.

            @details for the windows to get their size from the main_window
        */
        const position &_get_window_lower_right() const;

        /*!
            @return pointer to the display which the MainWindow represents
            @details for the windows and widgets to get information from the display and print themself on it.
        */
        inline UTFT *_get_display() const { return this->display; }
        /*!
            @return pionter to the touch instance which the MainWindow represents
            @details this function is rarely used and should not be used if not possible in a different way
        */
        inline URTouch *_get_touch() const { return this->touch; }

    protected:
        /*!
            @param _pos the position which should be checked if it is in the display
            @return true if _pos is in the display
            @details check if the given position is in the physical display - not the positions for the windows
            to get their size.
        */
        inline bool _check_in_display(const position &_pos) const;

        /*!
            @details read the touch from the URTouch instance
            and convert it to the position corresponding to the display

            will be called by the MainWindow loop function to read the touch from the display and redirect
            it to the matching actual_window

            @note !!! use this->touch->dataAvailable() to get if data is available
        */
        position _read_touch();

        /*!
            @details wake the display up and print the current window again - but only if the display
            is not send to sleep already
            resets the in_sleep_mode and sets it to false(woken up)
        */
        void _wake_up();

        /*!
            @details clear the display and send the display to sleep - but only if the display
            is not waked up already
            sets the in_sleep_mode to true
        */
        void _send_sleep();

    private:
        /*!
            true one widget has the focus and we are not allowed to change the focus
        */
        bool focus_frozen;
        Widget *focused_widget;    //! pointer to the actual focused widget - if there is no it is nullptr
        WindowBase *active_window; //! pointer to the active_window - if there is no it is nullptr

        UTFT *display; //! widgets and windows can get this pointer to draw on display and get informatoin
        URTouch *touch;

        /*!
            size of the "normal windows" which are shown on the main_window - the windows grab the size in their window-constructor
        */
        position window_upper_left, window_lower_right;

        /*!
            seconds which have to pass until the display falls in a sleep mode -
            the display will turn black until the next touch
        */
        uint16_t fall_asleep_timer = 60;
        /*!
            last time point the user interacted with the display/keypad
        */
        uint32_t last_interact_time_pt = 0;
        bool in_sleep_mode = false; //! true if in sleep_mode=black screen
    };
} // namespace FGUI
