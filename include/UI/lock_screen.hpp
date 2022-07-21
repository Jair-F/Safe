#pragma once
#include "UI.hpp"
#include <UTFT.h>

// for button
UI::position upper_left{20, 40};
UI::position lower_right{130, 70};

class lock_screen : public UI::Window
{
public:
    lock_screen(UI::MainWindow *_main_window) : UI::Window(_main_window),
                                                _button(this, ::upper_left, ::lower_right, this),
                                                _button2(this, {this->_get_display()->getDisplayXSize() - 100, this->_get_display()->getDisplayYSize() - 35}, {this->_get_display()->getDisplayXSize() - 1, this->_get_display()->getDisplayYSize() - 1}, this),
                                                text_feld(this, {0, 0}, this->_get_display()->getDisplayXSize() - 1, "Window title"),
                                                ch_box(this, {50, 150}, 25, 25, this),
                                                input_field(this, {150, 150}, 60, 35, this, UI::InputField<20, lock_screen>::IN_INPUT_TYPE::IN_TEXT),
                                                pop_up_window(this, 150, 100),
                                                exit_button(&pop_up_window, {1, 1}, {65, 25}, this),
                                                begin(this, {210, 40}, {300, 70}, this),
                                                c_button(this, {210, 80}, 20, this)
    {
        text_feld.set_border(false);
        text_feld.set_text_alignment(text_feld.AL_CENTER);
        text_feld.set_font(SmallFont);

        _button.setText("Click mich");
        _button.on_touch = &this->button_print_clicked;
        _button.on_release = &this->button_print_released;

        _button2.on_release = &this->restart_system;
        _button2.setText("Restart");

        ch_box.on_release = &this->_handle_check_box;
        ch_box.check_sign_color = VGA_WHITE;
        ch_box.background_color = VGA_GREEN;
        ch_box.border_color = VGA_AQUA;
        ch_box.set_checked(false);

        input_field.set_input_buffer("123");
        input_field.on_enter = &this->update_window_label;
        input_field.on_focus_lose = &this->update_window_label;

        pop_up_window.set_background_color(VGA_GRAY);
        pop_up_window.set_border_color(VGA_LIME);

        exit_button.setText("exit");
        exit_button.on_release = &this->exit_pop_up;

        begin.setText("Begin");
        begin.released_background_color = VGA_RED;
        begin.pressed_background_color = VGA_GREEN;
        begin.on_release = &this->begin_release;
    }
    virtual ~lock_screen() {}

    void loop() override;

    friend void func();

protected:
    void exit_pop_up(UI::Touch_Widget<lock_screen> *_widget)
    {
        this->hide_pop_up_window();
    }

    void begin_release(UI::Touch_Widget<lock_screen> *_widget)
    {
        this->text_feld.set_text("Begin");
    }

    void button_print_clicked(UI::Touch_Widget<lock_screen> *_widget)
    {
        Serial.println("Clicked button");
        text_feld.set_text("button clicked");
    }

    void button_print_released(UI::Touch_Widget<lock_screen> *_widget)
    {
        // UI::Button *but_ptr = static_cast<UI::Button *>(widget);
        Serial.println("Released button");
        text_feld.set_text("button is released");

        this->show_pop_up_window(&pop_up_window);
    }

    void _handle_check_box(UI::Touch_Widget<lock_screen> *_widget)
    {
        UI::CheckBox<lock_screen> *box = static_cast<UI::CheckBox<lock_screen> *>(_widget);
        Serial.println("Checkbox is released");
        if (box->is_checked())
        {
            Serial.println("Checkbox is checked");
        }
        else
        {
            Serial.println("Checkbox isnt checked");
        }
    }

    void restart_system(UI::Touch_Widget<lock_screen> *_widget)
    {
        void (*reset_func)() = 0;
        reset_func();
    }

    void update_window_label(UI::Touch_Widget<lock_screen> *_widget)
    {
        this->text_feld.set_text(this->input_field.get_input_buffer());
        this->text_feld.update_widget();
    }

private:
    UI::Button<lock_screen> _button;
    UI::Button<lock_screen> _button2;
    UI::TextLabel text_feld;
    UI::CheckBox<lock_screen> ch_box;
    UI::InputField<20, lock_screen> input_field;
    UI::PopUp_Window pop_up_window;

    UI::Button<lock_screen> exit_button;

    UI::Button<lock_screen> begin;

    UI::CloseButton<lock_screen> c_button;
};

void lock_screen::loop()
{
}
