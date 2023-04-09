#include "UI/Fingerprint/add_fingerprint.hpp"
#include "Fingerprint.hpp"

namespace
{
    // in pixels
    constexpr uint8_t HORIZONTAL_GAP_BETWEEN_BUTTONS = 15;
    constexpr uint8_t VERTICAL_GAP_BETWEEN_BUTTONS = 10;
    constexpr uint8_t BUTTON_HEIGHT = 40;
    constexpr uint8_t BUTTON_WIDTH = 135;
    constexpr uint8_t BUTTON_BORDER_WEIGHT = 3;

    constexpr uint8_t CANCLE_ADD_BUTTON_HEIGHT = 35;
    constexpr uint8_t CANCLE_ADD_BUTTON_WIDTH = 100;
    constexpr uint8_t TAG_ID_INPUT_FIELD_WIDTH = 80;
}

extern Fingerprint::Fingerprint *fingerprint;

add_fingerprint::add_fingerprint(FGUI::WindowBase *_parent_window) : FGUI::Window(_parent_window),
                                                                     window_title(this, {0, 0}, this->get_content_width(), "add fingerprint", BigFont),
                                                                     // window_title_div(this, {0, 32}, this->get_content_width(), 3, FGUI::Divider::d_alignment::AL_HORIZONTAL, VGA_BLUE),
                                                                     id_input_field_label(this, {30, 40}, 150, "fingerprint ID: ", Retro8x16),
                                                                     id(this, {190, 40}, TAG_ID_INPUT_FIELD_WIDTH, 35, this, FGUI::InputField<3, add_fingerprint>::IN_INPUT_TYPE::IN_TEXT),
                                                                     status_label(this, {(uint16_t)0 + this->get_border_weight(), 100},
                                                                                  this->get_content_width() - this->get_border_weight(), "insert fingperint id", Retro8x16),
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

    this->id_input_field_label.set_draw_border(false);

    this->id.set_border_weight(2);
    this->id.touched_border_color = VGA_GRAY;
    this->id.on_typing = &add_fingerprint::_on_type_tag_id_check;

    this->add_btn.setText("add");
    this->add_btn.set_border_weight(2);
    this->add_btn.on_release = &add_fingerprint::_add_fingerprint;

    this->cancel_btn.setText("cancel");
    this->cancel_btn.hide();
    this->cancel_btn.set_border_weight(2);
    this->cancel_btn.on_release = &add_fingerprint::_cancel_add_fingerprint;

    this->status_label.set_text_alignment(FGUI::TextLabel::text_alignment::AL_CENTER);
    this->status_label.set_border_weight(1);
    this->status_label.set_draw_border(false);

    this->exit_btn.setText("exit");
    this->exit_btn.set_border_weight(2);
    this->exit_btn.on_release = &add_fingerprint::_exit_window;
}

void add_fingerprint::_on_type_tag_id_check(FGUI::Touch_Widget<add_fingerprint> *_widget, const char *_input_buffer)
{
    if (strlen(_input_buffer) == 0)
    {
        this->status_label.released_text_color = VGA_RED;
        this->status_label.set_text("Please insert a valid ID");
        return;
    }
    uint8_t tag_id = String(_input_buffer).toInt();
    if (tag_id > 120)
    {
        this->status_label.released_text_color = VGA_RED;
        this->status_label.set_text("fingerprint ID needs to be between 0 and 50");
    }
    else
    {
        this->status_label.released_text_color = VGA_WHITE;
        this->status_label.set_text("click on add");
    }
}

void add_fingerprint::_exit_window(FGUI::Touch_Widget<add_fingerprint> *_widget)
{
    // PopUp_Window::_exit_pop_up_window(reinterpret_cast<FGUI::Touch_Widget<FGUI::PopUp_Window> *>(_widget));
    this->_get_main_window()->set_active_window(this->_get_parent_window());
}

void add_fingerprint::_add_fingerprint(FGUI::Touch_Widget<add_fingerprint> *_widget)
{
    const char *input_buffer = this->id.get_input_buffer();
    if (strlen(input_buffer) == 0)
    {
        this->status_label.released_text_color = VGA_RED;
        this->status_label.set_text("Please insert a valid ID");
        return;
    }

    this->add_fingerprint_id = String(input_buffer).toInt();

    if (add_fingerprint_id <= 120)
    {
        this->add_btn.hide();
        this->cancel_btn.show();
        this->id.disable();
        this->status_label.released_text_color = VGA_WHITE;
        this->status_label.set_text("add finger to reader");
        this->waiting_for_fingerprint_to_add = true;
    }
    else
    {
        this->status_label.released_text_color = VGA_RED;
        this->status_label.set_text("Required: 0 < ID < 120");
    }
}

void add_fingerprint::_cancel_add_fingerprint(FGUI::Touch_Widget<add_fingerprint> *_widget)
{
    this->waiting_for_fingerprint_to_add = false;
    this->cancel_btn.hide();
    this->add_btn.show();
    this->id.enable();
    this->status_label.set_text("click add");
}

void add_fingerprint::loop()
{
    if (this->waiting_for_fingerprint_to_add)
    {
        fingerprint->add_finger(this->add_fingerprint_id);
    }

    WindowBase::loop();
}