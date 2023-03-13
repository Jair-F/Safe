#include "UI/lock_screen.hpp"
#include "UI/RFID_Settings.hpp"
#include "UI/Settings.hpp"
#include "Lock/UNOB_handler.hpp"

extern const unsigned short settings_icon[];
extern const unsigned short info_icon[];
extern const unsigned short settings_big_icon[];

class RFID_settings_window;
extern RFID_settings_window *rfid_settings_wnd;
extern Settings_window *settings_window;
extern Lock lock;
extern Clock::Clock system_clock;
extern UNOB_handler unob_handler;

// for button

lock_screen::lock_screen(FGUI::MainWindow *_main_window) : FGUI::Window(_main_window),
                                                           window_title(this, {0, 0}, 319, "lock screen", arial_bold),
                                                           window_title_div(this, {0, 33}, 319, 3, FGUI::Divider::d_alignment::AL_HORIZONTAL, VGA_WHITE),
                                                           status_label(this, {0, 82}, 319, "add unlock_device to reader", Retro8x16),
                                                           settings_btn(this, {275, 195}, 38, 38, this, settings_big_icon, settings_big_icon, settings_big_icon),
                                                           info_btn(this, {0, 195}, 38, 38, this, info_icon, info_icon, info_icon)
{
    this->window_title.set_text_alignment(FGUI::TextLabel::text_alignment::AL_CENTER);
    this->window_title.set_draw_border(false);

    this->window_title_div.set_color(VGA_GRAY);

    this->status_label.set_text_alignment(FGUI::TextLabel::text_alignment::AL_CENTER);
    this->status_label.set_draw_border(false);
    this->status_label._focus_lose();

    this->settings_btn.released_border_color = VGA_WHITE;
    this->settings_btn.touched_background_color = VGA_FUCHSIA;
    this->settings_btn.set_border_weight(3);
    this->settings_btn.set_draw_border(false);
    this->settings_btn.on_release = &lock_screen::_open_settings;

    this->info_btn.set_draw_border(false);
    this->info_btn.on_release = &lock_screen::_open_info;
}

void lock_screen::_open_settings(FGUI::Touch_Widget<lock_screen> *_widget)
{
    this->settings_btn.set_draw_border(!this->settings_btn.get_draw_border());
    this->settings_btn.draw();

    if (this->settings_btn.get_draw_border())
    {
        this->status_label.set_text("authorize...");
    }
    else
    {
        this->status_label.set_text("add unlock_device to reader");
    }
}

void lock_screen::_open_info(FGUI::Touch_Widget<lock_screen> *_widget)
{
    // open info page
}

/**
 * send_backspace
 * make send_enter
 */

void lock_screen::send_input(char _input_data)
{
    // feed the pin object...
    auto label_text_ln = this->status_label.get_text().length();
    if (label_text_ln > 1)
        this->status_label.set_text(this->status_label.get_text().substring(0, label_text_ln - 2));
}

void lock_screen::send_backspace()
{
    auto label_text_ln = this->status_label.get_text().length();
    if (label_text_ln > 1)
        this->status_label.set_text(this->status_label.get_text().substring(0, label_text_ln - 2));
}

void lock_screen::send_enter()
{
    // push comparision in PIN unob
}

void lock_screen::loop()
{
    // true = the next read will be for authorizing to go to settings page
    if (this->settings_btn.get_draw_border())
    {
        auto unob_read_result = unob_handler.read_unobs();
        if (unob_read_result == Unlock_Object::unlock_authentication_reports::AUTHORIZED_UNLOCK_OBJECT ||
            unob_handler.unob_authorized_database_set() == false)
        {
            this->_get_main_window()->set_active_window(settings_window);
        }
        else if (unob_read_result == Unlock_Object::unlock_authentication_reports::UNAUTHORIZED_UNLOCK_OBJECT)
        {
            // lock.report_unauthorized_unlock_try();
        }
    }
    else
    {
        // lock.
    }
}
