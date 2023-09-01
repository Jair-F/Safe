#include "UI/lock_screen.hpp"
#include "UI/RFID_Settings.hpp"
#include "UI/Settings.hpp"
#include "UI/info_screen.hpp"

#include "Lock/UNOB_handler.hpp"
#include "Pin.hpp"

extern const unsigned short settings_icon[];
extern const unsigned short info_icon[];
extern const unsigned short settings_big_icon[];

extern RFID_settings_window *rfid_settings_wnd;
extern Settings_window *settings_window;
extern Info_Screen *info_screen;

extern Lock lock;
extern Clock::Clock system_clock;
extern UNOB_handler unob_handler;
extern Pin pin;

// for button

lock_screen::lock_screen(FGUI::MainWindow *_main_window) : FGUI::Window(_main_window),
                                                           window_title(this, {0, 0}, 300, "lock screen", arial_bold),
                                                           window_title_div(this, {0, 33}, 319, 3, FGUI::Divider::d_alignment::AL_HORIZONTAL, VGA_WHITE),
                                                           status_label(this, {0, 82}, 319, "unlock...", Retro8x16),
                                                           settings_btn(this, {275, 195}, 38, 38, this, settings_big_icon, settings_big_icon, settings_big_icon),
                                                           info_btn(this, {0, 193}, 38, 38, this, info_icon, 2, 2)
{
    this->window_title.set_text_alignment(FGUI::TextLabel::text_alignment::AL_CENTER);
    this->window_title.set_draw_border(false);

    this->window_title_div.set_color(VGA_GRAY);

    this->status_label.set_font(arial_bold);
    this->status_label.set_text_alignment(FGUI::TextLabel::text_alignment::AL_CENTER);
    this->status_label.set_draw_border(false);
    this->status_label._focus_lose();

    this->settings_btn.released_border_color = VGA_TEAL;
    this->settings_btn.touched_background_color = VGA_FUCHSIA;
    this->settings_btn.set_draw_border(false);
    this->settings_btn.on_release = &lock_screen::_request_open_settings;

    this->info_btn.released_border_color = VGA_BLACK;
    this->info_btn.set_draw_border(true);
    this->info_btn.on_release = &lock_screen::_open_info;

    Serial.print("info_btn: ");
    Serial.print(this->info_btn.get_bitmaps_width());
    Serial.print('x');
    Serial.print(this->info_btn.get_bitmaps_height());
    Serial.println();
    Serial.print("upper_left: ");
    Serial.print(this->info_btn.get_upper_left_pos().x_pos);
    Serial.print('x');
    Serial.print(this->info_btn.get_upper_left_pos().y_pos);
    Serial.println();
    Serial.print("lower_right: ");
    Serial.print(this->info_btn.get_lower_right_pos().x_pos);
    Serial.print('x');
    Serial.print(this->info_btn.get_lower_right_pos().y_pos);
    Serial.println();
    Serial.println();
}

void lock_screen::_request_open_settings(FGUI::Touch_Widget<lock_screen> *_widget)
{
    this->settings_btn.set_draw_border(!this->settings_btn.get_draw_border());
    this->settings_btn.draw();

    this->status_label.released_text_color = VGA_WHITE;
    if (this->settings_btn.has_border())
    {
        this->status_label.set_text("authorize...");
    }
    else
    {
        this->status_label.set_text("unlock...");
    }
    this->status_label.draw();
}

void lock_screen::_open_info(FGUI::Touch_Widget<lock_screen> *_widget)
{
    // open info page
    this->_get_main_window()->set_active_window(info_screen);
}

void lock_screen::_show_pin_input()
{
    String tmp = "";
    for (uint8_t i = 0; i < pin.get_input_buffer().length(); ++i)
        tmp += '*';

    this->status_label.released_text_color = VGA_WHITE;
    this->status_label.set_text(tmp);
    this->status_label.draw();
}

void lock_screen::_pre_hide()
{
    lock.lock(); // lock the lock if its not already locked...
}

/**
 * send_backspace
 * make send_enter
 */

void lock_screen::send_input(char _input_data)
{
    // feed the pin object...
    pin.append(_input_data);
    this->_show_pin_input();
}

void lock_screen::send_backspace()
{
    pin.pop_back();
    this->_show_pin_input();
}

/*
void lock_screen::send_enter()
{
    // if at waiting to authorize to enter settings_window dont open lock
    if (this->settings_btn.has_border())
    {
        if (pin._check_input_buffer())
        {
            // open the settings window
            this->_get_main_window()->set_active_window(settings_window);
        }
        else
        {
            lock.report_unauthorized_unlock_try();
        }
    }
}
*/

bool lock_screen::_waiting_access_settings() const
{
    return this->settings_btn.has_border();
}

void lock_screen::loop()
{
    // true = the next read will be for authorizing to go to settings page
    if (this->_waiting_access_settings())
    {
        auto unob_read_result = unob_handler.read_unobs();
        if (unob_read_result == Unlock_Object::unlock_authentication_reports::AUTHORIZED_UNLOCK_OBJECT ||
            unob_handler.unob_authorized_database_set() == false)
        {
            this->status_label.set_text("");
            pin.clear_input_buffer(); // otherwise it would jump next time straight into settings
            this->_get_main_window()->set_active_window(settings_window);
            this->settings_btn.set_draw_border(false);
        }
        else if (unob_read_result == Unlock_Object::unlock_authentication_reports::UNAUTHORIZED_UNLOCK_OBJECT)
        {
            this->status_label.released_text_color = VGA_RED;
            this->status_label.set_text("sorry, try again...");
            this->status_label.draw();
            lock.report_unauthorized_unlock_try();
            pin.clear_input_buffer(); // to not report in loop
        }
        else if (unob_read_result == Unlock_Object::unlock_authentication_reports::NO_UNLOCK_OBJECT_PRESENT)
        {
        }
    }
    else
    {
        lock.loop();
    }
}
