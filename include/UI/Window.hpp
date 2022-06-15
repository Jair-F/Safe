#pragma once
#include <UTFT.h>
#include <URTouch.h>

#include "Widget.hpp"
#include "LinkedList.hpp"
#include "MainWindow.hpp"

namespace UI
{
    class MainWindow;
    // URTouch touch;
    // UTFT display;

    /*
    class for making derived customized window classes to show on the MainWindow
    */
    class Window
    {
    public:
        Window() {}
        virtual ~Window();
        void show();
        void hide();

        /*
            @param _widget request focus for _widget to get keyboard_input
            @return true if the request was accepted else false
        */
        bool request_focus(Widget *_widget);

    private:
        MainWindow *main_window; // the display itself
    };

} // namespace UI
