#pragma once

#include "LinkedList.hpp"

#include "Widget.hpp"
#include "MainWindow.hpp"
#include "PopUp_Window.hpp"

#include <URTouch.h>
#include <UTFT.h>

namespace FGUI
{
    class MainWindow;
    class PopUp_Window;

    /**
     * \defgroup WindowBase WindowBase
     * \ingroup FGUI
     *
     *
     * @brief Base class for every window class(MainWindow or PopUp_Window).
     * handles some basic functionalities of windows which can partly be customized by the derived classes.
     *
     * @{
     */
    class WindowBase
    {
    public:
        /*!
            @details upper_left and lower_right needs to be set by the derived classtouch
            @param _upper_left  the upper left position of the window rectangle - absolute position
            @param _lower_right the lower right position of the window rectangle - absolute position
        */
        WindowBase(const position &_upper_left, const position &_lower_right, uint8_t _border_weight = 0);
        virtual ~WindowBase() {}

        // functions which derived classes need to implement with their pointer to the parent_window

        /**
         * \defgroup IMPLEMENT_IN_DERIVED functions which derived classes need to implement
         *  first group
         * @note Need to be implemented by derived classes.
         * @{
         */

        /**
         * @return the pointer of the focused widget that the main window holds actually.
         * @note only one widget can be focused on the whole display(main window).
         */
        virtual Widget *get_focused_widget() const = 0;
        /**
         * @details request for the given widget to gain the focus manually by this call.
         * @param widget pointer to the widget which requests to gain the focus.
         * @return true if the focus gain was granted, false otherwise.
         */
        virtual bool request_focus(Widget *_widget) = 0;
        /**
         * @return get the UTFT display library instance of the display
         */
        virtual UTFT *_get_display() const = 0;
        /**
         * @return get the UTFT display library instance of the touch
         */
        virtual URTouch *_get_touch() const = 0;
        /**
         * @return get the main window instance
         */
        virtual MainWindow *_get_main_window() const = 0;

        /**@}*/

        /*!
            @details show all registered widgets which are not hidden on the display.
            if there is actually shown a pop_up window it directs this show call to the pop_up window.
        */
        virtual void show();
        /*!
            @details hide all widgets which are shown on the display(overdraw them with the window background color).
            if there is actually shown a pop_up window it directs this show call to the pop_up window.
        */
        virtual void hide();

        /*!
            @details the loop function that will call the widgets loop funcions.
            function which inherited classes need to overwrite.
            It is to update the window for example with sensor values - actions which are not controlled/depend
            on touch actions.
            @note if you overwrite this function call the base class function in your loop function!
            @note This function will be called from the main_window-loop - so be shure to call the main_window loop in a loop!
        */
        virtual void loop();

        /*!
            @details a pop up window is not shown by setting it the the active window in the main window. it must to be set with this function
            in the window over which it will be drawn.
            @param _pop_up_window the pop up window pointer to be shown.
        */
        void show_pop_up_window(PopUp_Window *_pop_up_window);
        /*!
            @details if there is shown a pop up window, hide it. if not it does nothing.
        */
        void hide_pop_up_window();

        //
        // base functions of a window which are implementet by default by the WindowBase

        /*!
            @return get the width of the window(in pixels).
        */
        inline uint16_t width() const { return this->lower_right.x_pos - this->upper_left.x_pos; }
        /*!
            @return get the height of the window(in pixels).
        */
        inline uint16_t height() const { return this->lower_right.y_pos - this->upper_left.y_pos; }
        /*!
            @details this function calculates the absolute position to a given relative position of the window.
            its for widgets in the window to get their absolute positions on the display.
            @param _pos the relative pos from the widget
            @return the according absolute pos to the relative pos
        */
        position _calc_absolute_pos(const position &_pos) const;
        /*!
            @return the upper left position(absolute pos) of the window
        */
        const position &pos() const;
        /*!
            @return (absolute pos)
        */
        const position &_get_lower_right_pos() const;
        /*!
            @return (absolute pos)
        */
        const position &_get_upper_left_pos() const;

