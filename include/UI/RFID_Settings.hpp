#pragma once
#include "FGUI/FGUI.hpp"
#include "RFID/RFID.hpp"
#include "lock_screen.hpp"
#include "RFID_add_tag_by_read.hpp"
#include "RFID_delete_tag_by_id.hpp"
#include "InfoPopUP.hpp"
#include "UI/RFID_clear_database.hpp"

class RFID_settings_window : public FGUI::Window
{
public:
    RFID_settings_window(FGUI::WindowBase *_parent_window);

    void loop() override;

protected:
    void _exit_window(FGUI::Touch_Widget<RFID_settings_window> *_widget);
    void _show_add_tag_window(FGUI::Touch_Widget<RFID_settings_window> *_widget);
    void _show_clear_database_window(FGUI::Touch_Widget<RFID_settings_window> *_widget);
    void _show_delete_tag_by_id(FGUI::Touch_Widget<RFID_settings_window> *_widget);

private:
    FGUI::TextLabel window_title;

    FGUI::Button<RFID_settings_window> add_tag_btn;
    FGUI::Button<RFID_settings_window> delete_tag_by_read_btn;
    FGUI::Button<RFID_settings_window> delete_tag_by_id_btn;
    FGUI::Button<RFID_settings_window> clear_database_btn;

    FGUI::BitmapButton<RFID_settings_window> home_btn;

    RFID_add_tag_window rfid_add_tag_wnd;
    RFID_delete_tag_by_id rfid_delete_tag_by_id;
    RFID_clear_database clear_database_p_wnd;
};
