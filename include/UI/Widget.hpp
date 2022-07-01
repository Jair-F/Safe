#pragma once
#include <Arduino.h>
#include <URTouch.h>
#include <UTFT.h>
#include "position.hpp"
#include "UI/Window.hpp"

namespace UI
{
    class Window;

    enum Mode
    {
        Normal,  // nothing - normal(not clicked or focused)
        Focused, // after clicking on it its focused - ready for input
        Clicked
    };

    class Widget
    {
    private:
        bool focused; // if the object has the focus - the last element that was clicked
        bool hidden;
        Window *_parent_window;

    protected:
        URTouch &touch;
        UTFT &display;

        virtual void _draw_released_widget() = 0;

        virtual void _draw_pressed_widget() = 0;

        /*
            hide the widget - clear the space where the widget is drawn
        */
        virtual void _clear_widget_space() = 0;

        position _upper_left_pos, _lower_right_pos;

    public:
        virtual ~Widget() {}

        // if the widget is touched
        void (*on_click)() = []() {};
        void (*on_release)() = []() {};

        /*
            @param _uppser_left_pos upper left corner in relation to the parent window zero point
        */
        Widget(Window *_parent, const position _upper_left_pos, const position _lower_right_pos, UTFT &_display, URTouch &_touch);
        // Widget(position &_upper_left_pos, unsigned int _width, unsigned int _height, UTFT &_display, URTouch &_touch);

        uint16_t width() { return _lower_right_pos.x_pos - _upper_left_pos.x_pos; }
        uint16_t height() { return _lower_right_pos.y_pos - _upper_left_pos.y_pos; }

        /*
            if the element has focus it gets the input of the keypad - the last element that was touched
        */
        bool is_focused() { return this->focused; }

        void remove_focus();

        bool is_hidden() { return this->hidden; }

        /*
            handle the input of the keyboard/keypad
        */
        virtual void send_input(char _input_data) {}

        void hide();
        void show();

        /*
            @return the upper left position
        */
        const position &pos() const { return _upper_left_pos; }

        /*
            for sending click-signal
        */
        void _click();

        /*
            for sending release-signal
        */
        void _release();

        /*
            if the widget was clicked but not released on the pos
        */
        void _reset_click();

        /*
            check if _pos is in the widget.
            @return true if in the widget - else false
        */
        bool _check_pos(const position &_pos);

        void loop();
    };
}
