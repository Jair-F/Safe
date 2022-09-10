#pragma once
#include "FGUI.hpp"
#include "RFID/RFID.hpp"
#include "lock_screen.hpp"

extern const unsigned short white_repicthousebase_1484336388[];
class lock_screen;
extern lock_screen *l_screen;

extern uint8_t TinyFont[];

class RFID_settings_window : public UI::Window
{
public:
    RFID_settings_window(UI::MainWindow *_main_window);

protected:
    void _go_home(UI::Touch_Widget<RFID_settings_window> *_widget);

private:
    UI::Button<RFID_settings_window> add_tag_btn;
    UI::Button<RFID_settings_window> delete_tag_by_read_btn;
    UI::Button<RFID_settings_window> delete_tag_by_id_btn;
    UI::Button<RFID_settings_window> clear_database_btn;

    UI::BitMapButton<RFID_settings_window> home_btn;
};

RFID_settings_window::RFID_settings_window(UI::MainWindow *_main_window) : UI::Window(_main_window),
                                                                           add_tag_btn(this, {10, 10}, {80, 30}, this),
                                                                           delete_tag_by_read_btn(this, {10, 35}, {150, 55}, this),
                                                                           delete_tag_by_id_btn(this, {10, 60}, {150, 80}, this),
                                                                           clear_database_btn(this, {120, 10}, {260, 30}, this),
                                                                           home_btn(this, {283, 203}, 30, 30, this, white_repicthousebase_1484336388, white_repicthousebase_1484336388, 3, 0)
{
    this->home_btn.on_release = &RFID_settings_window::_go_home;

    this->add_tag_btn.setText("add tag");
    this->delete_tag_by_read_btn.setFont(TinyFont);
    this->delete_tag_by_read_btn.setText("delete tag by read");
    this->delete_tag_by_id_btn.setText("delete tag by id");
    this->clear_database_btn.setText("clear database");
}