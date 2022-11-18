#pragma once
#include "FGUI/FGUI.hpp"
#include "InfoPopUP.hpp"

class RFID_add_tag_window : public FGUI::Window
{
public:
    RFID_add_tag_window(FGUI::WindowBase *_parent_window);

    void loop() override;

protected:
    void _add_tag(FGUI::Touch_Widget<RFID_add_tag_window> *_widget);
    void _cancel_add_tag(FGUI::Touch_Widget<RFID_add_tag_window> *_widget);
    void _exit_window(FGUI::Touch_Widget<RFID_add_tag_window> *_widget);

    void _on_type_tag_id_check(FGUI::Touch_Widget<RFID_add_tag_window> *_widget, const char *_input_buffer);

private:
    FGUI::TextLabel window_title;
    // FGUI::Divider window_title_div;
    FGUI::TextLabel tag_id_input_field_label;
    FGUI::InputField<2, RFID_add_tag_window> tag_id;
    FGUI::TextLabel status_label;
    FGUI::Button<RFID_add_tag_window> cancel_btn; // will be show
    FGUI::Button<RFID_add_tag_window> add_btn;
    FGUI::Button<RFID_add_tag_window> exit_btn;

    bool waiting_for_tag_to_add = false;
    uint8_t add_tag_id; // id of the tag we should add
};
