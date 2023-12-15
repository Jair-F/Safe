#include "UI/Fingerprint_Settings.hpp"
#include "UI/Settings.hpp"
#include "Fingerprint.hpp"
#include "UI/lock_screen.hpp"

extern const unsigned short back_icon[];
extern const unsigned short back_33_33[];
extern const unsigned short settings_icon[];
extern const unsigned short clear_database[];
extern const unsigned short delete_by_id[];
extern const unsigned short delete_by_scan[];
extern const unsigned short plus_add[];

extern lock_screen *l_screen;
extern Settings_window *settings_window;

extern Fingerprint::Fingerprint *fingerprint;

Fingerprint_Settings::Fingerprint_Settings(FGUI::WindowBase *_parent_window) : FGUI::Window(_parent_window),
                                                                               window_title(this, {34, 0}, 285, "add", arial_bold, 0, 0),
                                                                               back_btn(this, {0, 0}, 33, 33, this, back_33_33, (uint8_t)0, (uint8_t)0),
                                                                               add_btn(this, {13, 40}, 40, 40, this, plus_add, 3, 0),
                                                                               delete_by_id_btn(this, {13, 90}, 40, 40, this, delete_by_id, 3, 0),
                                                                               delete_by_scan_btn(this, {13, 139}, 40, 40, this, delete_by_scan, 3, 0),
                                                                               clear_database_btn(this, {13, 188}, 40, 40, this, clear_database, 3, 0),
                                                                               div_v(this, {73, 41}, 191, 3, FGUI::Divider::d_alignment::AL_VERTICAL, VGA_WHITE),
                                                                               div_h(this, {0, 32}, 320, 3, FGUI::Divider::d_alignment::AL_HORIZONTAL, VGA_BLUE),
                                                                               id_label(this, {102, 49}, 86, "ID:", SmallFont, 0, 0),
                                                                               id_input(this, {226, 51}, 54, 29, this, FGUI::InputField<2, Fingerprint_Settings>::IN_INPUT_TYPE::IN_TEXT, 3),
                                                                               status_label(this, {88, 100}, 217, "status label", SmallFont, 0, 0),
                                                                               p_bar(this, {105, 143}, 183, 10, 2, 3),
                                                                               confirm_btn(this, {102, 185}, 67, 28, this, 2, 0),
                                                                               reset_btn(this, {218, 185}, 67, 28, this, 2, 0),
                                                                               delete_btn(this, {161, 185}, 67, 28, this, 2, 0)
{
    this->back_btn.on_release = &Fingerprint_Settings::_exit_window;

    this->window_title.set_text_alignment(FGUI::TextLabel::text_alignment::AL_CENTER);
    this->window_title.released_background_color = this->get_background_color();

    this->add_btn.touched_background_color = VGA_GRAY;
    this->add_btn.released_border_color = this->get_background_color();
    this->add_btn.on_release = &Fingerprint_Settings::_show_add;

    this->delete_by_id_btn.touched_background_color = VGA_GRAY;
    this->delete_by_id_btn.released_border_color = this->get_background_color();
    this->delete_by_id_btn.on_release = &Fingerprint_Settings::_show_delete_by_id;

    this->delete_by_scan_btn.touched_background_color = VGA_GRAY;
    this->delete_by_scan_btn.released_border_color = this->get_background_color();
    this->delete_by_scan_btn.on_release = &Fingerprint_Settings::_show_delete_by_scan;

    this->clear_database_btn.touched_background_color = VGA_GRAY;
    this->clear_database_btn.released_border_color = this->get_background_color();
    this->clear_database_btn.on_release = &Fingerprint_Settings::_show_clear_database;

    this->status_label.set_text_alignment(FGUI::TextLabel::text_alignment::AL_CENTER);

    this->id_input.on_typing = &Fingerprint_Settings::_print_id_info;

    this->confirm_btn.setText("confirm");
    this->confirm_btn.on_release = &Fingerprint_Settings::_handle_confirm;
    this->confirm_btn.mark_disabled();

    this->reset_btn.setText("reset");
    this->reset_btn.on_release = &Fingerprint_Settings::_handle_reset;
    this->reset_btn.mark_enabled();

    this->delete_btn.setText("delete");
    this->delete_btn.on_release = &Fingerprint_Settings::_handle_delete;
    this->delete_btn.mark_hidden();

    this->w_status = window_status::FINGER_add;
    this->add_btn.released_border_color = VGA_WHITE;
}

