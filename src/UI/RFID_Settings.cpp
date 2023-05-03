#include "UI/RFID_Settings.hpp"
#include "UI/Settings.hpp"
#include "RFID/RFID.hpp"

extern const unsigned short back_icon[];
extern const unsigned short back_33_33[];
extern const unsigned short settings_icon[];
extern const unsigned short clear_database[];
extern const unsigned short delete_by_id[];
extern const unsigned short delete_by_scan[];
extern const unsigned short plus_add[];

class lock_screen;
extern lock_screen *l_screen;
extern Settings_window *settings_window;

extern RFID::RFID *rfid;

RFID_settings_window::RFID_settings_window(FGUI::WindowBase *_parent_window) : FGUI::Window(_parent_window),
                                                                               window_title(this, {34, 0}, 285, "RFID - add", arial_bold, 0, 0),
                                                                               back_btn(this, {0, 0}, 33, 33, this, back_33_33, (uint8_t)0, (uint8_t)0),
                                                                               add_btn(this, {13, 40}, 40, 40, this, plus_add, 3, 0),
                                                                               delete_by_id_btn(this, {13, 90}, 40, 40, this, delete_by_id, 3, 0),
                                                                               delete_by_scan_btn(this, {13, 139}, 40, 40, this, delete_by_scan, 3, 0),
                                                                               clear_database_btn(this, {13, 188}, 40, 40, this, clear_database, 3, 0),
                                                                               div_v(this, {73, 41}, 191, 3, FGUI::Divider::d_alignment::AL_VERTICAL, VGA_WHITE),
                                                                               div_h(this, {0, 32}, 320, 3, FGUI::Divider::d_alignment::AL_HORIZONTAL, VGA_BLUE),
                                                                               tag_id_label(this, {102, 59}, 86, "ID:", SmallFont, 0, 0),
                                                                               tag_id_input(this, {226, 51}, 54, 29, this, FGUI::InputField<2, RFID_settings_window>::IN_INPUT_TYPE::IN_TEXT, 3),
                                                                               status_label(this, {88, 123}, 217, "status label", SmallFont, 0, 0),
                                                                               confirm_btn(this, {102, 185}, 67, 28, this, 2, 0),
                                                                               reset_btn(this, {218, 185}, 67, 28, this, 2, 0),
                                                                               delete_btn(this, {161, 185}, 67, 28, this, 2, 0)
{
    this->back_btn.on_release = &RFID_settings_window::_exit_window;

    this->window_title.set_text_alignment(FGUI::TextLabel::text_alignment::AL_CENTER);
    this->window_title.released_background_color = this->get_background_color();

    this->add_btn.touched_background_color = VGA_GRAY;
    this->add_btn.released_border_color = this->get_background_color();
    this->add_btn.on_release = &RFID_settings_window::_show_add;

    this->delete_by_id_btn.touched_background_color = VGA_GRAY;
    this->delete_by_id_btn.released_border_color = this->get_background_color();
    this->delete_by_id_btn.on_release = &RFID_settings_window::_show_delete_by_id;

    this->delete_by_scan_btn.touched_background_color = VGA_GRAY;
    this->delete_by_scan_btn.released_border_color = this->get_background_color();
    this->delete_by_scan_btn.on_release = &RFID_settings_window::_show_delete_by_scan;

    this->clear_database_btn.touched_background_color = VGA_GRAY;
    this->clear_database_btn.released_border_color = this->get_background_color();
    this->clear_database_btn.on_release = &RFID_settings_window::_show_clear_database;

    this->status_label.set_text_alignment(FGUI::TextLabel::text_alignment::AL_CENTER);

    this->tag_id_input.on_typing = &RFID_settings_window::_print_id_info;

    this->confirm_btn.setText("confirm");
    this->confirm_btn.on_release = &RFID_settings_window::_handle_confirm;
    this->confirm_btn.mark_disabled();

    this->reset_btn.setText("reset");
    this->reset_btn.on_release = &RFID_settings_window::_handle_reset;
    this->reset_btn.mark_disabled();

    this->delete_btn.setText("delete");
    this->delete_btn.on_release = &RFID_settings_window::_handle_delete;
    this->delete_btn.mark_hidden();

    this->w_status = window_status::RFID_add;
    this->add_btn.released_border_color = VGA_WHITE;
}

