#pragma once

#include "Keypad.hpp"
#include "UI/Widget.hpp"
#include "LinkedList.hpp"
namespace UI
{
    class Window;
    class MainWindow;
    /*
        Only one instance of this object can be on the display\x
    */
    class MainWindow
    {
    public:
        MainWindow(Keypad::Keypad &_keypad) : keypad(_keypad) {}

        String get_input() { return this->keypad.get_buffer(); }

        /*
            removes also the focused widget(sets it to nullptr)
        */
        void set_active_window(Window *_win);
        Window *get_active_window() { return active_window; }

    private:
        Keypad::Keypad &keypad;
        Widget *focused_widget;
        Window *active_window;
    };

    class Window
    {
    public:
        void show();
        void hide();

    private:
        SinglyLinkedList<Widget *> widgets;
    };

    // MainWindow mainwindow;

} // namespace UI