void Fingerprint_Settings::_exit_window(FGUI::Touch_Widget<Fingerprint_Settings> *_widget)
{
    this->_get_main_window()->set_active_window(this->_get_parent_window());
}

void Fingerprint_Settings::_show_add(FGUI::Touch_Widget<Fingerprint_Settings> *_widget)
{
    w_status = window_status::FINGER_add;

    this->add_btn.released_border_color = VGA_WHITE;
    this->delete_by_id_btn.released_border_color = VGA_BLACK;
    this->delete_by_scan_btn.released_border_color = VGA_BLACK;
    this->clear_database_btn.released_border_color = VGA_BLACK;

    this->p_bar.set_progress(0);

    this->window_title.set_text("add");

    this->confirm_btn.mark_disabled();

    this->delete_btn.hide();

    this->id_input.show();
    this->id_label.show();
    this->p_bar.show();
    this->confirm_btn.show();
    this->reset_btn.show();

    this->_redraw_chagned_window();
}

void Fingerprint_Settings::_show_delete_by_id(FGUI::Touch_Widget<Fingerprint_Settings> *_widget)
{
    w_status = window_status::FINGER_delete_by_id;

    this->add_btn.released_border_color = VGA_BLACK;
    this->delete_by_id_btn.released_border_color = VGA_WHITE;
    this->delete_by_scan_btn.released_border_color = VGA_BLACK;
    this->clear_database_btn.released_border_color = VGA_BLACK;

    this->window_title.set_text("del by id");

    this->p_bar.hide();
    this->confirm_btn.hide();
    this->reset_btn.hide();

    this->id_input.show();
    this->id_label.show();
    this->delete_btn.show();

    this->_redraw_chagned_window();
    // this->_print_id_info(&this->id_input,
    //                      this->id_input.get_input_buffer()); // changes the delete button to disabled if is no valid id
}

void Fingerprint_Settings::_show_delete_by_scan(FGUI::Touch_Widget<Fingerprint_Settings> *_widget)
{
    w_status = window_status::FINGER_delete_by_scan;

    this->add_btn.released_border_color = VGA_BLACK;
    this->delete_by_id_btn.released_border_color = VGA_BLACK;
    this->delete_by_scan_btn.released_border_color = VGA_WHITE;
    this->clear_database_btn.released_border_color = VGA_BLACK;

    this->window_title.set_text("del by scan");

    this->confirm_btn.mark_disabled();

    this->p_bar.hide();
    this->id_input.hide();
    this->id_label.hide();
    this->delete_btn.hide();

    this->confirm_btn.show();
    this->reset_btn.show();

    this->_redraw_chagned_window();
}

void Fingerprint_Settings::_show_clear_database(FGUI::Touch_Widget<Fingerprint_Settings> *_widget)
{
    w_status = window_status::FINGER_clear_database_waiting_confirm;

    this->add_btn.released_border_color = VGA_BLACK;
    this->delete_by_id_btn.released_border_color = VGA_BLACK;
    this->delete_by_scan_btn.released_border_color = VGA_BLACK;
    this->clear_database_btn.released_border_color = VGA_WHITE;

    this->delete_btn.mark_enabled();

    this->p_bar.hide();
    this->id_input.hide();
    this->id_label.hide();
    this->confirm_btn.hide();
    this->reset_btn.hide();

    this->delete_btn.show();

    this->window_title.set_text("clr database");

    this->_redraw_chagned_window();
}

bool Fingerprint_Settings::_id_ok(uint8_t _id)
{
    return !(_id > fingerprint->_get_fingerprint_capacity() || _id < 1);
}

void Fingerprint_Settings::_redraw_chagned_window()
{
    this->window_title.draw();
    this->status_label.draw();
    this->p_bar.draw();
    this->add_btn.draw();
    this->delete_by_id_btn.draw();
    this->delete_by_scan_btn.draw();
    this->clear_database_btn.draw();
}

