#pragma once
#include <Arduino.h>
#include "FGUI/FGUI.hpp"

class InfoPopUp : public FGUI::PopUp_Window
{
public:
    InfoPopUp(FGUI::WindowBase *_parent, uint16_t window_width, uint16_t window_height, String _msg, FGUI::font_type text_font);
    void set_info_text(String _msg);

protected:
    void _exit_window(FGUI::Touch_Widget<InfoPopUp> *_widget);

private:
    FGUI::TextLabel window_title;
    FGUI::TextLabel info_msg;
    FGUI::Button<InfoPopUp> ok_btn;
};
