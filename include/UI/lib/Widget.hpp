#pragma once
#include <Arduino.h>
#include <URTouch.h>
#include <UTFT.h>
#include "position.hpp"
#include "Window.hpp"

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
        bool hidden;
        Window *_parent_window;

    protected:
        // getting the display pointers from the parent window in the constructor
        URTouch *touch;
        UTFT *display;

        /*
            for touch_widgets we need released and pressed widget... on normal widget - which can not be touched
            this function and teh pressed widget are doing the same
        */
        virtual void _draw_released_widget() = 0;
        /*
            for touch_widgets we need released and pressed widget... on normal widget - which can not be touched
            this function and teh released widget are doing the same
        */
        virtual void _draw_pressed_widget() = 0;

        /*
            hide the widget - clear the space where the widget is drawn
        */
        virtual void _clear_widget_space() = 0;

        /*
            draws the widget on the screen - according to his actual status it calls _draw_released_widget or
            _draw_pressed_widget
        */
        virtual void _draw_widget() = 0;

        position _upper_left_pos, _lower_right_pos;

    public:
        virtual ~Widget() {}

        /*
            @param _uppser_left_pos upper left corner in relation to the parent window zero point
        */
        Widget(Window *_parent, const position _upper_left_pos, const position _lower_right_pos);
        Widget(Window *_parent, const position &_upper_left_pos, unsigned int _width, unsigned int _height);

        uint16_t width() { return _lower_right_pos.x_pos - _upper_left_pos.x_pos; }
        uint16_t height() { return _lower_right_pos.y_pos - _upper_left_pos.y_pos; }

        /*
            if the element has focus it gets the input of the keypad - the last element that was touched
            just defined to be able to call the derived class function touched_widget with the base class pointer
        */
        virtual bool is_focused() = 0;

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
            if the widget is hidden this function will not make anything

            -- defined just to create the ability to call the derived function in touch_widget with the base class pointer
        */
        virtual void _click() = 0;

        /*
            for sending release-signal
            if the widget is hidden this function will not make anything

            -- defined just to create the ability to call the derived function in touch_widget with the base class pointer
        */
        virtual void _release() = 0;

        /*
            if the widget was clicked but not released on the pos
            if the widget is hidden this function will not make anything

            -- defined just to create the ability to call the derived function in touch_widget with the base class pointer
        */
        virtual void _reset_click() = 0;

        /*
            check if _pos is in the widget.
            @return true if in the widget - else false
        */
        bool _check_pos(const position &_pos);

        Window *_get_parent_window() const { return this->_parent_window; }
    };
}
