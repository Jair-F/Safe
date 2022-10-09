#include "UI/lock_screen.hpp"
#include "UI/RFID_Settings.hpp"
#include "UI/Settings.hpp"

extern const unsigned short settings_white[];
extern const unsigned short back_sign_white[];
extern const unsigned short settings_black[];
extern const unsigned short white_repicthousebase_1484336388[];
class RFID_settings_window;
extern RFID_settings_window *rfid_settings_wnd;
extern Settings_window *settings_wnd;

// for button
FGUI::position upper_left{20, 40};
FGUI::position lower_right{130, 70};

lock_screen::lock_screen(FGUI::MainWindow *_main_window) : FGUI::Window(_main_window),
                                                           _button(this, ::upper_left, ::lower_right, this),
                                                           _button2(this, {this->_get_display()->getDisplayXSize() - 100, this->_get_display()->getDisplayYSize() - 35}, {this->_get_display()->getDisplayXSize() - 1, this->_get_display()->getDisplayYSize() - 1}, this),
                                                           text_feld(this, {0, 0}, this->_get_display()->getDisplayXSize() - 1, "Window\n title"),
                                                           ch_box(this, {35, 140}, 30, this),
                                                           input_field(this, {150, 150}, 60, 35, this, FGUI::InputField<20, lock_screen>::IN_INPUT_TYPE::IN_TEXT),
                                                           pop_up_window(this, 150, 100, 3),
                                                           status_bar(this, {0, this->_get_display()->getDisplayYSize() - 35}, this->_get_display()->getDisplayXSize() - 110, "ERROR"),
                                                           b_button(this, {160, 32}, 30, 30, this, white_repicthousebase_1484336388, white_repicthousebase_1484336388, white_repicthousebase_1484336388, 5, 10),
                                                           div(this, {5, 100}, 50, 4, FGUI::Divider::d_alignment::AL_VERTICAL, VGA_WHITE),
                                                           progressBar(this, {80, 100}, {290, 115}, 2),
                                                           u_giff(this, {225, 125}, 50, 50, 3, 2, 1000, 2, settings_white, settings_black),
                                                           p_button(&pop_up_window, {10, 20}, {100, 45}, this),
                                                           selction_menu(this, {225, 50}, {291, 72}, this)
{

    text_feld.set_draw_border(true);
    text_feld.set_text_alignment(text_feld.AL_CENTER);
    text_feld.set_font(SmallFont);
    text_feld.set_border_weight(3);

    _button.setText("Click mich");
    _button.set_border_weight(3);
    _button.touched_border_color = VGA_BLUE;
    _button.touched_background_color = VGA_GREEN;
    _button.on_touch = &lock_screen::button_print_clicked;
    _button.on_release = &lock_screen::button_print_released;

    _button2.on_release = &lock_screen::restart_system;
    _button2.setText("Restart");

    ch_box.on_release = &lock_screen::_handle_check_box;
    ch_box.normal_check_sign_color = VGA_WHITE;
    ch_box.released_background_color = VGA_GREEN;
    ch_box.touched_background_color = VGA_GREEN;
    ch_box.released_border_color = VGA_AQUA;
    ch_box.touched_border_color = VGA_RED;
    ch_box.set_border_weight(3);
    ch_box.set_checked(true);

    input_field.set_input_buffer("123");
    input_field.set_border_weight(4);
    input_field.touched_background_color = VGA_RED;
    input_field.on_enter = &lock_screen::update_window_label;
    input_field.on_focus_lose = &lock_screen::update_window_label;
    input_field.on_typing = &lock_screen::input_field_adjust;
    // Serial.println(this->_get_main_window()->request_focus(&input_field) == true ? "focus request successfull" : "focus request failed");

    pop_up_window.set_background_color(VGA_GRAY);
    pop_up_window.set_border_color(VGA_YELLOW);

    p_button.setText("uncheck_box");
    p_button.released_text_color = VGA_WHITE;
    p_button.set_border_weight(2);
    p_button.released_border_color = VGA_BLUE;
    p_button.on_release = &lock_screen::uncheck_check_box_pop_up;

    status_bar.set_text("ERROR");
    status_bar.set_text_alignment(FGUI::TextLabel::text_alignment::AL_CENTER);
    status_bar.set_draw_border(true);
    status_bar.set_font(SmallFont);

    b_button.on_release = &lock_screen::button_print_clicked;
    b_button.touched_border_color = VGA_GREEN;
    b_button.set_draw_border(true);

    div.set_draw_border(true);
    div.set_border_weight(2);
    div.released_border_color = VGA_GREEN;

    u_giff.released_background_color = VGA_GREEN;

    this->selction_menu.add_entry("INFO");
    this->selction_menu.add_entry("DEBUG");
    this->selction_menu.add_entry("WARNING");
    this->selction_menu.add_entry("ERROR");
    this->selction_menu.add_entry("CRITICAL");
    this->selction_menu.add_scroll_forward_key('6');
    this->selction_menu.add_scroll_forward_key('8');
    this->selction_menu.add_scroll_backward_key('4');
    this->selction_menu.add_scroll_backward_key('2');
    this->selction_menu.set_selection("WARNING");
}