void RFID_settings_window::_exit_window(FGUI::Touch_Widget<RFID_settings_window> *_widget)
{
    // switch back to the settings window
    this->_get_main_window()->set_active_window(this->_get_parent_window());
}

void RFID_settings_window::_show_add(FGUI::Touch_Widget<RFID_settings_window> *_widget)
{
    w_status = window_status::RFID_add;

    this->add_btn.released_border_color = VGA_WHITE;
    this->delete_by_id_btn.released_border_color = VGA_BLACK;
    this->delete_by_scan_btn.released_border_color = VGA_BLACK;
    this->clear_database_btn.released_border_color = VGA_BLACK;

    this->window_title.set_text("add");

    this->delete_btn.hide();

    this->tag_id_input.show();
    this->tag_id_label.show();
    this->confirm_btn.show();
    this->reset_btn.show();

    this->_redraw_chagned_window();
}

void RFID_settings_window::_show_delete_by_id(FGUI::Touch_Widget<RFID_settings_window> *_widget)
{
    w_status = window_status::RFID_delete_by_id;

    this->add_btn.released_border_color = VGA_BLACK;
    this->delete_by_id_btn.released_border_color = VGA_WHITE;
    this->delete_by_scan_btn.released_border_color = VGA_BLACK;
    this->clear_database_btn.released_border_color = VGA_BLACK;

    this->window_title.set_text("del by id");

    this->confirm_btn.hide();
    this->reset_btn.hide();

    this->tag_id_input.show();
    this->tag_id_label.show();
    this->delete_btn.show();

    this->_redraw_chagned_window();
    this->_print_id_info(&this->tag_id_input,
                         this->tag_id_input.get_input_buffer()); // changes the delete button to disabled if is no valid id
}

void RFID_settings_window::_show_delete_by_scan(FGUI::Touch_Widget<RFID_settings_window> *_widget)
{
    w_status = window_status::RFID_delete_by_scan;

    this->add_btn.released_border_color = VGA_BLACK;
    this->delete_by_id_btn.released_border_color = VGA_BLACK;
    this->delete_by_scan_btn.released_border_color = VGA_WHITE;
    this->clear_database_btn.released_border_color = VGA_BLACK;

    this->window_title.set_text("del by scan");

    this->tag_id_input.hide();
    this->tag_id_label.hide();
    this->delete_btn.hide();

    this->confirm_btn.show();
    this->reset_btn.show();

    this->_redraw_chagned_window();
}

void RFID_settings_window::_show_clear_database(FGUI::Touch_Widget<RFID_settings_window> *_widget)
{
    w_status = window_status::RFID_clear_database;

    this->add_btn.released_border_color = VGA_BLACK;
    this->delete_by_id_btn.released_border_color = VGA_BLACK;
    this->delete_by_scan_btn.released_border_color = VGA_BLACK;
    this->clear_database_btn.released_border_color = VGA_WHITE;

    this->delete_btn.enable();

    this->tag_id_input.hide();
    this->tag_id_label.hide();
    this->confirm_btn.hide();
    this->reset_btn.hide();

    this->delete_btn.show();

    this->window_title.set_text("clr database");

    this->_redraw_chagned_window();
}

void RFID_settings_window::_redraw_chagned_window()
{
    this->window_title.draw();
    this->status_label.draw();
    this->add_btn.draw();
    this->delete_by_id_btn.draw();
    this->delete_by_scan_btn.draw();
    this->clear_database_btn.draw();
}

