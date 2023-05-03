#include "UI/Settings.hpp"
#include "UI/lock_screen.hpp"
#include "UI/RFID_Settings.hpp"
#include "UI/Fingerprint/Fingerprint_Settings.hpp"

#include "Config.hpp"
#include "logging/Log.hpp"

extern lock_screen *l_screen;
extern RFID_settings_window *rfid_settings_wnd;
extern Fingerprint_settings_window *fingerprint_settings_wnd;
extern PIN_Settings *pin_settings_wnd;

extern Log::Log logger;

extern const unsigned short back_icon[];
extern const unsigned short save_icon[];

Settings_window::Settings_window(FGUI::WindowBase *_parent_window) : FGUI::Window(_parent_window),
                                                                     window_title(this, {0, 0}, 319, "Settings", 0),
                                                                     div(this, {0, 43}, 319, 3, FGUI::Divider::d_alignment::AL_HORIZONTAL, VGA_BLUE),
                                                                     back_btn(this, {0, 0}, 40, 40, this, back_icon, (uint8_t)0, (uint8_t)0),
                                                                     save_btn(this, {279, 0}, 36, 36, this, save_icon, (uint8_t)2, (uint8_t)0),
                                                                     rfid_settings_btn(this, {12, 54}, 130, 45, this),
                                                                     fingerprint_settings_btn(this, {177, 54}, 130, 45, this),
                                                                     pin_settings_btn(this, {12, 119}, 130, 45, this),
                                                                     lock_settings_btn(this, {177, 119}, 130, 45, this),
                                                                     system_settings_btn(this, {12, 184}, 130, 45, this),
                                                                     status_info_btn(this, {177, 184}, 130, 45, this),
                                                                     fingeprint_settings(this),
                                                                     rfid_settings(this),
                                                                     pin_settings_wnd(this),
                                                                     lock_settings(this),
                                                                     system_settings(this)
{
    this->window_title.set_text_alignment(FGUI::TextLabel::text_alignment::AL_CENTER);
    this->window_title.set_font(BigFont);
    this->window_title.set_draw_border(false);
    this->window_title.set_content_border_gap(12); // shift the text a bit lower

    this->fingerprint_settings_btn.setText("Fingeprint");
    this->fingerprint_settings_btn.touched_background_color = VGA_GRAY;
    this->fingerprint_settings_btn.set_border_weight(2);
    this->fingerprint_settings_btn.on_release = &Settings_window::_show_fingerprint_settings_window;

    this->rfid_settings_btn.setText("RFID");
    this->rfid_settings_btn.touched_background_color = VGA_GRAY;
    this->rfid_settings_btn.set_border_weight(2);
    this->rfid_settings_btn.on_release = &Settings_window::_show_rfid_settings_window;

    this->pin_settings_btn.setText("PIN");
    this->pin_settings_btn.touched_background_color = VGA_GRAY;
    this->pin_settings_btn.set_border_weight(2);
    this->pin_settings_btn.on_release = &Settings_window::_show_pin_settings_window;

    this->lock_settings_btn.setText("Lock");
    this->lock_settings_btn.touched_background_color = VGA_GRAY;
    this->lock_settings_btn.set_border_weight(2);
    this->lock_settings_btn.on_release = &Settings_window::_show_lock_settings_window;

    this->system_settings_btn.setText("System");
    this->system_settings_btn.touched_background_color = VGA_GRAY;
    this->system_settings_btn.set_border_weight(2);
    this->system_settings_btn.on_release = &Settings_window::_show_system_settings_window;

    this->status_info_btn.setText("Status Info");
    this->status_info_btn.touched_background_color = VGA_GRAY;
    this->status_info_btn.set_border_weight(2);
    this->status_info_btn.on_release = &Settings_window::_show_status_info_window;

    this->back_btn.set_draw_border(false);
    this->back_btn.set_border_weight(0);
    this->back_btn.on_release = &Settings_window::_go_to_lockscreen;
    this->back_btn.released_border_color = VGA_TRANSPARENT;

    this->save_btn.on_release = &Settings_window::_handle_save_config;
    this->save_btn.released_border_color = VGA_BLACK;
    this->set_draw_border(true);
}

void Settings_window::_go_to_lockscreen(FGUI::Touch_Widget<Settings_window> *_widget)
{
    this->_get_main_window()->set_active_window(l_screen);
}

void Settings_window::_show_rfid_settings_window(FGUI::Touch_Widget<Settings_window> *_widget)
{
    this->_get_main_window()->set_active_window(&this->rfid_settings);
}

void Settings_window::_show_fingerprint_settings_window(FGUI::Touch_Widget<Settings_window> *_widget)
{
    this->_get_main_window()->set_active_window(&this->fingeprint_settings);
}

void Settings_window::_show_pin_settings_window(FGUI::Touch_Widget<Settings_window> *_widget)
{
    this->_get_main_window()->set_active_window(&pin_settings_wnd);
}

void Settings_window::_show_lock_settings_window(FGUI::Touch_Widget<Settings_window> *_widget)
{
    this->_get_main_window()->set_active_window(&lock_settings);
}

void Settings_window::_show_system_settings_window(FGUI::Touch_Widget<Settings_window> *_widget)
{
    this->_get_main_window()->set_active_window(&system_settings);
}

void Settings_window::_handle_save_config(FGUI::Touch_Widget<Settings_window> *_widget)
{
    String config_str;
    Config::create_config_str(config_str);
    bool success = Config::write_config(&config_str);
    if (!success)
    {
        save_btn.released_border_color = VGA_RED;
        logger.log("[CONFIG] failed to save the config", Log::log_level::L_CRITICAL);
    }
    else
    {
        save_btn.released_border_color = VGA_GREEN;
        logger.log("[CONFIG] saved config", Log::log_level::L_DEBUG);
    }
    this->save_btn.draw();
}

void Settings_window ::_pre_show()
{
    this->save_btn.released_border_color = VGA_BLACK;
}
