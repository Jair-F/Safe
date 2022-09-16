#include "UI/InfoPopUP.hpp"

#define INFO_OK_BTN_WIDTH 55
#define INFO_OK_BTN_HEIGHT 23
#define INFO_POP_UP_BORDER_WEIGHT 2

InfoPopUp::InfoPopUp(FGUI::WindowBase *_parent, uint16_t window_width,
                     uint16_t window_height, String _msg) : PopUp_Window(_parent, window_width, window_height, INFO_POP_UP_BORDER_WEIGHT),
                                                            info_msg(this, {0, 0}, window_width - INFO_POP_UP_BORDER_WEIGHT - 2, _msg),
                                                            ok_btn(this, {this->width() / 2 - INFO_OK_BTN_WIDTH / 2, this->info_msg.height() + 15},
                                                                   INFO_OK_BTN_WIDTH, INFO_OK_BTN_HEIGHT, this)
{
    this->hide_close_button();

    this->info_msg.set_draw_border(false);
    this->info_msg.set_border_weight(0);
    this->info_msg.set_text_alignment(FGUI::TextLabel::text_alignment::AL_CENTER);

    this->ok_btn.setText("ok");
    this->ok_btn.set_border_weight(2);
    this->ok_btn.on_release = &InfoPopUp::_exit_window;
}

void InfoPopUp::_exit_window(FGUI::Touch_Widget<InfoPopUp> *_widget)
{
    this->_get_parent_window()->hide_pop_up_window();
}