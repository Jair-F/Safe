#pragma once
#include <Arduino.h>
#include <URTouch.h>
#include <UTFT.h>
#include "position.hpp"
#include "WindowBase.hpp"

namespace UI
{
    class WindowBase;

    /*
        enum Mode
        {
            Normal,  // nothing - normal(not clicked or focused)
            Focused, // after clicking on it its focused - ready for input
            Clicked
        };
        */

    class Widget
    {
    private:
        bool hidden = true;

        bool draw_border = true;
        uint8_t border_weight;

    protected:
        WindowBase *parent_window;

        // getting the display pointers from the parent window in the constructor
        URTouch *touch;
        UTFT *display;

        /*
            absoloute positions on the display
        */
        position upper_left, lower_right;

        /*
            for touch_widgets we need released and pressed widget... on normal widget - which can not be touched
            this function and the pressed widget are doing the same
        */
        virtual void _draw_released_content();

        /*
            draws the widget on the screen - according to his actual status it calls _draw_released_widget or
            _draw_touched_content
        */
        virtual void _draw_widget() { this->_draw_released_content(); }

        /*
            hide the widget - clear the space where the widget is drawn
            the widget space is always a rectangle
            clear the space with the background color of the window in which the widget is registerd tos
        */
        virtual void _clear_widget_space();

        virtual void _draw_released_border();
        virtual void _draw_released_background();

    public:
        /*
            @param _uppser_left_pos upper left corner in relation to the parent window zero point
        */
        Widget(WindowBase *_parent, const position &_upper_left, const position &_lower_right,
               uint8_t _border_weight = 1);
        Widget(WindowBase *_parent, const position &_upper_left, uint16_t _width, uint16_t _height,
               uint8_t _border_weight = 1);
        virtual ~Widget() {}

        /*
            the color values are RGB-565 values(16-bit value)
            RGB-565 color picker: https://chrishewett.com/blog/true-rgb565-colour-picker/
        */
        unsigned int released_border_color = VGA_WHITE;
        unsigned int released_background_color = VGA_BLACK;

        inline uint16_t width() const { return lower_right.x_pos - upper_left.x_pos; }
        inline uint16_t height() const { return lower_right.y_pos - upper_left.y_pos; }

        /*
            resize the widget
            the widget isnt update until the show_function is called again
        */
        void set_size(uint16_t _width, uint16_t _height);

        /*
            border weight in pixels
        */
        void set_border_weight(uint8_t _border_weight);
        inline uint8_t get_border_weight() const { return this->border_weight; }

        /*
            get the upper_left position of the content - calculated the border_weight out
            but only if draw_border is set. If draw_border is not set it returns the same as get_upper_left
        */
        position get_content_upper_left() const;
        /*
            get the lower_right position of the content - calculated the border_weight out
            but only if draw_border is set. If draw_border is not set it returns the same as lower_right
        */
        position get_content_lower_right() const;
        /*
            get the content_width - calculated the border_weight out
            but only if draw_border is set. If draw_border is not set it returns the same as width()
        */
        uint16_t get_content_width() const;
        /*
            get the content_height - calculated the border_weight out
            but only if draw_border is set. If draw_border is not set it returns the same as height()
        */
        uint16_t get_content_height() const;

        /*
            @param _draw_border true if the button should have a border - false if not
        */
        void set_draw_border(bool _draw_border) { this->draw_border = _draw_border; }
        bool get_draw_border() const { return this->draw_border; }

        /*
            if the element has focus it gets the input of the keypad - the last element that was touched
            just defined to be able to call the derived class function touched_widget with the base class pointer
        */
        inline virtual bool is_focused() const { return false; };
        inline bool is_hidden() const { return this->hidden; }

        /*
            handle the input of the keyboard/keypad
        */
        virtual void send_input(char _input_data) {}
        virtual void send_backspace() {} // handle backspace signal keypad
        virtual void send_enter() {}     // handle enter signal from keypad

        /*
            set is_hidden to true and clear the whole widget from the screen
        */
        void hide();

        /*
            set is_hidden to false and draw the whole widget on the screen
        */
        void show();

        /*
            wrapper for redrawing the widget from outside
        */
        void update_widget();

        /*
            @return the upper left position
        */
        inline const position &pos() const { return upper_left; }

        /*
            for sending click-signal
            if the widget is hidden this function will not make anything

            -- defined just to create the ability to call the derived function in touch_widget with the base class pointer

            @param _touch_data absolute position of the touch
        */
        virtual void _touch(const position &_touch_data) {}

        /*
            for sending release-signal
            if the widget is hidden this function will not make anything

            -- defined just to create the ability to call the derived function in touch_widget with the base class pointer

            @param _touch_data absolute position of the release of the touch
        */
        virtual void _release(const position &_touch_data) {}

        /*
            if the widget was clicked but not released on the pos
            if the widget is hidden this function will not make anything

            -- defined just to create the ability to call the derived function in touch_widget with the base class pointer
        */
        virtual void _reset_touch() {}

        /*
            function which will be called if the widget looses the focus.
            -- defined just to create the ability to call the derived function in touch_widget with the base class pointer

            btw. function for _focus_gain is the _touch function...
        */
        virtual void _focus_lose() {}

        /*
            check if _pos is in the widget.
            @return true if in the widget - else false
        */
        bool _check_pos(const position &_pos) const;

        inline WindowBase *_get_parent_window() const { return this->parent_window; }
    };
}