void RFID_settings_window::_handle_confirm(FGUI::Touch_Widget<RFID_settings_window> *_widget)
{
    switch (this->w_status)
    {
    case window_status::RFID_add:
    {
        if (this->tmp_uid.is_set())
        {
            rfid->add_tag(String(this->tag_id_input.get_input_buffer()).toInt(), this->tmp_uid);

            String tmp = "added tag ";
            tmp += this->tmp_uid.to_string();
            this->status_label.set_text(tmp);
        }
        else
        {
            this->status_label.set_text("please scan a valid tag");
        }
        break;
    }
    case window_status::RFID_delete_by_scan:
    {
        if (this->tmp_uid.is_set())
        {
            String tmp;
            if (rfid->remove_tag(this->tmp_uid))
            {
                tmp = "successfully removed tag ";
                tmp += this->tmp_uid.to_string();
                this->status_label.set_text(tmp);
            }
            else
            {
                tmp = "tag " + this->tmp_uid.to_string() + " not found in the database - nothing removed";
                this->status_label.set_text(tmp);
            }
        }
        else
            this->status_label.set_text("please scan a valid tag");
        break;
    }
    }
    this->status_label.draw();
}

void RFID_settings_window::_handle_reset(FGUI::Touch_Widget<RFID_settings_window> *_widget)
{
    this->tmp_uid.clear();
    this->add_btn.disable();
    this->reset_btn.disable();
}

void RFID_settings_window::_handle_delete(FGUI::Touch_Widget<RFID_settings_window> *_widget)
{
    switch (this->w_status)
    {
    case window_status::RFID_clear_database:
    {
        this->status_label.set_text("touch again delete to\nconfirm");
        this->w_status = window_status::RFID_clear_database_waiting_confirm;
        break;
    }
    case window_status::RFID_clear_database_waiting_confirm:
    {
        rfid->clear_database();
        this->status_label.set_text("cleared database");
        this->w_status = window_status::RFID_clear_database;
        break;
    }
    case window_status::RFID_delete_by_id:
    {
        String id = this->tag_id_input.get_input_buffer();
        rfid->remove_tag(id.toInt());
        this->status_label.set_text("tag removed");
        break;
    }
    }
    this->status_label.draw();
}

void RFID_settings_window::loop()
{
    switch (this->w_status)
    {
    case window_status::RFID_delete_by_scan:
    {
        if (rfid->tag_present() && this->tmp_uid.is_set() == false) // scan only if tmp uid isnt set
        {
            this->tmp_uid = rfid->read_tag_UID();
            if (this->tmp_uid.is_set() == false)
            {
                this->status_label.set_text("error reading tag UID");
                this->reset_btn.disable();
                this->add_btn.disable();
            }
            else
            {
                this->status_label.set_text("tag uid: " + this->tmp_uid.to_string());
                this->reset_btn.enable();
                this->add_btn.enable();
            }
            this->status_label.draw();
            this->confirm_btn.enable();
        }
        break;
    }
    }
    FGUI::WindowBase::loop();
}

void RFID_settings_window::_print_id_info(FGUI::Touch_Widget<RFID_settings_window> *_widget, const char *_input_buffer)
{
    String input_buffer = this->tag_id_input.get_input_buffer();

    // no valid id?
    if (input_buffer.length() == 0 ||
        !(input_buffer.toInt() < RFID::NUM_OF_TAGS && input_buffer.toInt() >= 0))
    {
        this->status_label.set_text("no valid id");
        this->confirm_btn.disable();
        this->delete_btn.disable();
    }
    else // valid id
    {
        this->delete_btn.enable();
        if (this->tmp_uid.is_set())
        {
            // button for adding tag by read needs also valid id and valid read tag uid
            this->confirm_btn.enable();
        }

        auto uid = rfid->get_tag_uid(input_buffer.toInt());
        if (uid.is_set())
        {
            String tmp = "set with UID: ";
            tmp += uid.to_string();
            this->status_label.set_text(tmp);
        }
        else
        {
            this->status_label.set_text("");
        }
    }
    this->status_label.draw();
}
