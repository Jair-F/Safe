#pragma once
#include <UTFT.h>
#include <URTouch.h>

#include "LinkedList.hpp"

#include "position.hpp"
#include "Widget.hpp"
#include "WindowBase.hpp"
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
    class Window : public WindowBase
    {
    public:
        Window(MainWindow *main_window);
        virtual ~Window() {}

        /*
            @param _widget request focus for _widget to get keyboard_input
            @return true if the request was accepted else false
        */
        bool request_focus(Widget *_widget);

        UTFT *_get_display() const;
        URTouch *_get_touch() const;
        Widget *get_focused_widget() const;

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
    };

} // namespace UI
