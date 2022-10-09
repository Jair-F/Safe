#pragma once
#include "FGUI/FGUI.hpp"

class lock_screen : public FGUI::Window
{
public:
    lock_screen(FGUI::MainWindow *_main_window);
    virtual ~lock_screen() {}

    friend void func();

protected:
    void uncheck_check_box_pop_up(FGUI::Touch_Widget<lock_screen> *_widget);
    void update_status_bar(FGUI::Touch_Widget<lock_screen> *_widget);

    void button_print_clicked(FGUI::Touch_Widget<lock_screen> *_widget);

    void button_print_released(FGUI::Touch_Widget<lock_screen> *_widget);

    void _handle_check_box(FGUI::Touch_Widget<lock_screen> *_widget);

    void restart_system(FGUI::Touch_Widget<lock_screen> *_widget);

    void update_window_label(FGUI::Touch_Widget<lock_screen> *_widget);

    void input_field_adjust(FGUI::Touch_Widget<lock_screen> *_widget, const char *_input_buffer);

private:
    FGUI::Button<lock_screen> _button;
    FGUI::Button<lock_screen> _button2;
    FGUI::TextLabel text_feld;
    FGUI::CheckBox<lock_screen> ch_box;
    FGUI::InputField<20, lock_screen> input_field;
    FGUI::PopUp_Window pop_up_window;

    FGUI::TextLabel status_bar;

    FGUI::BitmapButton<lock_screen> b_button;
    FGUI::Divider div;
    FGUI::ProgressBar progressBar;

    FGUI::Giff u_giff;

    FGUI::Button<lock_screen> p_button;
    FGUI::SingleSelectionMenu<lock_screen> selction_menu;
};
