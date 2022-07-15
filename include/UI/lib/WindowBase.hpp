#pragma once

#include "LinkedList.hpp"
#include "Widget.hpp"

#include <URTouch.h>
#include <UTFT.h>

namespace UI
{
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
        WindowBase(const position &_upper_left, const position &_lower_right) : upper_left(_upper_left), lower_right(_lower_right),
                                                                                registered_widgets(), last_focused_widget(nullptr) {}
        virtual ~WindowBase() {}

        virtual void show() = 0;
        virtual void hide() = 0;
        /*
            function which inherited classes need to overwrite.
            It is to update the window for example with sensor values - actions which are not controlled/depend
            on touch actions.
            This function will be called from the main_window-loop - so be shure to call the main_window loop in a loop!
        */
        virtual void loop() {}

        // functions which derived classes need to implement with their pointer to the parent_window

        // only one widget can be focused in the whole window - the pointer, that the main_window holds
        virtual Widget *get_focused_widget() const = 0;
        virtual bool request_focus(Widget *_widget) = 0;
        virtual UTFT *_get_display() const = 0;
        virtual URTouch *_get_touch() const = 0;

        //
        // base functions of a window which are implementet by default by the WindowBase

        inline uint16_t width() const
        {
            return this->lower_right.x_pos - this->upper_left.x_pos;
        }
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
        const position &pos() const { return this->upper_left; }
        // (absolute pos)
        const position &_get_lower_right_pos() const { return this->lower_right; }
        // (absolute pos)
        const position &_get_upper_left_pos() const { return this->upper_left; }

        /*
            register a new widget in the list of widgets in the window to which the touch_data
            will be passed.
        */
        void _register_widget(Widget *_widget);
        /*
            @return the pointer to the widget which will get the focus - nullptr if none.
        */
        Widget *handle_touch_clicked(const position &_touch_data);
        void handle_touch_released(const position &_touch_data);

    protected:
        // window size - rectangle(absolute positions)
        position upper_left;
        position lower_right;

    private:
        SinglyLinkedList<Widget *> registered_widgets;

        /*
            last focused widget in the window
            its used to find out wether the touch was touched and released insed the widget(positions) or not
            and according to that it calls the _released method of the widgets
        */
        Widget *last_focused_widget;
    };
} // namespace UI
