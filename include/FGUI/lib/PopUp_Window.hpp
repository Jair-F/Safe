#pragma once

#include "MainWindow.hpp"
#include "WindowBase.hpp"
#include "CloseButtons.hpp"

namespace FGUI
{
    class MainWindow;

    /*
        a pop up window which will be centerd in the display
    */
    class PopUp_Window : public WindowBase
    {
    public:
        PopUp_Window(WindowBase *_parent_window, uint16_t _width, uint16_t _height,
                     uint8_t _border_weight = 2, uint16_t _close_button_size = 22);
        virtual ~PopUp_Window();

        // void show() override;
        // void hide() override;
        Widget *get_focused_widget() const override;
        bool request_focus(Widget *_widget) override;

        void hide_close_button();
        void show_close_button();
        bool is_close_button_hidden();

        UTFT *_get_display() const override;
        URTouch *_get_touch() const override;
        MainWindow *_get_main_window() const override;

        WindowBase *_get_parent_window() const;

    protected:
        /*
            if the X of the pop_up_window is touched this method will be executed to hide the pop_up_window properly
            and adjust the parent window variables accordingly
        */
        void _exit_pop_up_window(Touch_Widget<PopUp_Window> *_widget);

    private:
        WindowBase *parent_window;

        uint16_t close_button_size = 22; // width and height
        CloseButton<PopUp_Window> close_button;
    };
} // namespace FGUI
