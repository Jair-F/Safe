#pragma once
#include "FGUI/FGUI.hpp"

#include "UI/Fingerprint/Fingerprint_Settings.hpp"
#include "UI/RFID_Settings.hpp"
#include "UI/PIN_Settings.hpp"
#include "UI/Lock_Settings.hpp"
#include "UI/System_Settings.hpp"

class Settings_window : public FGUI::Window
{
public:
    Settings_window(FGUI::WindowBase *_parent_window);
    virtual ~Settings_window(){};

protected:
    void _pre_show() override;

    void _go_to_lockscreen(FGUI::Touch_Widget<Settings_window> *_widget);
    void _show_rfid_settings_window(FGUI::Touch_Widget<Settings_window> *_widget);
    void _show_fingerprint_settings_window(FGUI::Touch_Widget<Settings_window> *_widget);
    void _show_pin_settings_window(FGUI::Touch_Widget<Settings_window> *_widget);
    void _show_lock_settings_window(FGUI::Touch_Widget<Settings_window> *_widget);
    void _show_system_settings_window(FGUI::Touch_Widget<Settings_window> *_widget);
    void _show_status_info_window(FGUI::Touch_Widget<Settings_window> *_widget) {}
    void _handle_save_config(FGUI::Touch_Widget<Settings_window> *_widget);

private:
    FGUI::TextLabel window_title;
    FGUI::Divider div;
    FGUI::BitmapButton<Settings_window> back_btn; // lock_screen
    FGUI::BitmapButton<Settings_window> save_btn;

    FGUI::Button<Settings_window> rfid_settings_btn;
    FGUI::Button<Settings_window> fingerprint_settings_btn;
    FGUI::Button<Settings_window> pin_settings_btn;
    FGUI::Button<Settings_window> lock_settings_btn;
    FGUI::Button<Settings_window> system_settings_btn;
    FGUI::Button<Settings_window> status_info_btn;

    Fingerprint_settings_window fingeprint_settings;
    RFID_settings_window rfid_settings;
    PIN_Settings pin_settings_wnd;
    Lock_Setings lock_settings;
    System_Settings system_settings;
};