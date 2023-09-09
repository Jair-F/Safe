#include "UI/System_Settings.hpp"

#include "system_clock/system_clock.hpp"
#include <ArduinoJson.h>
#include "Config.hpp"

extern FGUI::MainWindow m_window;
extern Clock::Clock system_clock;
extern StaticJsonDocument<1024> config;
extern Log::Log logger;

extern const unsigned short back_33_33[];

System_Settings::System_Settings(FGUI::WindowBase *_parent_window) : FGUI::Window(_parent_window),
                                                                     window_title(this, {33, 0}, 286, "System", arial_bold, 0, 0),
                                                                     back_btn(this, {0, 0}, 33, 33, this, back_33_33, (uint8_t)0, (uint8_t)0),
                                                                     div(this, {0, 34}, 319, 3, FGUI::Divider::d_alignment::AL_HORIZONTAL, VGA_BLUE),
                                                                     logging_level_label(this, {5, 47}, 84, "logging\nlevel", SmallFont, 0, 0),
                                                                     logging_level_sslct(this, {82, 54}, 65, 20, this, 2, 2),
                                                                     screen_timeout_label(this, {5, 97}, 105, "screen\ntimeout(s)", SmallFont, 0, 0),
                                                                     screen_timeout_input(this, {119, 106}, 35, 25, this, FGUI::InputField<3, System_Settings>::IN_INPUT_TYPE::IN_TEXT, 2),
                                                                     reset_config_btn(this, {27, 155}, 105, 28, this, 2, 0),
                                                                     window_div(this, {159, 45}, 144, 3, FGUI::Divider::d_alignment::AL_VERTICAL, VGA_WHITE),
                                                                     time_label(this, {165, 60}, 51, "time", SmallFont, 0, 0),
                                                                     time_h_input(this, {216, 60}, 25, 25, this, FGUI::InputField<2U, System_Settings>::IN_INPUT_TYPE::IN_TEXT, 0),
                                                                     time_spacer1(this, {241, 43}, 12, ":", SmallFont, 0, 0),
                                                                     time_m_input(this, {253, 60}, 25, 25, this, FGUI::InputField<2U, System_Settings>::IN_INPUT_TYPE::IN_TEXT, 0),
                                                                     time_spacer2(this, {278, 43}, 12, ":", SmallFont, 0, 0),
                                                                     time_s_input(this, {290, 60}, 25, 25, this, FGUI::InputField<2U, System_Settings>::IN_INPUT_TYPE::IN_TEXT, 0),
                                                                     date_label(this, {165, 108}, 51, "date", SmallFont, 0, 0),
                                                                     date_d_input(this, {216, 106}, 25, 25, this, FGUI::InputField<2U, System_Settings>::IN_INPUT_TYPE::IN_TEXT, 0),
                                                                     date_spacer1(this, {238, 90}, 12, "/", SmallFont, 0, 0),
                                                                     date_m_input(this, {253, 106}, 25, 25, this, FGUI::InputField<2U, System_Settings>::IN_INPUT_TYPE::IN_TEXT, 0),
                                                                     date_spacer2(this, {275, 90}, 12, "/", SmallFont, 0, 0),
                                                                     date_y_input(this, {290, 106}, 25, 25, this, FGUI::InputField<2U, System_Settings>::IN_INPUT_TYPE::IN_TEXT, 0),
                                                                     wday_label(this, {165, 154}, 98, "weekday", SmallFont, 0, 0),
                                                                     wday_input(this, {280, 154}, 25, 25, this, FGUI::InputField<1U, System_Settings>::IN_INPUT_TYPE::IN_TEXT, 0),
                                                                     save_btn(this, {110, 201}, 100, 28, this, 2, 0)
{
    this->window_title.set_text_alignment(FGUI::TextLabel::text_alignment::AL_CENTER);

    this->back_btn.on_release = &System_Settings::_exit_window;

    this->logging_level_sslct.add_entry("DEBUG");
    this->logging_level_sslct.add_entry("INFO");
    this->logging_level_sslct.add_entry("WARNING");
    this->logging_level_sslct.add_entry("ERROR");
    this->logging_level_sslct.add_entry("CRITICAL");
    this->logging_level_sslct.set_selection("DEBUG");
    this->logging_level_sslct.add_scroll_forward_key('6');
    this->logging_level_sslct.add_scroll_forward_key('8');
    this->logging_level_sslct.add_scroll_backward_key('4');
    this->logging_level_sslct.add_scroll_backward_key('2');

    this->screen_timeout_input.set_input_buffer(String(m_window.get_fall_aspleep_timer()));

    this->reset_config_btn.setText("reset config");
    this->reset_config_btn.on_release = &System_Settings::_handle_reset_config;

    auto now = system_clock.now();
    this->time_h_input.set_input_buffer(String(now.hour));
    this->time_h_input.set_border_weight(1);
    this->time_h_input.released_border_color = VGA_RED;
    this->time_h_input.touched_border_color = VGA_RED;
    this->time_h_input.set_draw_border(false);
    this->time_h_input.on_typing = &System_Settings::_handle_on_type_hour;

    this->time_m_input.set_input_buffer(String(now.minute));
    this->time_m_input.set_border_weight(1);
    this->time_m_input.released_border_color = VGA_RED;
    this->time_m_input.touched_border_color = VGA_RED;
    this->time_m_input.set_draw_border(false);
    this->time_m_input.on_typing = &System_Settings::_handle_on_type_minute;

    this->time_s_input.set_input_buffer(String(now.second));
    this->time_s_input.set_border_weight(1);
    this->time_s_input.released_border_color = VGA_RED;
    this->time_s_input.touched_border_color = VGA_RED;
    this->time_s_input.set_draw_border(false);
    this->time_s_input.on_typing = &System_Settings::_handle_on_type_second;

    this->date_d_input.set_input_buffer(String(now.m_day));
    this->date_d_input.set_border_weight(1);
    this->date_d_input.released_border_color = VGA_RED;
    this->date_d_input.touched_border_color = VGA_RED;
    this->date_d_input.set_draw_border(false);
    this->date_d_input.on_typing = &System_Settings::_handle_on_type_day;

    this->date_m_input.set_input_buffer(String(now.month));
    this->date_m_input.set_border_weight(1);
    this->date_m_input.released_border_color = VGA_RED;
    this->date_m_input.touched_border_color = VGA_RED;
    this->date_m_input.set_draw_border(false);
    this->date_m_input.on_typing = &System_Settings::_handle_on_type_month;

    this->date_y_input.set_input_buffer(String(now.year));
    this->date_y_input.set_border_weight(1);
    this->date_y_input.released_border_color = VGA_RED;
    this->date_y_input.touched_border_color = VGA_RED;
    this->date_y_input.set_draw_border(false);
    this->date_y_input.on_typing = &System_Settings::_handle_on_type_year;

    this->wday_input.set_input_buffer(String(now.w_day));
    this->wday_input.set_border_weight(1);
    this->wday_input.released_border_color = VGA_RED;
    this->wday_input.touched_border_color = VGA_RED;
    this->wday_input.set_draw_border(false);
    this->wday_input.on_typing = &System_Settings::_handle_on_type_wday;

    this->save_btn.setText("save");
    this->save_btn.on_release = &System_Settings::_handle_save;
}

