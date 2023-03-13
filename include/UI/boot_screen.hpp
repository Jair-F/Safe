#include "FGUI/FGUI.hpp"

class boot_screen : public FGUI::Window
{
public:
    boot_screen(FGUI::MainWindow *_main_window);

    void loop() override;

private:
    FGUI::TextLabel name_banner;
    FGUI::TextLabel boot_msg;
};
