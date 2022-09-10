#pragma once
#include "FGUI/FGUI.hpp"
#include "RFID/RFID.hpp"
#include "lock_screen.hpp"

class RFID_settings_window : public FGUI::Window
{
public:
    RFID_settings_window(FGUI::MainWindow *_main_window);

protected:
    void _go_home(FGUI::Touch_Widget<RFID_settings_window> *_widget);

private:
    FGUI::Button<RFID_settings_window> add_tag_btn;
    FGUI::Button<RFID_settings_window> delete_tag_by_read_btn;
    FGUI::Button<RFID_settings_window> delete_tag_by_id_btn;
    FGUI::Button<RFID_settings_window> clear_database_btn;

    FGUI::BitMapButton<RFID_settings_window> home_btn;
};
