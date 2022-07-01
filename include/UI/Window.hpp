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
        Window(MainWindow *_main_window, position upper_left, uint16_t width, uint16_t height) : _main_window(_main_window), _upper_left(upper_left), _lower_right{width, height}, last_focused_widget(nullptr) {}
        Window(MainWindow *_main_window, position upper_left, position lower_right) : _main_window(_main_window), _upper_left(upper_left), _lower_right(lower_right), last_focused_widget(nullptr) {}
        virtual ~Window() {}
        virtual void show() = 0;
        virtual void hide() = 0;
        virtual void loop() = 0;

        /*
            @param _widget request focus for _widget to get keyboard_input
            @return true if the request was accepted else false
        */
        bool request_focus(Widget *_widget);

        uint16_t width() { return this->_lower_right.x_pos - this->_upper_left.x_pos; }
        uint16_t height() { return this->_lower_right.y_pos - this->_upper_left.y_pos; }

        /*
            @return the upper left position
        */
        const position &pos() const { return this->_upper_left; }

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

    private:
        MainWindow *_main_window; // the display itself

        // window size - rectangle
        position _upper_left;
        position _lower_right;
        Widget *last_focused_widget;

        SinglyLinkedList<Widget *> registered_widgets;
    };

} // namespace UI
