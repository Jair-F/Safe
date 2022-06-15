#pragma once
#include <Arduino.h>
#include <URTouch.h>
#include <UTFT.h>
#include "UI/Window.hpp"

namespace UI
{
    class Window;
    struct position
    {
        unsigned int x_pos; // horizontal
        unsigned int y_pos; // vertical
    };

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
        Window *parent_window;

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
        void (*on_click)() = []() {};
        void (*on_release)() = []() {};
        Widget(position &_upper_left_pos, position &_lower_right_pos, UTFT &_display, URTouch &_touch);
        // Widget(position &_upper_left_pos, unsigned int _width, unsigned int _height, UTFT &_display, URTouch &_touch);

        unsigned int width() { return _lower_right_pos.x_pos - _upper_left_pos.x_pos; }
        unsigned int height() { return _lower_right_pos.y_pos - _upper_left_pos.y_pos; }

        /*
            if the element has focus it gets the input of the keypad - the last element that was touched
        */
        bool is_focused() { return this->focused; }

        void remove_focus();

        bool is_hidden() { return this->hidden; }

        virtual void send_input(char _input_data) {}

        void hide();
        void show();

        /*
            @return the upper left position
        */
        const position &pos() const { return _upper_left_pos; }

        void loop();
    };
}
