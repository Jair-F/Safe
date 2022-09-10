#include "UI/rfid_settings_window.hpp"

extern const unsigned short white_repicthousebase_1484336388[];
class lock_screen;
extern lock_screen *l_screen;
extern uint8_t TinyFont[];

void RFID_settings_window::_go_home(FGUI::Touch_Widget<RFID_settings_window> *_widget)
{
    // switch to home window
    this->_get_main_window()->set_active_window(l_screen);
}

RFID_settings_window::RFID_settings_window(FGUI::MainWindow *_main_window) : FGUI::Window(_main_window),
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