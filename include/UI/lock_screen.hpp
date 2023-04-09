#pragma once
#include "FGUI/FGUI.hpp"

class lock_screen : public FGUI::Window
{
public:
    lock_screen(FGUI::MainWindow *_main_window);
    virtual ~lock_screen() {}

    void loop() override;
    void send_input(char _input_data) override;
    // void send_enter() override;
    void send_backspace() override;

protected:
    void _request_open_settings(FGUI::Touch_Widget<lock_screen> *_widget);
    void _open_info(FGUI::Touch_Widget<lock_screen> *_widget);

    // show the input buffer of the PIN on the status label
    void _show_pin_input();

private:
    FGUI::TextLabel window_title;
    FGUI::Divider window_title_div;

    FGUI::TextLabel status_label;

    FGUI::BitmapButton<lock_screen> settings_btn;
    FGUI::BitmapButton<lock_screen> info_btn;
};
