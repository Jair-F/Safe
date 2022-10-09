#include "UI/RFID_clear_database.hpp"
#include "RFID/RFID.hpp"

// extern RFID::RFID rfid;
extern uint8_t mykefont2[];
extern uint8_t Retro8x16[];
extern uint8_t Sinclair_Inverted_S[];
extern uint8_t TinyFont[];

RFID_clear_database::RFID_clear_database(FGUI::WindowBase *_parent) : PopUp_Window(_parent, 240, 170, 3, false),
                                                                      status_label(this, {0, 0}, this->get_content_width() - 10, "do you really want to clear the database??", BigFont),
                                                                      clear_database_btn(this, {20, 120}, 70, 30, this),
                                                                      exit_window_btn(this, {110, 120}, 70, 30, this)
{
    this->status_label.released_text_color = 0xfa69;
    this->status_label.set_draw_border(false);

    this->clear_database_btn.setText("clear");
    this->clear_database_btn.set_border_weight(2);
    this->clear_database_btn.on_release = &RFID_clear_database::_clear_database;

    this->exit_window_btn.setText("exit");
    this->exit_window_btn.set_border_weight(2);
    this->exit_window_btn.on_release = &RFID_clear_database::_exit_window;
}

void RFID_clear_database::_exit_window(FGUI::Touch_Widget<RFID_clear_database> *_widget)
{
    this->status_label.released_text_color = VGA_RED;
    this->status_label.set_text("do you really want to clear the database??");

    PopUp_Window::_exit_pop_up_window(reinterpret_cast<FGUI::Touch_Widget<PopUp_Window> *>(_widget));
}

void RFID_clear_database::_clear_database(FGUI::Touch_Widget<RFID_clear_database> *_widget)
{
    // rfid.clear_database();
    this->status_label.released_text_color = VGA_GREEN;
    this->status_label.set_text("Database was cleared");
}
