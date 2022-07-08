#pragma once
#include "UI.hpp"

// for button
UI::position upper_left{20, 40};
UI::position lower_right{130, 70};

void func()
{
}

class lock_screen : public UI::Window
{
public:
    lock_screen(UI::MainWindow *_main_window) : UI::Window(_main_window), _button(this, upper_left, lower_right, this), _button2(this, {100, 100}, {180, 140}, this)
    {
        _button.setText("Click mich");

        _button.on_click = &this->button_print_clicked;
        _button.on_release = &this->button_print_released;

        _button2.setText("button2");
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
#endif
    }

private:
    UI::Button<lock_screen> _button;
    UI::Button<lock_screen> _button2;
};

void lock_screen::show()
{
    _button.show();
    _button2.show();
}

void lock_screen::hide()
{
    _button.hide();
    _button2.hide();
}

void lock_screen::loop()
{
}
