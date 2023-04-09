#include "UI/RFID_add_tag_by_read.hpp"
#include "UI/RFID_Settings.hpp"
#include "RFID/RFID.hpp"

extern RFID::RFID rfid;

#define CANCLE_ADD_BUTTON_HEIGHT 35
#define CANCLE_ADD_BUTTON_WIDTH 100
#define TAG_ID_INPUT_FIELD_WIDTH 80

extern RFID_settings_window *rfid_settings_wnd;

RFID_add_tag_window::RFID_add_tag_window(FGUI::WindowBase *_parent_window) : FGUI::Window(_parent_window),
                                                                             window_title(this, {0, 0}, this->get_content_width(), "add tag by read", BigFont),
                                                                             // window_title_div(this, {0, 32}, this->get_content_width(), 3, FGUI::Divider::d_alignment::AL_HORIZONTAL, VGA_BLUE),
                                                                             tag_id_input_field_label(this, {30, 40}, 150, "tag ID: ", BigFont),
                                                                             tag_id(this, {190, 40}, TAG_ID_INPUT_FIELD_WIDTH, 35, this, FGUI::InputField<2, RFID_add_tag_window>::IN_INPUT_TYPE::IN_TEXT),
                                                                             status_label(this, {0 + this->get_border_weight(), 100},
                                                                                          this->get_content_width() - this->get_border_weight(), "insert tag id", BigFont),
                                                                             cancel_btn(this, {46, 180}, CANCLE_ADD_BUTTON_WIDTH, CANCLE_ADD_BUTTON_HEIGHT, this),
                                                                             add_btn(this, {46, 180}, CANCLE_ADD_BUTTON_WIDTH, CANCLE_ADD_BUTTON_HEIGHT, this),
                                                                             exit_btn(this, {186, 180}, CANCLE_ADD_BUTTON_WIDTH, CANCLE_ADD_BUTTON_HEIGHT, this)
{
    // this->set_background_color(0x41e8); // gray like
    this->set_border_color(VGA_BLUE);

    this->window_title.set_draw_border(true);
    this->window_title.set_border_weight(3);
    this->window_title.released_border_color = VGA_BLUE;
    this->window_title.mark_shown();

    this->tag_id_input_field_label.set_draw_border(false);

    this->tag_id.set_border_weight(2);
    this->tag_id.touched_border_color = VGA_GRAY;
    this->tag_id.on_typing = &RFID_add_tag_window::_on_type_tag_id_check;

    this->add_btn.setText("add");
    this->add_btn.set_border_weight(2);
    this->add_btn.on_release = &RFID_add_tag_window::_add_tag;

    this->cancel_btn.setText("cancel");
    this->cancel_btn.hide();
    this->cancel_btn.set_border_weight(2);
    this->cancel_btn.on_release = &RFID_add_tag_window::_cancel_add_tag;

    this->status_label.set_text_alignment(FGUI::TextLabel::text_alignment::AL_CENTER);
    this->status_label.set_border_weight(1);
    this->status_label.set_draw_border(false);

    this->exit_btn.setText("exit");
    this->exit_btn.set_border_weight(2);
    this->exit_btn.on_release = &RFID_add_tag_window::_exit_window;
}

void RFID_add_tag_window::_add_tag(FGUI::Touch_Widget<RFID_add_tag_window> *_widget)
{
    const char *input_buffer = this->tag_id.get_input_buffer();
    if (strlen(input_buffer) == 0)
    {
        this->status_label.released_text_color = VGA_RED;
        this->status_label.set_text("Please insert a valid ID");
        return;
    }

    this->add_tag_id = String(input_buffer).toInt();

    if (add_tag_id <= 50)
    {
        this->add_btn.hide();
        this->cancel_btn.show();
        this->tag_id.disable();
        this->status_label.released_text_color = VGA_WHITE;
        this->status_label.set_text("add tag to reader");
        this->waiting_for_tag_to_add = true;
    }
    else
    {
        this->status_label.released_text_color = VGA_RED;
        this->status_label.set_text("Required: 0 < ID < 51");
    }
}

void RFID_add_tag_window::_cancel_add_tag(FGUI::Touch_Widget<RFID_add_tag_window> *_widget)
{
    waiting_for_tag_to_add = false;
    this->cancel_btn.hide();
    this->add_btn.show();
    this->tag_id.enable();
    this->status_label.set_text("");
}

void RFID_add_tag_window::loop()
{
    WindowBase::loop();

    if (this->waiting_for_tag_to_add)
    {
        Serial.println("reading tag...");
        // if (rfid.read_add_tag(add_tag_id))
        //{
        delay(3000);
        this->cancel_btn.hide();
        this->add_btn.show();
        this->waiting_for_tag_to_add = false;
        this->tag_id.enable();
        this->status_label.released_text_color = VGA_GREEN;
        this->status_label.set_text("tag was added");
        //}
    }

    /*
    if (rfid.read_add_tag(this->tag_id))
    {
        this->status_label.set_text("tag added");
        this->cancel_btn.hide();
        this->ok_btn.show();
    }
    */
}

void RFID_add_tag_window::_on_type_tag_id_check(FGUI::Touch_Widget<RFID_add_tag_window> *_widget, const char *_input_buffer)
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
        this->status_label.set_text("click on add");
    }
}

void RFID_add_tag_window::_exit_window(FGUI::Touch_Widget<RFID_add_tag_window> *_widget)
{
    // PopUp_Window::_exit_pop_up_window(reinterpret_cast<FGUI::Touch_Widget<FGUI::PopUp_Window> *>(_widget));
    this->_get_main_window()->set_active_window(this->_get_parent_window());
}