void System_Settings::_exit_window(FGUI::Touch_Widget<System_Settings> *_widget)
{
    this->_get_main_window()->set_active_window(this->_get_parent_window());
}

void System_Settings::_handle_on_type_hour(FGUI::Touch_Widget<System_Settings> *_widget, const char *_input_buffer)
{
    if (strlen(_input_buffer) == 2)
        this->_get_main_window()->request_focus(&this->time_m_input);

    uint8_t _hour = String(_input_buffer).toInt();
    if (_hour > 24)
        _widget->set_draw_border(true);
    else
        _widget->set_draw_border(false);
    _widget->draw();
}

void System_Settings::_handle_on_type_minute(FGUI::Touch_Widget<System_Settings> *_widget, const char *_input_buffer)
{
    if (strlen(_input_buffer) == 2)
        this->_get_main_window()->request_focus(&this->time_s_input);
    else if (strlen(_input_buffer) == 0)
        this->_get_main_window()->request_focus(&this->time_h_input);

    uint8_t _minute = String(_input_buffer).toInt();
    if (_minute > 59)
        _widget->set_draw_border(true);
    else
        _widget->set_draw_border(false);
    _widget->draw();
}

void System_Settings::_handle_on_type_second(FGUI::Touch_Widget<System_Settings> *_widget, const char *_input_buffer)
{
    if (strlen(_input_buffer) == 0)
        this->_get_main_window()->request_focus(&this->time_m_input);

    uint8_t _second = String(_input_buffer).toInt();
    if (_second > 59)
        _widget->set_draw_border(true);
    else
        _widget->set_draw_border(false);
    _widget->draw();
}

