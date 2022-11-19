#pragma once
#include <Arduino.h>
#include <URTouch.h>
#include <UTFT.h>
#include "position.hpp"
#include "WindowBase.hpp"

namespace FGUI
{

    typedef uint8_t *font_type;

    class WindowBase;

    /**
     * \defgroup Widget
     * \ingroup FGUI
     *
     * @brief Base class for all widgets. Size of all widgets are defined by a rectangle with 2 points(uper_left and lower_right).
     *
     * @details it holds virtual functions for drawing the border, the background and clearing the space at hiding, which
     * can be reimplemented by derived classes but are good for the most cases.
     * The _draw_content function has to be reimplemented by derived classes!
     *
     * @note the class holds the colors for the widget and also some TouchWidget functions to be able to call them at the Touch Widgets,
     *       but here the do nothing.
     *
     * @{
     */
    class Widget
    {
    public:
        /**
         * @brief widget_status -
         * represents the posible status of the widget it
         * is also used to pass the draw function which state of the widget should be drawn at touching,
         * disabled or released
         */
        enum w_status
        {
            S_RELEASED,
            S_TOUCHED,
            S_DISABLED
        };

        /**
         * @param _parent the parent window to which the widget will register to
         * @param _upper_left_pos upper left corner in relation to the parent window zero point
         * @param _lower_right lower right corner in relation to the parent window zero point
         * @param _border_weight size of the border in pixels
         */
        Widget(WindowBase *_parent, const position &_upper_left, const position &_lower_right,
               uint8_t _border_weight = 1);
        /**
         * @param _parent the parent window to which the widget will register to
         * @param _upper_left_pos upper left corner in relation to the parent window zero point
         * @param _width the width of the widget in pixels
         * @param _height the height of the widget in pixels
         * @param _border_weight size of the border in pixels
         */
        Widget(WindowBase *_parent, const position &_upper_left, uint16_t _width, uint16_t _height,
               uint8_t _border_weight = 1);
        virtual ~Widget();

        /**
         * @brief the color values are RGB-565 values(16-bit value).
         * RGB-565 color picker: https://chrishewett.com/blog/true-rgb565-colour-picker/
         *
         * @{
         */
        unsigned int released_border_color = VGA_WHITE;
        unsigned int released_background_color = VGA_BLACK;
        /** @} */

        /**
         * @return the width of the widget in pixels
         */
        inline uint16_t width() const { return lower_right.x_pos - upper_left.x_pos; }
        /**
         * @return the height of the widget in pixels
         */
        inline uint16_t height() const { return lower_right.y_pos - upper_left.y_pos; }

        /**
         * @brief resizes the widget to the new height and width
         * @note this shifts the lower_right pos. the upper_left is not changed!
         * @note the widget isnt update until the show_function is called again!
         */
        void set_size(uint16_t _width, uint16_t _height);

        /**
         * set border weight in pixels
         * @note the widget will not be redrawn - you need to redraw it manually to see the change
         */
        void set_border_weight(uint8_t _border_weight);
        /**
         * @return the border weight in pixels
         */
        inline uint8_t get_border_weight() const { return this->border_weight; }

        /**
         * @return get the upper_left position of the content - calculated the border_weight out
         * but only if draw_border is set. If draw_border is not set it returns the same as get_upper_left
         * @note !! absolute position!!!
         */
        position get_content_upper_left() const;
        /**
         * @return get the lower_right position of the content - calculated the border_weight out
         * but only if draw_border is set. If draw_border is not set it returns the same as lower_right
         * @note !! absolute position!!!
         */
        position get_content_lower_right() const;
        /**
         * @return get the content_width - calculated the border_weight out
         * but only if draw_border is set. If draw_border is not set it returns the same as width()
         */
        uint16_t get_content_width() const;
        /**
         * @return get the content_height - calculated the border_weight out
         * but only if draw_border is set. If draw_border is not set it returns the same as height()
         */
        uint16_t get_content_height() const;

        /**
         * @param _draw_border true if the button should have a border - false if not
         */
        void set_draw_border(bool _draw_border) { this->draw_border = _draw_border; }
        /**
         * @return true if the border is/will be drawn.
         * @note does the same as has_border().s
         */
        inline bool get_draw_border() const { return this->draw_border; }
        /**
         * @return true if the border is/will be drawn.
         * @note does the same as get_draw_border().
         */
        inline bool has_border() const { return this->draw_border; }

        /**
         * @return true if the widget is focused.
         * @note only declared here for th touche_widget to be able to call this func on all widgets.
         * @details focused widgets the input of the keypad - the last element that was touched
         * just defined to be able to call the derived class function touched_widget with the base class pointer
         */
        inline virtual bool is_focused() const { return false; };
        /**
         * @return true if the widget is hidden
         * @details hidden widgets are not displayed on the displayed on the display
         * even when the window is shown it will not be displayed
         */
        inline bool is_hidden() const { return this->hidden; }
        /**
         * @return true if the widget is disabled.
         * @details disabled widgets act like grayed out widgets on the computer - they are drawn on
         * the display but cant be touched.
         */
        virtual inline bool is_disabled() const { return false; }

