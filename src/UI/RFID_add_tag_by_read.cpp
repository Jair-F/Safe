#include "UI/RFID_add_tag_by_read.hpp"
#include "RFID/RFID.hpp"

extern RFID::RFID rfid;

RFID_add_tag_pop_up::RFID_add_tag_pop_up(WindowBase *_parent_window) : PopUp_Window(_parent_window, 120, 120, 2),
                                                                       cancel_btn(this, {15, 60}, 50, 25, this),
                                                                       ok_btn(this, {15, 60}, 50, 25, this),
                                                                       status_label(this, {0 + this->get_border_weight(), 35},
                                                                                    this->get_content_width() - this->get_border_weight()),
                                                                       tag_id(this, {0, 0}, 60, 35, this, FGUI::InputField<2, RFID_add_tag_pop_up>::IN_INPUT_TYPE::IN_TEXT)
{
    ok_btn.setText("ok");
    ok_btn.hide();
    ok_btn.on_release = &RFID_add_tag_pop_up::_exit_window;

    cancel_btn.setText("cancel");
    cancel_btn.on_release = &RFID_add_tag_pop_up::_exit_window;

    status_label.set_text_alignment(FGUI::TextLabel::text_alignment::AL_CENTER);
    status_label.set_text("add tag");
    status_label.set_draw_border(false);
}

void RFID_add_tag_pop_up::_exit_window(FGUI::Touch_Widget<RFID_add_tag_pop_up> *_widget)
{
    this->_get_parent_window()->hide_pop_up_window();
}

void RFID_add_tag_pop_up::loop()
{
    WindowBase::loop();

    /*
    if (rfid.read_add_tag(this->tag_id))
    {
        this->status_label.set_text("tag added");
        this->cancel_btn.hide();
        this->ok_btn.show();
    }
    */
}