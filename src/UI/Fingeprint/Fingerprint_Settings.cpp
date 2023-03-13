#include "UI/Fingerprint/Fingerprint_Settings.hpp"
#include "UI/lock_screen.hpp"
#include "UI/Settings.hpp"
#include "Fingerprint.hpp"

extern Fingerprint::Fingerprint *fingerprint;
extern lock_screen *l_screen;
extern Settings_window *settings_window;

extern const unsigned short settings_icon[];
extern const unsigned short info_icon[];

namespace
{
    // in pixels
    constexpr uint8_t GAP_BETWEEN_BUTTONS = 10;
    constexpr uint8_t BUTTON_HEIGHT = 40;
    constexpr uint8_t BUTTON_WIDTH = 220;
    constexpr uint8_t BUTTON_BORDER_WEIGHT = 3;
}

Fingerprint_settings_window::Fingerprint_settings_window(FGUI::WindowBase *_parent_window) : FGUI::Window(_parent_window),
                                                                                             window_title(this, {0, 0}, 319),
                                                                                             add_finger_btn(this, {50, 40}, BUTTON_WIDTH, BUTTON_HEIGHT, this),
                                                                                             delete_finger_by_read_btn(this, {50, add_finger_btn.get_lower_right_pos().y_pos + GAP_BETWEEN_BUTTONS},
                                                                                                                       BUTTON_WIDTH, BUTTON_HEIGHT, this),
                                                                                             delete_finger_by_id_btn(this, {50, delete_finger_by_read_btn.get_lower_right_pos().y_pos + GAP_BETWEEN_BUTTONS},
                                                                                                                     BUTTON_WIDTH, BUTTON_HEIGHT, this),
                                                                                             clear_database_btn(this, {50, delete_finger_by_id_btn.get_lower_right_pos().y_pos + GAP_BETWEEN_BUTTONS},
                                                                                                                BUTTON_WIDTH, BUTTON_HEIGHT, this),
                                                                                             home_btn(this, {283, 203}, 30, 30, this, settings_icon, settings_icon, settings_icon, 3, 0),

                                                                                             add_finger_by_read(this)
{
    this->home_btn.on_release = &Fingerprint_settings_window::_exit_window;

    this->window_title.set_text_alignment(FGUI::TextLabel::text_alignment::AL_CENTER);
    this->window_title.set_font(BigFont);
    this->window_title.set_border_weight(2);
    this->window_title.released_border_color = VGA_BLUE;
    this->window_title.set_text("Fingerprint Settings");

    this->add_finger_btn.setText("add fingerprint");
    this->add_finger_btn.touched_background_color = VGA_GRAY;
    this->add_finger_btn.set_border_weight(BUTTON_BORDER_WEIGHT);
    this->add_finger_btn.on_release = &Fingerprint_settings_window::_show_add_finger_window;

    this->delete_finger_by_id_btn.set_border_weight(BUTTON_BORDER_WEIGHT);
    this->delete_finger_by_id_btn.touched_background_color = VGA_GRAY;
    this->delete_finger_by_id_btn.on_release = &Fingerprint_settings_window::_show_delete_finger_by_id;
    this->delete_finger_by_id_btn.setText("delete fingerprint by id");

    this->delete_finger_by_read_btn.set_border_weight(BUTTON_BORDER_WEIGHT);
    this->delete_finger_by_read_btn.touched_background_color = VGA_GRAY;
    this->delete_finger_by_read_btn.setText("delete fingerprint by read");

    this->clear_database_btn.set_border_weight(BUTTON_BORDER_WEIGHT);
    this->clear_database_btn.touched_background_color = VGA_GRAY;
    this->clear_database_btn.setText("clear database");
    this->clear_database_btn.on_release = &Fingerprint_settings_window::_show_clear_database_window;
}

void Fingerprint_settings_window::_show_add_finger_window(FGUI::Touch_Widget<Fingerprint_settings_window> *_widget)
{
    // switch to home window
    this->_get_main_window()->set_active_window(&this->add_finger_by_read);
}

void Fingerprint_settings_window::_exit_window(FGUI::Touch_Widget<Fingerprint_settings_window> *_widget)
{
    // switch to home window
    this->_get_main_window()->set_active_window(this->_get_parent_window());
}