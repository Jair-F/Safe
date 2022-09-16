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

    /*
        Base class for window

        !! parent_window need to be set by every derived class from the window
    */
    class WindowBase
    {
    public:
        /*
            upper_left and lower_right needs to be set by the derived class

            @param _upper_left  absolute position
            @param _lower_right absolute position
        */
        WindowBase(const position &_upper_left, const position &_lower_right, uint8_t _border_weight = 0);
        virtual ~WindowBase() {}

        // functions which derived classes need to implement with their pointer to the parent_window

        // only one widget can be focused in the whole window - the pointer, that the main_window holds
        virtual Widget *get_focused_widget() const = 0;
        virtual bool request_focus(Widget *_widget) = 0;
        virtual UTFT *_get_display() const = 0;
        virtual URTouch *_get_touch() const = 0;
        virtual MainWindow *_get_main_window() const = 0;

        /*
            show all the widgets and draw the window if required(PopUpWindow)
        */
        virtual void show();
        /*
            hide all the widgets
        */
        virtual void hide();

        /*
            the loop function that will call the widgets loop funcions - if you overwrite this function call the base class function
            in your loop function!

            function which inherited classes need to overwrite.
            It is to update the window for example with sensor values - actions which are not controlled/depend
            on touch actions.
            This function will be called from the main_window-loop - so be shure to call the main_window loop in a loop!
        */
        virtual void loop();

        void show_pop_up_window(PopUp_Window *_pop_up_window);
        void hide_pop_up_window();

        //
        // base functions of a window which are implementet by default by the WindowBase

        inline uint16_t width() const { return this->lower_right.x_pos - this->upper_left.x_pos; }
        inline uint16_t height() const { return this->lower_right.y_pos - this->upper_left.y_pos; }
        /*
            for widgets in the window to get the absolute pos on the display
            @param _pos the relative pos from the widget
            @return the according absolute pos to the relative pos
        */
        position _calc_absolute_pos(const position &_pos) const;
        /*
            @return the upper left position(absolute pos)
        */
        const position &pos() const;
        // (absolute pos)
        const position &_get_lower_right_pos() const;
        // (absolute pos)
        const position &_get_upper_left_pos() const;

        /*
            the lower right position of the content - calculated
            the border out
            !! absolute position!!!
        */
        position get_content_lower_right() const;

        /*
            the upper left position of the content - calculated
            the border out
            !! absolute position!!!
        */
        position get_content_upper_left() const;

        uint16_t get_content_width() const;
        uint16_t get_content_height() const;

        /*
            register a new widget in the list of widgets in the window to which the touch_data
            will be passed.
        */
        void _register_widget(Widget *_widget);

        /*
            the reverse function of _register_widget - removes a registered widget if its
            in the registered_widgets list
            @return true if a widget was removed, false if not
        */
        bool _unregister_widget(Widget *_widget);

        /*
            @return the pointer to the widget which will get the focus - nullptr if none.
        */
        Widget *handle_touch_clicked(const position &_touch_data);
        void handle_touch_released(const position &_touch_data);

        void set_background_color(unsigned int _background_color) { this->background_color = _background_color; }
        inline unsigned int get_background_color() const { return this->background_color; }

        uint8_t get_border_weight() const { return this->border_weight; }

        void set_border_color(unsigned int _border_color) { this->border_color = _border_color; }
        unsigned int get_border_color() const { return this->border_color; }
        /*
            @return true if a border is drawed, false if not
        */
        bool get_draw_border() const { return this->border_weight != 0; }

        /*
            redraw the whole window on the screen
        */
        void _redraw_window();

    protected:
        void _draw_background();
        void _draw_border();

        // window size - rectangle(absolute positions)
        position upper_left;
        position lower_right;

        uint8_t border_weight;
        unsigned int border_color = VGA_WHITE;
        unsigned int background_color = VGA_BLACK;

    private:
        SinglyLinkedList<Widget *> registered_widgets;

        /*
            set(not nullptr) if a pop_up window is currently showed on the window
        */
        PopUp_Window *active_pop_up_window;

        /*
            last focused widget in the window
            its used to find out wether the touch was touched and released insed the widget(positions) or not
            and according to that it calls the _released method of the widgets
        */
        Widget *last_focused_widget;
    };
} // namespace FGUI
