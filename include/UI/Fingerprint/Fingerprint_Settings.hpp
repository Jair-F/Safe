#pragma once
#include "FGUI/FGUI.hpp"
#include "Fingerprint.hpp"
#include "UI/Fingerprint/add_fingerprint.hpp"

class Fingerprint_settings_window : public FGUI::Window
{
public:
    Fingerprint_settings_window(FGUI::WindowBase *_parent_window);

protected:
    void _exit_window(FGUI::Touch_Widget<Fingerprint_settings_window> *_widget);
    void _show_add_finger_window(FGUI::Touch_Widget<Fingerprint_settings_window> *_widget);
    void _show_clear_database_window(FGUI::Touch_Widget<Fingerprint_settings_window> *_widget) {}
    void _show_delete_finger_by_id(FGUI::Touch_Widget<Fingerprint_settings_window> *_widget) {}

private:
    FGUI::TextLabel window_title;

    FGUI::Button<Fingerprint_settings_window> add_finger_btn;
    FGUI::Button<Fingerprint_settings_window> delete_finger_by_read_btn;
    FGUI::Button<Fingerprint_settings_window> delete_finger_by_id_btn;
    FGUI::Button<Fingerprint_settings_window> clear_database_btn;

    FGUI::BitmapButton<Fingerprint_settings_window> home_btn;

    add_fingerprint add_finger_by_read;
};
