#pragma once
#include "FGUI/FGUI.hpp"
#include "Pin.hpp"

class PIN_Settings : public FGUI::Window
{
public:
    PIN_Settings(FGUI::WindowBase *_parent_window);

    virtual ~PIN_Settings() {}

protected:
    void _exit_window(FGUI::Touch_Widget<PIN_Settings> *_widget);

    /**
     * handles the apply button touch and sets the Pin
     */
    void _handle_apply(FGUI::Touch_Widget<PIN_Settings> *_widget);

    /**
     * checks the input buffer of both input fields and compares them
     * disables and enables the apply button accordingly and prints to
     * status_label if they dont match
     */
    void _on_type_input_check(FGUI::Touch_Widget<PIN_Settings> *_widget, const char *_input_buffer);

private:
    FGUI::TextLabel window_title;
    FGUI::BitmapButton<PIN_Settings> back_btn;
    FGUI::Divider div;

    FGUI::TextLabel new_pin_label;
    FGUI::TextLabel new_pin_confirm_label;

    FGUI::InputField<_MAX_PIN_LN_, PIN_Settings> new_pin_input;
    FGUI::InputField<_MAX_PIN_LN_, PIN_Settings> new_pin_confirm_input;

    FGUI::TextLabel status_label;

    FGUI::Button<PIN_Settings> apply_btn;
};
