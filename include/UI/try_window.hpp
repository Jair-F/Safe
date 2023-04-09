#pragma once
#include "FGUI/FGUI.hpp"

class try_window : public FGUI::Window
{
public:
    try_window(FGUI::MainWindow *_main_window);
    virtual ~try_window() {}

    friend void func();

protected:
    void uncheck_check_box_pop_up(FGUI::Touch_Widget<try_window> *_widget);
    void update_status_bar(FGUI::Touch_Widget<try_window> *_widget);

    void button_print_clicked(FGUI::Touch_Widget<try_window> *_widget);

    void button_print_released(FGUI::Touch_Widget<try_window> *_widget);

    void _handle_check_box(FGUI::Touch_Widget<try_window> *_widget);

    void restart_system(FGUI::Touch_Widget<try_window> *_widget);

    void update_window_label(FGUI::Touch_Widget<try_window> *_widget);

    void input_field_adjust(FGUI::Touch_Widget<try_window> *_widget, const char *_input_buffer);

private:
    FGUI::Button<try_window> _button;
    FGUI::Button<try_window> _button2;
    FGUI::TextLabel text_feld;
    FGUI::CheckBox<try_window> ch_box;
    FGUI::InputField<20, try_window> input_field;
    FGUI::PopUp_Window pop_up_window;

    FGUI::TextLabel status_bar;

    FGUI::BitmapButton<try_window> b_button;
    FGUI::Divider div;
    FGUI::ProgressBar progressBar;

    FGUI::Giff u_giff;

    FGUI::Button<try_window> p_button;
    FGUI::SingleSelectionMenu<try_window> selction_menu;
};
