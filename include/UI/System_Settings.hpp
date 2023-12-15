#pragma once
#include "FGUI/FGUI.hpp"

class System_Settings : public FGUI::Window
{
public:
    System_Settings(FGUI::WindowBase *_parent_window);

    virtual ~System_Settings() {}

protected:
    void _pre_show() override; // sets the time and date to the current values
    void _pre_hide() override; // resets the border of for example the save btn

    void _exit_window(FGUI::Touch_Widget<System_Settings> *_widget);

    void _handle_on_type_hour(FGUI::Touch_Widget<System_Settings> *_widget, const char *_input_buffer);
    void _handle_on_type_minute(FGUI::Touch_Widget<System_Settings> *_widget, const char *_input_buffer);
    void _handle_on_type_second(FGUI::Touch_Widget<System_Settings> *_widget, const char *_input_buffer);

    void _handle_on_type_day(FGUI::Touch_Widget<System_Settings> *_widget, const char *_input_buffer);
    void _handle_on_type_month(FGUI::Touch_Widget<System_Settings> *_widget, const char *_input_buffer);
    void _handle_on_type_year(FGUI::Touch_Widget<System_Settings> *_widget, const char *_input_buffer);
    void _check_date();

    void _handle_on_type_wday(FGUI::Touch_Widget<System_Settings> *_widget, const char *_input_buffer);

    void _handle_save(FGUI::Touch_Widget<System_Settings> *_widget);
    void _handle_reset_config(FGUI::Touch_Widget<System_Settings> *_widget);

private:
    FGUI::TextLabel window_title;
    FGUI::BitmapButton<System_Settings> back_btn;
    FGUI::Divider div;

    FGUI::TextLabel logging_level_label;
    FGUI::SingleSelectionMenu<System_Settings> logging_level_sslct;

    FGUI::TextLabel screen_timeout_label;
    FGUI::InputField<3, System_Settings> screen_timeout_input;

    FGUI::Button<System_Settings> reset_config_btn;

    FGUI::Divider window_div; // vertical divider in the middle window

    FGUI::TextLabel time_label;
    FGUI::InputField<2, System_Settings> time_h_input; // hour
    FGUI::TextLabel time_spacer1;                      // :
    FGUI::InputField<2, System_Settings> time_m_input; // minute
    FGUI::TextLabel time_spacer2;                      //:
    FGUI::InputField<2, System_Settings> time_s_input; // second

    FGUI::TextLabel date_label;
    FGUI::InputField<2, System_Settings> date_d_input; // day
    FGUI::TextLabel date_spacer1;
    FGUI::InputField<2, System_Settings> date_m_input; // month
    FGUI::TextLabel date_spacer2;
    FGUI::InputField<2, System_Settings> date_y_input; // year

    FGUI::TextLabel wday_label;
    FGUI::InputField<1, System_Settings> wday_input;

    FGUI::Button<System_Settings> save_btn;
};