#pragma once
#include "UI.hpp"
#include <UTFT.h>

// for button
UI::position upper_left{20, 40};
UI::position lower_right{130, 70};

extern const unsigned short settings_white[];
extern const unsigned short back_sign_white[];
extern const unsigned short settings_black[];

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
                                                status_bar(this, {0, this->_get_display()->getDisplayYSize() - 35}, this->_get_display()->getDisplayXSize() - 110, "ERROR"),
                                                b_button(this, {160, 40}, 50, 50, this, settings_white, settings_black),
                                                div(this, {5, 100}, 50, 5, UI::Divider::d_alignment::AL_VERTICAL, VGA_WHITE),
                                                progressBar(this, {80, 100}, {290, 115}, 1)
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

        status_bar.set_text("ERROR");
        status_bar.set_text_alignment(UI::TextLabel::text_alignment::AL_CENTER);
        status_bar.set_border(true);
        status_bar.set_font(SmallFont);

        b_button.on_release = &this->button_print_clicked;
        b_button.pressed_border_color = VGA_GREEN;
        b_button.set_border(true);

        progressBar.set_border_weight(2);
    }
    virtual ~lock_screen() {}

    void loop() override;

    friend void func();

protected:
    void update_status_bar(UI::Touch_Widget<lock_screen> *_widget)
    {
        this->status_bar.set_text(this->input_field.get_input_buffer());
    }

    void button_print_clicked(UI::Touch_Widget<lock_screen> *_widget)
    {
        // Serial.println("Clicked button");
        text_feld.set_text("button clicked");
        this->div.hide();
        if (this->div.height() < 50)
        {
            this->div.set_size(this->div.width(), this->div.height() + 10);
        }
        else
        {
            this->div.set_size(this->div.width(), this->div.height() - 10);
        }
        this->div.show();

        String tmp = this->input_field.get_input_buffer();
        uint8_t step = tmp.toInt();
        this->progressBar.set_progress(this->progressBar.get_progress() + step);
        if (this->progressBar.get_progress() >= 100)
        {
            this->progressBar.set_progress(0);
        }
    }

    void button_print_released(UI::Touch_Widget<lock_screen> *_widget)
    {
        // UI::Button *but_ptr = static_cast<UI::Button *>(widget);
        // Serial.println("Released button");
        text_feld.set_text("button is released");
        this->show_pop_up_window(&pop_up_window);
    }

    void _handle_check_box(UI::Touch_Widget<lock_screen> *_widget)
    {
        UI::CheckBox<lock_screen> *box = static_cast<UI::CheckBox<lock_screen> *>(_widget);
        // Serial.println("Checkbox is released");
        if (box->is_checked())
        {
            // Serial.println("Checkbox is checked");
            this->status_bar.set_text(this->input_field.get_input_buffer());
        }
        else
        {
            // Serial.println("Checkbox isnt checked");
            this->status_bar.set_text("Checkbox is released");
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

    UI::TextLabel status_bar;

    UI::BitMapButton<lock_screen> b_button;
    UI::Divider div;
    UI::ProgressBar progressBar;
};

void lock_screen::loop()
{
}