        /*!
            @details get the lower right position of the content - calculated the border weight of the window out
            @note !! absolute position!!!
        */
        position get_content_lower_right() const;

        /*!
            @details get the upper left position of the content - calculated the border weight of the window out
            @note !! absolute position!!!
        */
        position get_content_upper_left() const;

        /*!
            @return get the width of the content space in the window(in pixels).
            @note the content space is the space in the window where the widgets can be shown - the whole size of the window
            except of the border.
        */
        uint16_t get_content_width() const;
        /*!
            @return get the height of the content space in the window(in pixels).
            @note the content space is the space in the window where the widgets can be shown - the whole size of the window
            except of the border.
        */
        uint16_t get_content_height() const;

        /*!
            @details register a new widget in the list of widgets in the window
            will be called by the widget base class in the constructor.
            @note to be shown at window show, be able to be touched, gain focus and get keypad input.
        */
        void _register_widget(Widget *_widget);

        /*!
            the reverse function of _register_widget - removes a registered widget if its
            in the registered_widgets list
            @return true if a widget was removed, false if not found in the list.
        */
        bool _unregister_widget(Widget *_widget);

        /*!
            @details this func iterates over all registered widgets and checks if the touch_data matched to one of the widget.
            if found a match it executes the according on_touch, on_release, on_focus_lose... callbacks.
            @return the pointer to the widget which will get the focus due to touched if the focus
            isnt frozen at the main_window - nullptr if none.
        */
        Widget *handle_touch_clicked(const position &_touch_data);
        /*!
            @param _touch_data position where the touch was released
            @details iterates over the registered widgets and executes the on_release callback if a match is found.
        */
        void handle_touch_released(const position &_touch_data);

        /*!
            @details the background color of the window and the color which the widgets will be overdrawn if the are hidden.
            the window needs to be redrawn manually to take effect the change.
        */
        void set_background_color(unsigned int _background_color) { this->background_color = _background_color; }
        inline unsigned int get_background_color() const { return this->background_color; }

        uint8_t get_border_weight() const { return this->border_weight; }

        /*!
            @details the window needs to be redrawn manually to take effect the change.
        */
        void set_border_color(unsigned int _border_color) { this->border_color = _border_color; }
        unsigned int get_border_color() const { return this->border_color; }
        /*!
            @return true if the border will/is drawn, false otherwise.
        */
        bool get_draw_border() const { return this->border_weight != 0; }

        /*!
            @details disables to draw the border(simply changes the border_weight to 0).
            the window needs to be redrawn manually to take effect the change.
        */
        void set_draw_border(bool _draw_border) { this->border_weight = 0; }

        /*!
            @details redraw the whole window on the screen
        */
        void _redraw_window();

    protected:
        /*!
            @details fill the window space with the background color.
        */
        void _draw_background();
        /*!
            @details draws the border of the window
        */
        void _draw_border();

        // window size - rectangle(absolute positions)
        /*!
            @details the upper left corner of the window.
            the window is defined as a rectangle.
        */
        position upper_left;
        /*!
            @details the lower right corner of the window.
            the window is defined as a rectangle.
        */
        position lower_right;

        /*!
            the border weight of the window in pixels.
        */
        uint8_t border_weight;
        unsigned int border_color = VGA_WHITE;
        unsigned int background_color = VGA_BLACK;

    private:
        /*!
            @details the list of the widgets which are registered to the window. they are shown on the display if
            they are not set to hidden and their calbacks of on_touch, on_release and on_focus_lose are called according
            to the touches on the window - but only if they arent hidden or disabled.
        */
        SinglyLinkedList<Widget *> registered_widgets;

        /*!
            @details set(not nullptr) if a pop_up window is currently showed on the window
        */
        PopUp_Window *active_pop_up_window;

        /*!
            @details last focused widget in the window
            its used to find out wether the touch was touched and released insed the widget(positions) or not
            and according to that it calls the _released method of the widgets
        */
        Widget *last_focused_widget;
    };
    /** @} */

} // namespace FGUI
