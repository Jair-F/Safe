#include "UI/try_window.hpp"
#include "UI/RFID_Settings.hpp"
#include "UI/Settings.hpp"

extern const unsigned short settings_white[];
extern const unsigned short back_sign_white[];
extern const unsigned short settings_black[];

// for button
FGUI::position upper_left{20, 40};
FGUI::position lower_right{130, 70};

extern const unsigned short back_icon[];
extern const unsigned short clear_database[];
extern const unsigned short delete_by_id[];
extern const unsigned short plus_add[];
extern const unsigned short delete_by_scan[];
extern const unsigned short plus_add[];

try_window::try_window(FGUI::MainWindow *_main_window) : FGUI::Window(_main_window),
                                                         _button(this, ::upper_left, ::lower_right, this),
                                                         _button2(this,
                                                                  {this->_get_display()->getDisplayXSize() - 100, this->_get_display()->getDisplayYSize() - 35},
                                                                  {this->_get_display()->getDisplayXSize() - 1, this->_get_display()->getDisplayYSize() - 1},
                                                                  this),
                                                         text_feld(this, {0, 0}, 319,
                                                                   "Window\n title", SmallFont, 2, 0),
                                                         ch_box(this, {35, 140}, 25, this, 3),
                                                         input_field(this,
                                                                     {150, 150}, 60, 35, this,
                                                                     FGUI::InputField<20, try_window>::IN_INPUT_TYPE::IN_TEXT,
                                                                     4),
                                                         pop_up_window(this, 150, 100, 3),
                                                         status_bar(this,
                                                                    {0, this->_get_display()->getDisplayYSize() - 35},
                                                                    this->_get_display()->getDisplayXSize() - 110,
                                                                    "ERROR"),
                                                         b_button(this, {160, 32}, 40, 40, this, plus_add, delete_by_scan, delete_by_id, 5, 3),
                                                         div(this, {5, 100}, 50, 4, FGUI::Divider::d_alignment::AL_VERTICAL, VGA_WHITE),
                                                         progressBar(this, {80, 100}, {290, 115}, 2),
                                                         u_giff(this, {225, 125}, 40, 40, 3, 2, 1000, 2, clear_database, delete_by_id),
                                                         p_button(&pop_up_window, {10, 20}, {100, 45}, this),
                                                         selction_menu(this, {225, 50}, {291, 72}, this)
{

    text_feld.set_draw_border(true);
    text_feld.set_text_alignment(text_feld.AL_CENTER);
    this->text_feld.released_background_color = VGA_BLUE;
    this->text_feld.released_border_color = VGA_WHITE;
    this->set_draw_border(true);

    _button.setText("Click mich");
    _button.set_border_weight(3);
    _button.touched_border_color = VGA_BLUE;
    _button.touched_background_color = VGA_GREEN;
    _button.on_touch = &try_window::button_print_clicked;
    _button.on_release = &try_window::button_print_released;

    _button2.on_release = &try_window::restart_system;
    _button2.setText("Restart");

    ch_box.on_release = &try_window::_handle_check_box;
    ch_box.normal_check_sign_color = VGA_WHITE;
    ch_box.released_background_color = VGA_GREEN;
    ch_box.touched_background_color = VGA_GREEN;
    ch_box.released_border_color = VGA_AQUA;
    ch_box.touched_border_color = VGA_RED;
    ch_box.set_checked(true);

    input_field.set_input_buffer("123");
    input_field.touched_background_color = VGA_RED;
    input_field.on_enter = &try_window::update_window_label;
    input_field.on_focus_lose = &try_window::update_window_label;
    input_field.on_typing = &try_window::input_field_adjust;
    // Serial.println(this->_get_main_window()->request_focus(&input_field) == true ? "focus request successfull" : "focus request failed");

    pop_up_window.set_background_color(VGA_GRAY);
    pop_up_window.set_border_color(VGA_YELLOW);

    p_button.setText("uncheck_box");
    p_button.released_text_color = VGA_WHITE;
    p_button.set_border_weight(2);
    p_button.released_border_color = VGA_BLUE;
    p_button.on_release = &try_window::uncheck_check_box_pop_up;

    status_bar.set_text("ERROR");
    status_bar.set_text_alignment(FGUI::TextLabel::text_alignment::AL_CENTER);
    status_bar.set_draw_border(true);
    status_bar.set_font(SmallFont);

    b_button.on_release = &try_window::button_print_clicked;
    b_button.touched_border_color = VGA_GREEN;
    b_button.set_draw_border(true);

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
    this->selction_menu.set_selection("CRITICAL");
}

void try_window::uncheck_check_box_pop_up(FGUI::Touch_Widget<try_window> *_widget)
{
    this->ch_box.set_checked(!this->ch_box.is_checked());
    this->hide_pop_up_window();
    // this->_get_main_window()->set_active_window(settings_wnd);
}
void try_window::update_status_bar(FGUI::Touch_Widget<try_window> *_widget)
{
    this->status_bar.set_text(this->input_field.get_input_buffer());
}

void try_window::button_print_clicked(FGUI::Touch_Widget<try_window> *_widget)
{
    // Serial.println("Clicked button");
    text_feld.set_text("button clicked");

    String tmp = this->input_field.get_input_buffer();
    uint8_t step = tmp.toInt();
    if (this->progressBar.get_progress() >= 100)
    {
        this->progressBar.set_progress(0);
    }
    this->progressBar.set_progress(this->progressBar.get_progress() + step);
}

void try_window::button_print_released(FGUI::Touch_Widget<try_window> *_widget)
{
    // FGUI::Button *but_ptr = static_cast<FGUI::Button *>(widget);
    // Serial.println("Released button");
    text_feld.set_text("button is released");
    this->show_pop_up_window(&pop_up_window);
}

void try_window::_handle_check_box(FGUI::Touch_Widget<try_window> *_widget)
{
    FGUI::CheckBox<try_window> *box = static_cast<FGUI::CheckBox<try_window> *>(_widget);
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

void try_window::restart_system(FGUI::Touch_Widget<try_window> *_widget)
{
    void (*reset_func)() = 0;
    reset_func();
}

void try_window::update_window_label(FGUI::Touch_Widget<try_window> *_widget)
{
    this->text_feld.set_text(this->input_field.get_input_buffer());
    this->text_feld.draw();

    String tmp = this->input_field.get_input_buffer();
    this->u_giff.set_bitmap_change_speed(tmp.toInt() * 10);
}

void try_window::input_field_adjust(FGUI::Touch_Widget<try_window> *_widget, const char *_input_buffer)
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
