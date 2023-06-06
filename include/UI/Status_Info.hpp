#pragma once
#include "FGUI/FGUI.hpp"
#include "logging/Log.hpp"

extern Log::Log logger;

class Status_Info_window : public FGUI::Window
{
public:
    Status_Info_window(FGUI::WindowBase *_parent_window);

    virtual ~Status_Info_window() {}

protected:
    void _exit_window(FGUI::Touch_Widget<Status_Info_window> *_widget);
    void _pre_show() override; // reads the logger before show

private:
    FGUI::TextLabel window_title;
    FGUI::BitmapButton<Status_Info_window> back_btn;
    FGUI::Divider div;

    FGUI::TextLabel info_label;
};
