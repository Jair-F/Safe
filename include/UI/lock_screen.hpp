#pragma once
#include "UI.hpp"

// for button
UI::position upper_left{20, 40};
UI::position lower_right{130, 70};

class lock_screen : public UI::Window
{
public:
    lock_screen(UI::MainWindow *_main_window, UI::position upper_left, uint16_t width, uint16_t height) : UI::Window(_main_window, upper_left, width, height), _button(this, upper_left, lower_right, myGLCD, myTouch), _button2(this, {100, 100}, {180, 140}, myGLCD, myTouch)
    {
        _button.setText("Click mich");

        _button.on_click = []()
        {
#ifdef DEBUG
            debug_message("Clicked button");
#else
            Serial.println("Clicked button");
#endif
        };
        _button.on_release = []()
        {
#ifdef DEBUG
            debug_message("Released button");
#else
            Serial.println("Released button");
#endif
        };

        _button2.setText("button2");
    }
    virtual ~lock_screen() {}

    void show() override;
    void hide() override;
    void loop() override;

private:
    UI::Button _button;
    UI::Button _button2;
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
    _button.loop();
}
