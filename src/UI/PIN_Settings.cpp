#include "UI/PIN_Settings.hpp"
#include "Pin.hpp"
#include "Config.hpp"

extern Pin pin;

extern const unsigned short back_33_33[];

PIN_Settings::PIN_Settings(FGUI::WindowBase *_parent_window) : FGUI::Window(_parent_window),
                                                               window_title(this, {33, 0}, 286, "PIN", arial_bold, 0, 0),
                                                               back_btn(this, {0, 0}, 33, 33, this, back_33_33, (uint8_t)0, (uint8_t)0),
                                                               div(this, {0, 34}, 319, 3, FGUI::Divider::d_alignment::AL_HORIZONTAL, VGA_BLUE),
                                                               new_pin_label(this, {45, 59}, 160, "new pin:", SmallFont, 0, 0),
                                                               new_pin_confirm_label(this, {45, 112}, 160, "confirm new pin:", SmallFont, 0, 0),
                                                               new_pin_input(this, {220, 51}, 54, 29, this, FGUI::InputField<(uint8_t)_MAX_PIN_LN_, PIN_Settings>::IN_INPUT_TYPE::IN_PASSWORD, 2),
                                                               new_pin_confirm_input(this, {220, 104}, 54, 29, this, FGUI::InputField<(uint8_t)_MAX_PIN_LN_, PIN_Settings>::IN_INPUT_TYPE::IN_PASSWORD, 2),
                                                               status_label(this, {0, 148}, 320, "", SmallFont, 0, 0),
                                                               apply_btn(this, {126, 198}, 67, 28, this, 3, 2)
{
    this->window_title.set_draw_border(false);
    this->window_title.set_text_alignment(FGUI::TextLabel::text_alignment::AL_CENTER);

    this->back_btn.on_release = &PIN_Settings::_exit_window;

    this->new_pin_confirm_label.show();
    this->new_pin_confirm_label.released_text_color = VGA_WHITE;

    this->new_pin_input.touched_border_color = 0x998f;
    this->new_pin_input.on_typing = &PIN_Settings::_on_type_input_check;

    this->new_pin_confirm_input.on_typing = &PIN_Settings::_on_type_input_check;
    this->new_pin_confirm_input.touched_border_color = 0x998f;

    this->status_label.set_text_alignment(FGUI::TextLabel::text_alignment::AL_CENTER);

    this->apply_btn.setText("apply");
    this->apply_btn.mark_disabled();
    this->apply_btn.touched_border_color = 0x998f;
    this->apply_btn.on_release = &PIN_Settings::_handle_apply;
}

void PIN_Settings::_exit_window(FGUI::Touch_Widget<PIN_Settings> *_widget)
{
    this->_get_main_window()->set_active_window(this->_get_parent_window());
}

void PIN_Settings::_handle_apply(FGUI::Touch_Widget<PIN_Settings> *_widget)
{
    pin.set_pin(this->new_pin_input.get_input_buffer());
    this->status_label.released_text_color = VGA_GREEN;
    this->status_label.set_text("pin changed");

    this->new_pin_input.clear_input_buffer();
    this->new_pin_confirm_input.clear_input_buffer();
    
    this->apply_btn.disable();

    this->new_pin_input.draw();
    this->new_pin_confirm_input.draw();
    this->status_label.draw();
}

void PIN_Settings::_on_type_input_check(FGUI::Touch_Widget<PIN_Settings> *_widget, const char *_input_buffer)
{
    // String new_pin = this->new_pin_input.get_input_buffer();
    // String new_pin_confirm = this->new_pin_confirm_input.get_input_buffer();

    if (strlen(this->new_pin_input.get_input_buffer()) > 0 &&
        strcmp(this->new_pin_input.get_input_buffer(), this->new_pin_confirm_input.get_input_buffer()) > 0)
    {
        this->apply_btn.disable();
        this->status_label.released_text_color = VGA_RED;
        this->status_label.set_text("inputs doesnt match");
    }
    else
    {
        this->apply_btn.enable();
        this->status_label.set_text("");
    }
    this->status_label.draw();
}
