#pragma once
#include <Arduino.h>
#include <URTouch.h>
#include <UTFT.h>
#include "UI/MainWindow.hpp"

struct position
{
    unsigned int x_pos; // horizontal
    unsigned int y_pos; // vertical
};

enum Mode
{
    Normal,  // nothing - normal(not clicked or focused)
    Focused, // after clicking on it its focused - ready for input
    Clicked
};

class Widget
{
private:
    bool focused; // if the object has the focus - the last element that was clicked

protected:
    URTouch &touch;
    UTFT &display;

    virtual void _draw_released_widget() = 0;

    virtual void _draw_pressed_widget() {}

    position _upper_left_pos, _lower_right_pos;

public:
    void (*on_click)() = []() {};
    void (*on_release)() = []() {};
    Widget(position &_upper_left_pos, position &_lower_right_pos, UTFT &_display, URTouch &_touch);

    unsigned int width() { return _lower_right_pos.x_pos - _upper_left_pos.x_pos; }
    unsigned int height() { return _lower_right_pos.y_pos - _upper_left_pos.y_pos; }

    bool is_focused() { return this->focused; }

    /*
        @return the upper left position
    */
    const position &pos() const { return _upper_left_pos; }

    void loop();
};

Widget::Widget(position &_upper_left_pos, position &_lower_right_pos, UTFT &_display, URTouch &_touch) : touch(_touch), display(_display), _upper_left_pos(_upper_left_pos), _lower_right_pos(_lower_right_pos)
{
}

void Widget::loop()
{
    if (touch.dataAvailable())
    {
        touch.read();
        /*
            x and y axis are switched on my display
        */
        auto x_pos = 320 - touch.getY();
        auto y_pos = 240 - touch.getX(); // y-axis is rotated on my display and we have 240 pixels
        if (x_pos > display.disp_x_size || y_pos > display.disp_y_size)
        {
            return;
        }
        if (x_pos >= _upper_left_pos.x_pos && x_pos <= _lower_right_pos.x_pos) // x_pos matches
        {
            if (y_pos >= _upper_left_pos.y_pos && y_pos <= _lower_right_pos.y_pos)
            {
                this->_draw_pressed_widget();
                on_click();
                do
                {
                    touch.read();
                    x_pos = 320 - touch.getY();
                    y_pos = 240 - touch.getX();
                } while (touch.dataAvailable() && (x_pos >= _upper_left_pos.x_pos && x_pos <= _lower_right_pos.x_pos) && (y_pos >= _upper_left_pos.y_pos && y_pos <= _lower_right_pos.y_pos));

                on_release();
                this->_draw_released_widget();
            }
        }
        // Serial.println(x_pos);
        // Serial.println(y_pos);
        // Serial.println();
    }
}