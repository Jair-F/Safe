#pragma once
#include "FGUI/FGUI.hpp"

class RFID_add_tag_pop_up : public FGUI::PopUp_Window
{
public:
    RFID_add_tag_pop_up(WindowBase *_parent_window);

    void loop() override;

protected:
    void _exit_window(FGUI::Touch_Widget<RFID_add_tag_pop_up> *_widget);

private:
    FGUI::Button<RFID_add_tag_pop_up> cancel_btn;
    // if the tag was added this button will be shown to exit the window
    FGUI::Button<RFID_add_tag_pop_up> ok_btn;
    FGUI::TextLabel status_label;

    FGUI::InputField<2, RFID_add_tag_pop_up> tag_id;
};