        /**
         * @details the widget will be redrawn automatically with disabled colors
         * if a widget is disabled it cant be touched
         * @note only here to make it possible to be called on a touche widget - not available for this widget
         */
        virtual void disable() {}
        /**
         * @brief the oposite of disable
         * @details the widget will be redrawn automatically with disabled colors
         * if a widget is disabled it cant be touched
         * @note only here to make it possible to be called on a touch widget - not available for this widget
         */
        virtual void enable() {}

        /**
         * @details handle the input of the keyboard/keypad
         */
        virtual void send_input(char _input_data) {}
        /**
         * @details send the special key command backspace
         */
        virtual void send_backspace() {}
        /**
         * @details send the special key command enter
         */
        virtual void send_enter() {}

        /**
         * @details the widget will be marked as hidden - will not be shown if the window to which this
         * widget is registered to, is shown.
         * the widget will stay hidden until the method show() is called.
         * A hidden widget cant be touched on the display and its loop function will not be called
         * even if the parent window will shown on the display.
         */
        void hide();

        /**
         * @details marks the widget as hidden but doesnt changes the current state - means
         * if the widget is currently shown it leaves it - but next time it will be hidden
         */
        inline void mark_hidden() { this->hidden = true; }

        /**
         * @details marks the widget as shown and shows the widget.
         * @note the widget will be redrawn with this function call
         */
        void show();

        /**
         * @details marks the widget as shown but doesnt changes the current state - means
         * if the widget is currently hidden it leaves it - but next time it will be shown
         */
        inline void mark_shown() { this->hidden = false; }

        /**
         * @details loop function that will be called by the active window if the widget is shown/visible/not hidden
         */
        virtual void loop() {}

        /**
         * @details wrapper for redrawing or drawing the widget from outside
         * draw the widget if its not hidden
         */
        void draw();

        /**
         * @details wrapper for clear_widget_space
         * clears the widget space with the parents background color if the widget isnt already hidden
         */
        void clear();

        /**
         * @return the upper left position
         * @note the absolute position will be returned!
         */
        inline const position &pos() const { return upper_left; }
        /**
         * @return the upper left position
         * @note the absolute position will be returned!
         */
        inline const position &get_upper_left_pos() const { return this->upper_left; }
        /**
         * @return the lower right position
         * @note the absolute position will be returned!
         */
        inline const position &get_lower_right_pos() const { return this->lower_right; }

        /**
         * @details for sending click-signal. if the widget is hidden this function will not make anything
         * @note -- defined just to create the ability to call the derived function in touch_widget with the base class pointer -
         * in this widget class it wont make anything.
         * @param _touch_data absolute position of the touch
         */
        virtual void _touch(const position &_touch_data) {}

        /**
         * @details for sending release-signal if the widget is hidden this function will not make anything
         * @note -- defined just to create the ability to call the derived function in touch_widget with the base class pointer -
         * in this widget class it wont make anything.
         * @param _touch_data absolute position of the release of the touch
         */
        virtual void _release(const position &_touch_data) {}

        /**
         * @details resets the widget state to released if the widget was touched and not released without calling the
         * on_release callback function.
         * if the widget is hidden this function will not make anything
         * @note -- defined just to create the ability to call the derived function in touch_widget with the base class pointer -
         * in this widget class it wont make anything.
         */
        virtual void _reset_touch() {}

        /**
         * @details for sending focus loose signal to the widget.
         * @note -- defined just to create the ability to call the derived function in touch_widget with the base class pointer -
         * in this widget class it wont make anything.
         */
        virtual void _focus_lose() {}

        /**
         * @details check if _pos is in the widget.
         * @param _pos a touch position - needs to be absolute!
         * @return true if in the widget - else false
         */
        bool _check_pos(const position &_pos) const;

        inline WindowBase *_get_parent_window() const { return this->parent_window; }

    protected:
        WindowBase *parent_window;

        // getting the display pointers from the parent window in the constructor
        URTouch *touch;
        UTFT *display;

        /**
         * @details absoloute positions on the display
         *
         * @{
         */
        position upper_left, lower_right;
        /** @} */

        /**
         * @details draws the content of the widget according to the passed status - touched, released
         * or disabled...
         * @param _st whether to draw the status disabled, touched or released
         */
        virtual void _draw_content(w_status _st) {}

        /**
         * @details draws the widget on the screen - according to his actual status.
         */
        virtual void _draw_widget();

        /**
            @details hide the widget - clears the rectangle space where the widget is drawn
            clears the space with the background color of the parent window
        */
        virtual void _clear_widget_space();

        /**
         * @details draws the border of the widget according to the passed status.
         * @param _st whether to draw the status disabled, touched or released
         */
        virtual void _draw_border(w_status _st);
        /**
         * @details draws the border of the widget according to the passed status.
         * @param _st whether to draw the status disabled, touched or released
         */
        virtual void _draw_background(w_status _st);

    private:
        /**
         * if true, the widget will not be shown on the display if the window is shown. if the widget is hidden
         * it also cant be touched.
         */
        bool hidden = false;

        /**
         * if true the border of the widget will be drawn.
         */
        bool draw_border = true;
        /**
         * the borderweight in pixels of the widget
         */
        uint8_t border_weight;
    };

    /** @} */

}
