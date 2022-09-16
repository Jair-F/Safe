#include "UI/RFID_Settings.hpp"
#include "RFID/RFID.hpp"

extern const unsigned short white_repicthousebase_1484336388[];
class lock_screen;
extern lock_screen *l_screen;
extern uint8_t TinyFont[];

namespace
{
    // in pixels
    constexpr uint8_t GAP_BETWEEN_BUTTONS = 10;
    constexpr uint8_t BUTTON_HEIGHT = 40;
    constexpr uint8_t BUTTON_WIDTH = 180;
    constexpr uint8_t BUTTON_BORDER_WEIGHT = 3;
}

/*
    gap between the buttons is 10px
    button width: 180px
    button height: 40px
*/
RFID_settings_window::RFID_settings_window(FGUI::MainWindow *_main_window) : FGUI::Window(_main_window),
                                                                             window_title(this, {0, 0}, 319),
                                                                             add_tag_btn(this, {70, 40}, BUTTON_WIDTH, BUTTON_HEIGHT, this),
                                                                             delete_tag_by_read_btn(this, {70, add_tag_btn.get_lower_right_pos().y_pos + GAP_BETWEEN_BUTTONS},
                                                                                                    BUTTON_WIDTH, BUTTON_HEIGHT, this),
                                                                             delete_tag_by_id_btn(this, {70, delete_tag_by_read_btn.get_lower_right_pos().y_pos + GAP_BETWEEN_BUTTONS},
                                                                                                  BUTTON_WIDTH, BUTTON_HEIGHT, this),
                                                                             clear_database_btn(this, {70, delete_tag_by_id_btn.get_lower_right_pos().y_pos + GAP_BETWEEN_BUTTONS},
                                                                                                BUTTON_WIDTH, BUTTON_HEIGHT, this),
                                                                             home_btn(this, {283, 203}, 30, 30, this, white_repicthousebase_1484336388, white_repicthousebase_1484336388, white_repicthousebase_1484336388, 3, 0),
                                                                             add_tag_wnd(this),
                                                                             info_wnd(this, 90, 70, "tag added")
{
    this->home_btn.on_release = &RFID_settings_window::_go_home;

    this->window_title.set_text_alignment(FGUI::TextLabel::text_alignment::AL_CENTER);
    this->window_title.set_font(BigFont);
    this->window_title.set_border_weight(2);
    this->window_title.released_border_color = VGA_BLUE;
    this->window_title.set_text("RFID Settings");

    this->add_tag_btn.setText("add tag");
    this->add_tag_btn.touched_background_color = VGA_GRAY;
    this->add_tag_btn.set_border_weight(BUTTON_BORDER_WEIGHT);
    this->add_tag_btn.on_release = &RFID_settings_window::_show_add_tag_wnd;

    this->delete_tag_by_id_btn.set_border_weight(BUTTON_BORDER_WEIGHT);
    this->delete_tag_by_id_btn.touched_background_color = VGA_GRAY;
    this->delete_tag_by_id_btn.setText("delete tag by id");

    this->delete_tag_by_read_btn.set_border_weight(BUTTON_BORDER_WEIGHT);
    this->delete_tag_by_read_btn.touched_background_color = VGA_GRAY;
    this->delete_tag_by_read_btn.setText("delete tag by read");

    this->clear_database_btn.set_border_weight(BUTTON_BORDER_WEIGHT);
    this->clear_database_btn.touched_background_color = VGA_GRAY;
    this->clear_database_btn.setText("clear database");
}

void RFID_settings_window::_go_home(FGUI::Touch_Widget<RFID_settings_window> *_widget)
{
    // switch to home window
    this->_get_main_window()->set_active_window(l_screen);
}

void RFID_settings_window::_show_add_tag_wnd(FGUI::Touch_Widget<RFID_settings_window> *_widget)
{
    this->show_pop_up_window(&this->add_tag_wnd);
    // this->show_pop_up_window(&info_wnd);
}

void RFID_settings_window::loop()
{

    FGUI::WindowBase::loop();
}