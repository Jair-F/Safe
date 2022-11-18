#include "UI/RFID_delete_tag_by_id.hpp"
#include "UI/RFID_Settings.hpp"
#include "RFID/RFID.hpp"

extern RFID::RFID rfid;
extern RFID_settings_window *rfid_settings_wnd;
extern uint8_t Retro8x16[]; // font...

RFID_delete_tag_by_id::RFID_delete_tag_by_id(FGUI::WindowBase *_parent_window) : Window(_parent_window),
                                                                                 window_title(this, {0, 0}, this->get_content_width(), "delete tag by id", BigFont),
                                                                                 tag_id_to_remove(this, {120, 60}, 80, 40, this, FGUI::InputField<2, RFID_delete_tag_by_id>::IN_INPUT_TYPE::IN_TEXT),
                                                                                 status_label(this, {0, 120}, this->get_content_width(), "", Retro8x16),
                                                                                 delete_btn(this, {46, 180}, 100, 35, this),
                                                                                 exit_btn(this, {186, 180}, 100, 35, this)
{
    this->window_title.set_border_weight(3);
    this->window_title.released_border_color = VGA_BLUE;

    this->tag_id_to_remove.on_typing = &RFID_delete_tag_by_id::_on_type_tag_id_check;
    this->tag_id_to_remove.set_border_weight(2);

    this->status_label.set_draw_border(false);
    this->status_label.set_text_alignment(FGUI::TextLabel::text_alignment::AL_CENTER);
    this->status_label.set_text("insert tag id to remove");

    this->delete_btn.setText("delete");
    this->delete_btn.set_border_weight(3);
    this->delete_btn.on_release = &RFID_delete_tag_by_id::_remove_tag_by_id;

    this->exit_btn.setText("exit");
    this->exit_btn.set_border_weight(3);
    this->exit_btn.on_release = &RFID_delete_tag_by_id::_exit_window;
}

bool RFID_delete_tag_by_id::_check_tag_id(const char *_id)
{
    if (strlen(_id) == 0)
    {
        return false;
    }
    uint8_t tag_id = String(_id).toInt();
    if (tag_id > 50)
    {
        return false;
    }
    return true;
}

void RFID_delete_tag_by_id::_on_type_tag_id_check(FGUI::Touch_Widget<RFID_delete_tag_by_id> *_widget, const char *_input_buffer)
{
    if (strlen(_input_buffer) == 0)
    {
        this->status_label.released_text_color = VGA_RED;
        this->status_label.set_text("Please insert a valid ID");
        return;
    }
    uint8_t tag_id = String(_input_buffer).toInt();
    if (tag_id > 50)
    {
        this->status_label.released_text_color = VGA_RED;
        this->status_label.set_text("tag ID needs to be between 0 and 50");
    }
    else
    {
        this->status_label.released_text_color = VGA_WHITE;
        this->status_label.set_text("click on remove");
    }
}

void RFID_delete_tag_by_id::_exit_window(FGUI::Touch_Widget<RFID_delete_tag_by_id> *_widget)
{
    this->_get_main_window()->set_active_window(this->_get_parent_window());
}

void RFID_delete_tag_by_id::_remove_tag_by_id(FGUI::Touch_Widget<RFID_delete_tag_by_id> *_widget)
{
    if (strlen(this->tag_id_to_remove.get_input_buffer()) == 0) // if a input is in the input_field
    {
        this->_on_type_tag_id_check(&this->tag_id_to_remove, this->tag_id_to_remove.get_input_buffer()); // sets the status label accordingly...
    }
    else if (this->_check_tag_id(this->tag_id_to_remove.get_input_buffer()))
    {
        uint8_t tag_to_remove = String(this->tag_id_to_remove.get_input_buffer()).toInt();
        rfid.remove_tag(tag_to_remove);
        this->status_label.released_text_color = VGA_GREEN;
        this->status_label.set_text("tag removed");
    }
    else
    {
        this->status_label.released_text_color = VGA_RED;
        this->status_label.set_text("id is out of range");
    }
}
