#pragma once
#include <Arduino.h>
#include <URTouch.h>
#include <UTFT.h>
#include "position.hpp"
#include "WindowBase.hpp"

namespace FGUI
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
        /*
            if the window is currently hidden on which the widget is registerd on
            it will be hidden. changes if the window gets shown and hidden
        */
        bool hidden = false;

        bool draw_border = true;
        uint8_t border_weight;

    public:
        /*
            widget_status -
            represents the posible status of the widget
            is also used to pass the draw function which state of the widget should be drawn at touching,
            disabled or released
        */
        enum w_status
        {
            S_RELEASED,
            S_TOUCHED,
            S_DISABLED
        };

        /*
            @param _uppser_left_pos upper left corner in relation to the parent window zero point
        */
        Widget(WindowBase *_parent, const position &_upper_left, const position &_lower_right,
               uint8_t _border_weight = 1);
        Widget(WindowBase *_parent, const position &_upper_left, uint16_t _width, uint16_t _height,
               uint8_t _border_weight = 1);
        virtual ~Widget();

        /*
            the color values are RGB-565 values(16-bit value)
            RGB-565 color picker: https://chrishewett.com/blog/true-rgb565-colour-picker/
        */
        unsigned int released_border_color = VGA_WHITE;
        unsigned int released_background_color = VGA_BLACK;

        inline uint16_t width() const { return lower_right.x_pos - upper_left.x_pos; }
        inline uint16_t height() const { return lower_right.y_pos - upper_left.y_pos; }

        /*
            resize the widget - only shift the lower_right pos
            the widget isnt update until the show_function is called again
        */
        void set_size(uint16_t _width, uint16_t _height);

        /*
            border weight in pixels
            redraw the widget manually
        */
        void set_border_weight(uint8_t _border_weight);
        inline uint8_t get_border_weight() const { return this->border_weight; }

        /*
            get the upper_left position of the content - calculated the border_weight out
            but only if draw_border is set. If draw_border is not set it returns the same as get_upper_left
            !! absolute position!!!
        */
        position get_content_upper_left() const;
        /*
            get the lower_right position of the content - calculated the border_weight out
            but only if draw_border is set. If draw_border is not set it returns the same as lower_right
            !! absolute position!!!
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
        virtual inline bool is_disabled() const { return false; }

        /*
            @param _disabled true if the widget should be disabled
            the widget will be redrawn automatically with disabled colors
            if a widget is disabled it cant be touched
            only here to make it possible to be called on a touche widget - not available for this widget
        */
        virtual void set_disabled(bool _disabled) {}

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
            loop function that will be called by the active window if the widget is shown/visible
        */
        virtual void loop() {}

        /*
            wrapper for redrawing or drawing the widget from outside
            draw the widget if its not hidden
        */
        void draw();

        /*
            wrapper for clear_widget_space
            clears the widget space with the parents background color if the widget isnt hidden
        */
        void clear();

        /*
            @return the upper left position
        */
        inline const position &pos() const { return upper_left; }
        inline const position &get_lower_right_pos() const { return this->lower_right; }

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
            @param _st whether to draw the status disabled, touched or released
            draws the content of the widget according to the actual status of the widget - touched, released
            or disabled...
        */
        virtual void _draw_content(w_status _st) {}

        /*
            draws the widget on the screen - according to his actual status it calls _draw_released_widget or
            _draw_content or disabled
        */
        virtual void _draw_widget();

        /*
            hide the widget - clear the space where the widget is drawn
            the widget space is always a rectangle
            clear the space with the background color of the window in which the widget is registerd tos
        */
        virtual void _clear_widget_space();

        /*
            @param _st whether to draw the status disabled, touched or released
        */
        virtual void _draw_border(w_status _st);
        /*
            @param _st whether to draw the status disabled, touched or released
        */
        virtual void _draw_background(w_status _st);
    };
}
