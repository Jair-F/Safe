#pragma once
#include "FGUI/FGUI.hpp"

class RFID_clear_database : public FGUI::PopUp_Window
{
public:
    RFID_clear_database(FGUI::WindowBase *_parent);

protected:
    void _clear_database(FGUI::Touch_Widget<RFID_clear_database> *_widget);
    void _exit_window(FGUI::Touch_Widget<RFID_clear_database> *_widget);

private:
    FGUI::TextLabel status_label;
    FGUI::Button<RFID_clear_database> clear_database_btn;
    FGUI::Button<RFID_clear_database> exit_window_btn;
};
