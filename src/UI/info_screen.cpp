#include "UI/info_screen.hpp"

extern const unsigned short back_icon[];
extern const unsigned short src_code_qr_code[];

Info_Screen::Info_Screen(FGUI::WindowBase *_parent_window) : FGUI::Window(_parent_window),
                                                             window_title(this, {0, 0}, 319, "INFO", arial_normal),
                                                             div(this, {160, 34}, 193, 4, FGUI::Divider::d_alignment::AL_VERTICAL, VGA_GRAY),
                                                             back_btn(this, {0, 0}, 40, 40, this, back_icon, (uint8_t)0, (uint8_t)0),
                                                             qr_code(this, {15, 68}, 120, 120, src_code_qr_code, 3, 3),
                                                             qr_code_label(this, {20, 210}, 120, "Source Code", SmallFont),
                                                             info_label(this, {174, 97}, 141, "Safe-OS: v1.0\nFGUI:    v1.0\nAuthor:  Jair F.", SmallFont)
{
    this->window_title.set_draw_border(false);
    this->window_title.set_text_alignment(FGUI::TextLabel::text_alignment::AL_CENTER);

    this->back_btn.on_release = &Info_Screen::_go_back;

    Serial.print("QR_Code_size: ");
    Serial.print(this->qr_code.get_bitmap_height());
    Serial.print('x');
    Serial.print(this->qr_code.get_bitmap_width());
    Serial.println();

    this->qr_code_label.set_draw_border(false);
    this->qr_code_label.set_text_alignment(FGUI::TextLabel::text_alignment::AL_CENTER);

    this->info_label.set_draw_border(false);
}

void Info_Screen::_go_back(FGUI::Touch_Widget<Info_Screen> *_widget)
{
    this->_get_main_window()->set_active_window(this->_get_parent_window());
}