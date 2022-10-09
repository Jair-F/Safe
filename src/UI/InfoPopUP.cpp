#include "UI/InfoPopUP.hpp"

#define INFO_OK_BTN_WIDTH 45
#define INFO_OK_BTN_HEIGHT 25
#define INFO_POP_UP_BORDER_WEIGHT 2

InfoPopUp::InfoPopUp(FGUI::WindowBase *_parent, uint16_t window_width, uint16_t window_height, String _msg,
                     FGUI::font_type text_font) : FGUI::PopUp_Window(_parent, window_width, window_height, INFO_POP_UP_BORDER_WEIGHT),
                                                  window_title(this, {0, 0}, this->get_content_width() - this->_get_close_button_size() - 1, "Info", SmallFont),
                                                  info_msg(this, {0, this->window_title.height() + 2},
                                                           this->get_content_width() - this->get_border_weight(), _msg, text_font),
                                                  ok_btn(this, {this->get_content_width() / 2 - INFO_OK_BTN_WIDTH / 2, this->get_content_height() - INFO_OK_BTN_HEIGHT - 1},
                                                         INFO_OK_BTN_WIDTH, INFO_OK_BTN_HEIGHT, this)
{
    this->window_title.set_text_alignment(FGUI::TextLabel::text_alignment::AL_LEFT);
    this->window_title.set_draw_border(false);
    this->window_title.released_border_color = this->get_border_color();

    this->info_msg.set_draw_border(false);
    this->info_msg.set_border_weight(0);
    this->info_msg.set_text_alignment(FGUI::TextLabel::text_alignment::AL_LEFT);

    this->ok_btn.setText("ok");
    this->ok_btn.set_border_weight(2);
    this->ok_btn.on_release = &InfoPopUp::_exit_window;
}

void InfoPopUp::_exit_window(FGUI::Touch_Widget<InfoPopUp> *_widget)
{
    this->_exit_pop_up_window(reinterpret_cast<FGUI::Touch_Widget<PopUp_Window> *>(_widget));
}

void InfoPopUp::set_info_text(String _msg)
{
    this->info_msg.set_text(_msg);
}