void System_Settings::_handle_on_type_day(FGUI::Touch_Widget<System_Settings> *_widget, const char *_input_buffer)
{
    if (strlen(_input_buffer) == 2)
        this->_get_main_window()->request_focus(&this->date_m_input);

    this->_check_date();
}

void System_Settings::_handle_on_type_month(FGUI::Touch_Widget<System_Settings> *_widget, const char *_input_buffer)
{
    if (strlen(_input_buffer) == 2)
        this->_get_main_window()->request_focus(&this->date_y_input);
    else if (strlen(_input_buffer) == 0)
        this->_get_main_window()->request_focus(&this->date_d_input);

    this->_check_date();
}

void System_Settings::_handle_on_type_year(FGUI::Touch_Widget<System_Settings> *_widget, const char *_input_buffer)
{
    if (strlen(_input_buffer) == 0)
        this->_get_main_window()->request_focus(&this->date_m_input);

    this->_check_date(); // in case we are in month February and change leap year

    _widget->draw();
}

void System_Settings::_check_date()
{
    uint8_t _day = String(this->date_d_input.get_input_buffer()).toInt();
    uint8_t _month = String(this->date_m_input.get_input_buffer()).toInt();
    uint8_t _year = String(this->date_y_input.get_input_buffer()).toInt();

    if (_month <= 12 && _month >= 1)
        this->date_m_input.set_draw_border(false);
    else
        this->date_m_input.set_draw_border(true);

    this->date_d_input.set_draw_border(false);
    switch (_month)
    {
    case 1:  // January
    case 3:  // March
    case 5:  // May
    case 7:  // July
    case 8:  // August
    case 10: // October
    case 12: // December
    {
        if (_day > 31 || _day < 1)
            this->date_d_input.set_draw_border(true);
        break;
    }

    case 2: // February
    {
        if (_year % 4 != 0 && // not a leap year
            (_day > 28 || _day < 1))
            this->date_d_input.set_draw_border(true);
        else if (_year % 4 == 0 && // leap year
                 _day > 29 && _day < 1)
            this->date_d_input.set_draw_border(true);
        break;
    }

    case 4:  // April
    case 6:  // June
    case 9:  // September
    case 11: // November
    {
        if (_day > 30 || _day < 1)
            this->date_d_input.set_draw_border(true);
        break;
    }

    default:
    {
        this->date_d_input.set_draw_border(true);
        break;
    }
    }

    this->date_d_input.draw();
    this->date_m_input.draw();
}

void System_Settings::_handle_on_type_wday(FGUI::Touch_Widget<System_Settings> *_widget, const char *_input_buffer)
{
    uint8_t _wday = String(_input_buffer).toInt();
    if (_wday <= 7 && _wday >= 1)
        _widget->set_draw_border(false);
    else
        _widget->set_draw_border(true);
    _widget->draw();
}

