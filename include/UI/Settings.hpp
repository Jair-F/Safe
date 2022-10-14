#pragma once
#include "FGUI/FGUI.hpp"

#include "UI/Fingerprint/Fingerprint_Settings.hpp"
#include "UI/RFID_Settings.hpp"

class Settings_window : public FGUI::Window
{
public:
    Settings_window(FGUI::MainWindow *_main_window);

protected:
    void _go_to_lockscreen(FGUI::Touch_Widget<Settings_window> *_widget);
    void _show_rfid_settings_window(FGUI::Touch_Widget<Settings_window> *_widget);
    void _show_fingerprint_settings_window(FGUI::Touch_Widget<Settings_window> *_widget);
    void _show_pin_settings_window(FGUI::Touch_Widget<Settings_window> *_widget) {}
    void _show_lock_settings_window(FGUI::Touch_Widget<Settings_window> *_widget) {}
    void _show_system_settings_window(FGUI::Touch_Widget<Settings_window> *_widget) {}
    void _show_status_info_window(FGUI::Touch_Widget<Settings_window> *_widget) {}

private:
    FGUI::TextLabel window_title;

    FGUI::Button<Settings_window> fingerprint_settings_btn;
    FGUI::Button<Settings_window> rfid_settings_btn;
    FGUI::Button<Settings_window> pin_settings_btn;
    FGUI::Button<Settings_window> lock_settings_btn;
    FGUI::Button<Settings_window> system_settings_btn;
    FGUI::Button<Settings_window> status_info_btn;

    Fingerprint_settings_window fingeprint_settings;
    RFID_settings_window rfid_settings;
};