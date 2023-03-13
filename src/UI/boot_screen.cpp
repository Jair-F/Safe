#include "UI/boot_screen.hpp"

// some fonts
extern uint8_t arial_bold[];   // big - good
extern uint8_t arial_normal[]; // big - ok
extern uint8_t Sinclair_S[];   // small - really small - bad...
extern uint8_t SmallFont[];    // small
extern uint8_t mykefont2[];    // small - really small - ok
extern uint8_t Retro8x16[];    // "small" - good
extern uint8_t TinyFont[];     // small - good

boot_screen::boot_screen(FGUI::MainWindow *_main_window) : Window(_main_window),
                                                           name_banner(this, {0, 60}, 319, "Safe", arial_bold),
                                                           boot_msg(this, {0, 160}, 319, "Loading...", Sinclair_S)
{
    this->name_banner.set_text_alignment(FGUI::TextLabel::text_alignment::AL_CENTER);
    this->name_banner.set_draw_border(false);

    this->boot_msg.set_text_alignment(FGUI::TextLabel::text_alignment::AL_CENTER);
    this->boot_msg.set_draw_border(false);
}

void boot_screen::loop()
{
    constexpr uint16_t d = 200;
    this->boot_msg.set_font(arial_bold);
    this->boot_msg.set_text("arial_bold ... ");
    this->_redraw_window();

    delay(d);

    this->boot_msg.set_font(arial_normal);
    this->boot_msg.set_text("arial_normal ... ");
    this->_redraw_window();

    delay(d);

    this->boot_msg.set_font(Sinclair_S);
    this->boot_msg.set_text("Sinclair_S ... ");
    this->_redraw_window();

    delay(d);

    this->boot_msg.set_font(SmallFont);
    this->boot_msg.set_text("SmallFont ... ");
    this->_redraw_window();

    delay(d);

    this->boot_msg.set_font(mykefont2);
    this->boot_msg.set_text("mykefont2 ... ");
    this->_redraw_window();

    delay(d);

    this->boot_msg.set_font(Retro8x16);
    this->boot_msg.set_text("Retro8x16 ... ");
    this->_redraw_window();

    delay(d);

    this->boot_msg.set_font(TinyFont);
    this->boot_msg.set_text("TinyFont ... ");
    this->_redraw_window();

    delay(d);
}