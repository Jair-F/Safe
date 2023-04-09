#include "FGUI/FGUI.hpp"

class Info_Screen : public FGUI::Window
{
public:
    Info_Screen(FGUI::WindowBase *_parent_window);
    virtual ~Info_Screen() {}

protected:
    void _go_back(FGUI::Touch_Widget<Info_Screen> *_widget); // if back_btn is triggered
private:
    FGUI::TextLabel window_title;
    FGUI::Divider div; // vertical split
    FGUI::BitmapButton<Info_Screen> back_btn;
    FGUI::Bitmap qr_code;
    FGUI::TextLabel qr_code_label;
    FGUI::TextLabel info_label; // where the version numbers and author is written
};