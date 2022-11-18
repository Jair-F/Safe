#pragma once
#include "FGUI/FGUI.hpp"

class add_fingerprint : public FGUI::Window
{
public:
    add_fingerprint(FGUI::WindowBase *_parent_window);

    void loop() override;

protected:
    void _add_fingerprint(FGUI::Touch_Widget<add_fingerprint> *_widget);
    void _cancel_add_fingerprint(FGUI::Touch_Widget<add_fingerprint> *_widget);
    void _exit_window(FGUI::Touch_Widget<add_fingerprint> *_widget);
    void _on_type_tag_id_check(FGUI::Touch_Widget<add_fingerprint> *_widget, const char *_input_buffer);

private:
    FGUI::TextLabel window_title;
    // FGUI::Divider window_title_div;
    FGUI::TextLabel id_input_field_label;
    FGUI::InputField<3, add_fingerprint> id;
    FGUI::TextLabel status_label;
    FGUI::Button<add_fingerprint> cancel_btn; // will be show
    FGUI::Button<add_fingerprint> add_btn;
    FGUI::Button<add_fingerprint> exit_btn;

    bool waiting_for_fingerprint_to_add = false;
    uint8_t add_fingerprint_id; // id of the tag we should add
};
