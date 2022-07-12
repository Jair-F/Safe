#pragma once
#include <UTFT.h>
#include <URTouch.h>

#include "position.hpp"
#include "Widget.hpp"
#include "LinkedList.hpp"
#include "MainWindow.hpp"

namespace UI
{
    class MainWindow;
    class Widget;
    // URTouch touch;
    // UTFT display;

    /*
    class for making derived customized window classes to show on the MainWindow
    */
    class Window
    {
    public:
        Window(MainWindow *main_window);
        virtual ~Window() {}
        virtual void show() = 0;
        virtual void hide() = 0;

        /*
            @param _widget request focus for _widget to get keyboard_input
            @return true if the request was accepted else false
        */
        bool request_focus(Widget *_widget);

        inline uint16_t width() const { return this->_lower_right.x_pos - this->_upper_left.x_pos; }
        inline uint16_t height() const { return this->_lower_right.y_pos - this->_upper_left.y_pos; }

        /*
            @return the upper left position(absolute pos)
        */
        const position &pos() const { return this->_upper_left; }
        // (absolute pos)
        const position &_get_lower_right_pos() const { return this->_lower_right; }
        // (absolute pos)
        const position &_get_upper_left_pos() const { return this->_upper_left; }

        /*
            calc the absolute position of the real display
        */
        position _calc_absolute_pos(const position &_pos) const;

        /*
            @return the pointer to the widget which will get the focus - nullptr if none.
        */
        Widget *handle_touch_clicked(const position &_touch_data);
        void handle_touch_released(const position &_touch_data);

        /*
            register a new widget in the list of widgets in the window to which the touch_data
            will be passed.
        */
        void _register_widget(Widget *_widget);

        UTFT *_get_display() const;
        URTouch *_get_touch() const;

        Widget *get_focused_widget() const;

        /*
            function which inherited classes need to overwrite.
            It is to update the window for example with sensor values - actions which are not controlled/depend
            on touch actions.
            This function will be called from the main_window-loop - so be shure to call the main_window loop in a loop!
        */
        virtual void loop() {}

#define WINDOW_BACKGROUND_COLOR VGA_BLACK
        /*
            get the background color defined for this window
        */
        inline unsigned int get_background_color() const
        {
            return WINDOW_BACKGROUND_COLOR;
        }

    private:
        MainWindow *_main_window; // the display itself
        Widget *last_focused_widget;
        SinglyLinkedList<Widget *> registered_widgets;

        // window size - rectangle
        position _upper_left;
        position _lower_right;
    };

} // namespace UI