void System_Settings::_handle_reset_config(FGUI::Touch_Widget<System_Settings> *_widget)
{
    String reset_config_str = Config::reset_config();

    if (Config::write_config(&reset_config_str))
        this->reset_config_btn.released_border_color = VGA_GREEN;
    else
        this->reset_config_btn.released_border_color = VGA_RED;

    this->reset_config_btn.draw();

    /**
     * restart the DUE - like pressing the reset button on the board
     * https://forum.arduino.cc/t/due-software-reset/332764/11
     */
    rstc_start_software_reset(RSTC);
}

void System_Settings::_handle_save(FGUI::Touch_Widget<System_Settings> *_widget)
{
    bool success = true;

    byte logging_level;
    String loggin_level_str = this->logging_level_sslct.get_selection();

    if (loggin_level_str == "CRITICAL")
        logging_level = Log::log_level::L_CRITICAL;
    else if (loggin_level_str == "DEBUG")
        logging_level = Log::log_level::L_DEBUG;
    else if (loggin_level_str == "ERROR")
        logging_level = Log::log_level::L_ERROR;
    else if (loggin_level_str == "INFO")
        logging_level = Log::log_level::L_INFO;
    else if (loggin_level_str == "WARNING")
        logging_level = Log::log_level::L_WARNING;

    logger.set_logging_level(logging_level);

    this->_get_main_window()->set_fall_asleep_timer(String(this->screen_timeout_input.get_input_buffer()).toInt());

    Clock::time_point now;
    now.hour = String(this->time_h_input.get_input_buffer()).toInt();
    now.minute = String(this->time_m_input.get_input_buffer()).toInt();
    now.second = String(this->time_s_input.get_input_buffer()).toInt();
    now.m_day = String(this->date_d_input.get_input_buffer()).toInt();
    now.month = String(this->date_m_input.get_input_buffer()).toInt();
    now.year = String(this->date_y_input.get_input_buffer()).toInt();
    now.w_day = String(this->wday_input.get_input_buffer()).toInt();

    success = system_clock.set_date_time(now) == false ? false : success; // set the value only if it failed

    if (success)
        save_btn.released_border_color = VGA_GREEN;
    else
        save_btn.released_border_color = VGA_RED;
    save_btn.draw();
}

void System_Settings::_pre_show()
{
    String logging_level_str;
    switch (logger.get_logging_level())
    {
    case Log::log_level::L_CRITICAL:
        logging_level_str = "CRITICAL";
        break;
    case Log::log_level::L_DEBUG:
        logging_level_str = "DEBUG";
        break;
    case Log::log_level::L_ERROR:
        logging_level_str = "ERROR";
        break;
    case Log::log_level::L_INFO:
        logging_level_str = "INFO";
        break;
    case Log::log_level::L_WARNING:
        logging_level_str = "WARNING";
        break;
    }
    this->logging_level_sslct.set_selection(logging_level_str);

    this->screen_timeout_input.set_input_buffer(String(this->_get_main_window()->get_fall_aspleep_timer()));

    auto now = system_clock.now();
    this->time_h_input.set_input_buffer(String(now.hour));
    this->time_m_input.set_input_buffer(String(now.minute));
    this->time_s_input.set_input_buffer(String(now.second));

    this->date_d_input.set_input_buffer(String(now.m_day));
    this->date_m_input.set_input_buffer(String(now.month));
    this->date_y_input.set_input_buffer(String(now.year));

    this->wday_input.set_input_buffer(String(now.w_day));

    this->reset_config_btn.released_border_color = VGA_WHITE;
    this->save_btn.released_border_color = VGA_WHITE;
}

void System_Settings::_pre_hide()
{
    this->save_btn.released_border_color = VGA_WHITE;
}