void lock_screen::uncheck_check_box_pop_up(FGUI::Touch_Widget<lock_screen> *_widget)
{
    this->ch_box.set_checked(!this->ch_box.is_checked());
    this->hide_pop_up_window();
    this->_get_main_window()->set_active_window(settings_wnd);
}
void lock_screen::update_status_bar(FGUI::Touch_Widget<lock_screen> *_widget)
{
    this->status_bar.set_text(this->input_field.get_input_buffer());
}

void lock_screen::button_print_clicked(FGUI::Touch_Widget<lock_screen> *_widget)
{
    // Serial.println("Clicked button");
    text_feld.set_text("button clicked");
    this->div.hide();
    if (this->div.height() < 50)
    {
        this->div.set_size(this->div.width(), this->div.height() + 10);
    }
    else
    {
        this->div.set_size(this->div.width(), this->div.height() - 10);
    }
    this->div.show();

    String tmp = this->input_field.get_input_buffer();
    uint8_t step = tmp.toInt();
    if (this->progressBar.get_progress() >= 100)
    {
        this->progressBar.set_progress(0);
    }
    this->progressBar.set_progress(this->progressBar.get_progress() + step);
}

void lock_screen::button_print_released(FGUI::Touch_Widget<lock_screen> *_widget)
{
    // FGUI::Button *but_ptr = static_cast<FGUI::Button *>(widget);
    // Serial.println("Released button");
    text_feld.set_text("button is released");
    this->show_pop_up_window(&pop_up_window);
}

void lock_screen::_handle_check_box(FGUI::Touch_Widget<lock_screen> *_widget)
{
    FGUI::CheckBox<lock_screen> *box = static_cast<FGUI::CheckBox<lock_screen> *>(_widget);
    // Serial.println("Checkbox is released");
    if (box->is_checked())
    {
        // Serial.println("Checkbox is checked");
        this->status_bar.set_text(this->input_field.get_input_buffer());
    }
    else
    {
        // Serial.println("Checkbox isnt checked");
        this->status_bar.set_text("Checkbox is released");
    }
    if (this->ch_box.is_checked())
    {
        this->u_giff.resume();
    }
    else
    {
        this->u_giff.stop();
    }

    if (!this->ch_box.is_checked())
    {
        this->input_field.disable();
        this->b_button.disable();
        this->_button.disable();
        this->selction_menu.disable();

        this->status_bar.disable();
    }
    else
    {
        this->input_field.enable();
        this->b_button.enable();
        this->_button.enable();
        this->selction_menu.enable();

        this->status_bar.enable();
    }
}

void lock_screen::restart_system(FGUI::Touch_Widget<lock_screen> *_widget)
{
    void (*reset_func)() = 0;
    reset_func();
}

void lock_screen::update_window_label(FGUI::Touch_Widget<lock_screen> *_widget)
{
    this->text_feld.set_text(this->input_field.get_input_buffer());
    this->text_feld.draw();

    String tmp = this->input_field.get_input_buffer();
    this->u_giff.set_bitmap_change_speed(tmp.toInt() * 10);
}

void lock_screen::input_field_adjust(FGUI::Touch_Widget<lock_screen> *_widget, const char *_input_buffer)
{
    if (strcmp(_input_buffer, "1875") == 0)
    {
        _widget->touched_background_color = VGA_GREEN;
    }
    else
    {
        _widget->touched_background_color = VGA_RED;
    }
}
