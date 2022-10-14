#include "UI/Settings.hpp"
#include "UI/lock_screen.hpp"
#include "UI/RFID_Settings.hpp"
#include "UI/Fingerprint/Fingerprint_Settings.hpp"

extern lock_screen *l_screen;
extern RFID_settings_window *rfid_settings_wnd;
extern Fingerprint_settings_window *fingerprint_settings_wnd;

namespace
{
    // in pixels
    constexpr uint8_t HORIZONTAL_GAP_BETWEEN_BUTTONS = 15;
    constexpr uint8_t VERTICAL_GAP_BETWEEN_BUTTONS = 10;
    constexpr uint8_t BUTTON_HEIGHT = 40;
    constexpr uint8_t BUTTON_WIDTH = 135;
    constexpr uint8_t BUTTON_BORDER_WEIGHT = 3;
}

Settings_window::Settings_window(FGUI::MainWindow *_main_window) : FGUI::Window(_main_window),
                                                                   window_title(this, {0, 0}, 319),
                                                                   fingerprint_settings_btn(this, {HORIZONTAL_GAP_BETWEEN_BUTTONS, 40}, BUTTON_WIDTH, BUTTON_HEIGHT, this),
                                                                   rfid_settings_btn(this, {this->fingerprint_settings_btn.get_lower_right_pos().x_pos + HORIZONTAL_GAP_BETWEEN_BUTTONS, 40},
                                                                                     BUTTON_WIDTH, BUTTON_HEIGHT, this),

                                                                   pin_settings_btn(this, {HORIZONTAL_GAP_BETWEEN_BUTTONS, this->rfid_settings_btn.get_lower_right_pos().y_pos + VERTICAL_GAP_BETWEEN_BUTTONS},
                                                                                    BUTTON_WIDTH, BUTTON_HEIGHT, this),
                                                                   lock_settings_btn(this, {this->pin_settings_btn.get_lower_right_pos().x_pos + HORIZONTAL_GAP_BETWEEN_BUTTONS, this->pin_settings_btn.get_upper_left_pos().y_pos},
                                                                                     BUTTON_WIDTH, BUTTON_HEIGHT, this),

                                                                   system_settings_btn(this, {HORIZONTAL_GAP_BETWEEN_BUTTONS, this->lock_settings_btn.get_lower_right_pos().y_pos + VERTICAL_GAP_BETWEEN_BUTTONS},
                                                                                       BUTTON_WIDTH, BUTTON_HEIGHT, this),
                                                                   status_info_btn(this, {this->system_settings_btn.get_lower_right_pos().x_pos + HORIZONTAL_GAP_BETWEEN_BUTTONS, this->system_settings_btn.get_upper_left_pos().y_pos},
                                                                                   BUTTON_WIDTH, BUTTON_HEIGHT, this),

                                                                   fingeprint_settings(this),
                                                                   rfid_settings(this)
{
    this->window_title.set_text_alignment(FGUI::TextLabel::text_alignment::AL_CENTER);
    this->window_title.set_font(BigFont);
    this->window_title.set_border_weight(2);
    this->window_title.released_border_color = VGA_BLUE;
    this->window_title.set_text("Settings");

    this->fingerprint_settings_btn.setText("Fingeprint");
    this->fingerprint_settings_btn.touched_background_color = VGA_GRAY;
    this->fingerprint_settings_btn.set_border_weight(BUTTON_BORDER_WEIGHT);
    this->fingerprint_settings_btn.on_release = &Settings_window::_show_fingerprint_settings_window;

    this->rfid_settings_btn.setText("RFID");
    this->rfid_settings_btn.touched_background_color = VGA_GRAY;
    this->rfid_settings_btn.set_border_weight(BUTTON_BORDER_WEIGHT);
    this->rfid_settings_btn.on_release = &Settings_window::_show_rfid_settings_window;

    this->pin_settings_btn.setText("PIN");
    this->pin_settings_btn.touched_background_color = VGA_GRAY;
    this->pin_settings_btn.set_border_weight(BUTTON_BORDER_WEIGHT);
    this->pin_settings_btn.on_release = &Settings_window::_show_pin_settings_window;

    this->lock_settings_btn.setText("Lock");
    this->lock_settings_btn.touched_background_color = VGA_GRAY;
    this->lock_settings_btn.set_border_weight(BUTTON_BORDER_WEIGHT);
    this->lock_settings_btn.on_release = &Settings_window::_show_lock_settings_window;

    this->system_settings_btn.setText("System");
    this->system_settings_btn.touched_background_color = VGA_GRAY;
    this->system_settings_btn.set_border_weight(BUTTON_BORDER_WEIGHT);
    this->system_settings_btn.on_release = &Settings_window::_show_system_settings_window;

    this->status_info_btn.setText("Status Info");
    this->status_info_btn.touched_background_color = VGA_GRAY;
    this->status_info_btn.set_border_weight(BUTTON_BORDER_WEIGHT);
    this->status_info_btn.on_release = &Settings_window::_show_status_info_window;
}

void Settings_window::_go_to_lockscreen(FGUI::Touch_Widget<Settings_window> *_widget)
{
    // this->_get_main_window()->set_active_window(l_screen);
}

void Settings_window::_show_rfid_settings_window(FGUI::Touch_Widget<Settings_window> *_widget)
{
    this->_get_main_window()->set_active_window(&this->rfid_settings);
}

void Settings_window::_show_fingerprint_settings_window(FGUI::Touch_Widget<Settings_window> *_widget)
{
    this->_get_main_window()->set_active_window(&this->fingeprint_settings);
}