void Fingerprint_Settings::_print_id_info(FGUI::Touch_Widget<Fingerprint_Settings> *_widget, const char *_input_buffer)
{
    uint8_t id = String(this->id_input.get_input_buffer()).toInt();
    if (id > fingerprint->_get_fingerprint_capacity() ||
        id < 1)
    {
        this->status_label.set_text("invalid id");
        this->confirm_btn.disable();
        this->delete_btn.disable();
    }
    else
    {
        this->status_label.set_text("");
        if (this->w_status == window_status::FINGER_add_waiting_for_2)
        {
            this->confirm_btn.enable();
            this->delete_btn.enable();
        }
    }
    this->status_label.draw();
}

void Fingerprint_Settings::_pre_show()
{
    this->p_bar.set_progress(0);
    this->confirm_btn.disable();
}

void Fingerprint_Settings::_handle_delete(FGUI::Touch_Widget<Fingerprint_Settings> *_widget)
{
    switch (this->w_status)
    {
    case window_status::FINGER_clear_database:
    {
        if (fingerprint->clear_database() == true)
            this->status_label.set_text("cleared database");
        else
            this->status_label.set_text("error clearing database");
        break;
    }
    case window_status::FINGER_clear_database_waiting_confirm:
    {
        this->w_status = window_status::FINGER_clear_database;
        this->status_label.set_text("press again to confirm");
        break;
    }
    case window_status::FINGER_delete_by_id:
    {
        if (strlen(this->id_input.get_input_buffer()) > 0)
        {
            if (fingerprint->delete_finger(String(this->id_input.get_input_buffer()).toInt()))
                this->status_label.set_text("finger removed");
            else
                this->status_label.set_text("error removing finger");
        }
        else
        {
            this->status_label.set_text(F("please insert an id"));
        }

        break;
    }
    }
    this->status_label.draw();
}

void Fingerprint_Settings::_handle_reset(FGUI::Touch_Widget<Fingerprint_Settings> *_widget)
{
    switch (this->w_status)
    {
    case window_status::FINGER_add_waiting_for_2:
    {
        this->w_status = window_status::FINGER_add;
        break;
    }
    }
    this->confirm_btn.disable();
}

void Fingerprint_Settings::_handle_confirm(FGUI::Touch_Widget<Fingerprint_Settings> *_widget)
{
    switch (this->w_status)
    {
    case window_status::FINGER_add:
        this->p_bar.set_progress(0);
        this->confirm_btn.disable();

    case window_status::FINGER_add_waiting_for_2:
    {
        if (fingerprint->store_finger_model(String(this->id_input.get_input_buffer()).toInt()))
            this->status_label.set_text(F("finger added"));
        else
            this->status_label.set_text(F("error adding the finger"));

        this->confirm_btn.disable();
        this->p_bar.set_progress(0);
        break;
    }
    case window_status::FINGER_delete_by_scan:
    {
        uint8_t _del_id;
        if (fingerprint->get_id(_del_id))
        {
            if (fingerprint->delete_finger(_del_id))
                this->status_label.set_text(F("finger deleted"));
            else
                this->status_label.set_text(F("error deleting the finger"));
        }
        else
        {
            this->status_label.set_text(F("no matching finer found in the database"));
        }

        this->confirm_btn.disable();
        break;
    }
    }
    this->status_label.draw();
}

void Fingerprint_Settings::loop()
{
    switch (this->w_status)
    {
    case window_status::FINGER_add:
    {
        if (fingerprint->read_finger(1))
        {
            this->p_bar.set_progress(50);
            this->delete_btn.mark_enabled();
            this->add_btn.mark_enabled();
            this->w_status = window_status::FINGER_add_waiting_for_2;
        }
        break;
    }
    case window_status::FINGER_delete_by_scan:
    {
        if (fingerprint->read_finger(1))
            this->confirm_btn.enable();
        break;
    }
    case window_status::FINGER_add_waiting_for_2:
    {
        if (fingerprint->finger_on_sensor())
        {
            if (fingerprint->read_finger(2))
            {
                this->p_bar.set_progress(100);
                this->confirm_btn.enable();
            }
        }
        break;
    }
    }
    FGUI::WindowBase::loop();
}
