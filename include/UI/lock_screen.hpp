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
                                                text_feld(this, {0, 0}, this->_get_display()->getDisplayYSize() - 1, "Window title"),
                                                ch_box(this, {50, 150}, 25, 25, this),
                                                input_field(this, {150, 150}, 60, 35, this, UI::InputField<10, '*', lock_screen>::IN_INPUT_TYPE::IN_DIGIT)
    {
        text_feld.set_border(false);
        text_feld.set_text_alignment(text_feld.AL_CENTER);
        text_feld.set_font(SmallFont);

        _button.setText("Click mich");
        _button.on_click = &this->button_print_clicked;
        _button.on_release = &this->button_print_released;

        _button2.on_release = &this->restart_system;
        _button2.setText("Restart");

        ch_box.on_release = &this->_handle_check_box;
        ch_box.check_sign_color = VGA_WHITE;
        ch_box.background_color = VGA_GREEN;
        ch_box.border_color = VGA_AQUA;
        ch_box.set_checked(false);

        input_field.set_input_buffer("123");
    }
    virtual ~lock_screen() {}

    void show() override;
    void hide() override;
    void loop() override;

    friend void func();

protected:
    void button_print_clicked(UI::Touch_Widget<lock_screen> *_widget)
    {
#ifdef DEBUG
        debug_message("Clicked button");
#else
        Serial.println("Clicked button");
        text_feld.set_text("button clicked");
#endif
    }

    void button_print_released(UI::Touch_Widget<lock_screen> *_widget)
    {
        // UI::Button *but_ptr = static_cast<UI::Button *>(widget);
        // but_ptr->
#ifdef DEBUG
        debug_message("Released button");
#else
        Serial.println("Released button");
        text_feld.set_text("button is released");
#endif
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

private:
    UI::Button<lock_screen> _button;
    UI::Button<lock_screen> _button2;
    UI::TextLabel text_feld;
    UI::CheckBox<lock_screen> ch_box;
    UI::InputField<10, '*', lock_screen> input_field;
};

void lock_screen::show()
{
    _button.show();
    _button2.show();
    text_feld.show();
    ch_box.show();
    input_field.show();
}

void lock_screen::hide()
{
    _button.hide();
    _button2.hide();
    text_feld.hide();
    input_field.hide();
}

void lock_screen::loop()
{
}
