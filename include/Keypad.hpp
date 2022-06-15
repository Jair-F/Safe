#pragma once
#include <Arduino.h>
#include <Adafruit_Keypad.h>
#include "GlobalVariables.hpp"
#include "GlobalConstants.hpp"

/*
    Library Reference: https://adafruit.github.io/Adafruit_Keypad/html/class_adafruit___keypad.html
*/
namespace Keypad
{
#define ROWS 4
#define COLUMNS 3
    /*
        KEYPAD_PID3844 for 4x4
        KEYPAD_PID1824 for 3x4

        #define KEYPAD_TYPE KEYPAD_PID KEYPAD_PID3844
    */

    byte ROW_PINS[ROWS] = {PIN_R1, PIN_R2, PIN_R3, PIN_R4};
    byte COLUMN_PINS[COLUMNS] = {PIN_C1, PIN_C2, PIN_C3};
    char KEYS[ROWS][COLUMNS] = {{'1', '2', '3'},
                                {'4', '5', '6'},
                                {'7', '8', '9'},
                                {'*', '0', '#'}};
#define CANCEL_KEY '*'
#define ENTER_KEY '#'

    class Keypad
    {
    public:
        Keypad();
        Keypad(char **userKeymap, byte *rows, byte *columns, int numRows, int numColumns);
        ~Keypad() {}

        void (*on_input)(char _input_data) = {};

        void loop();

        void begin();

        void (*buffer_changed)();

    private:
        Adafruit_Keypad keypad;
    };
} // namespace Keypad

Keypad::Keypad::Keypad(char **userKeymap, byte *rows,
                       byte *columns, int numRows, int numColumns) : keypad(makeKeymap(userKeymap), rows, columns, numRows, numColumns)
{
}

Keypad::Keypad::Keypad() : keypad(makeKeymap(KEYS), ROW_PINS, COLUMN_PINS, ROWS, COLUMNS)
{
}

void Keypad::Keypad::begin()
{
    this->keypad.begin();
}

void Keypad::Keypad::loop()
{
    this->keypad.tick(); // read for input

    if (this->keypad.available() >= 1) // some input in the buffer
    {
        keypadEvent key_event = this->keypad.read();
        if (key_event.bit.EVENT == 1) // a key was pressed - not released...
        {
            this->on_input(static_cast<char>(key_event.bit.KEY));
        }
    }
}

/*
char Keypad::Keypad::get()
{
    this->keypad.tick();

    if (this->keypad.available() >= 1)
    {
        keypadEvent key_event;
        key_event = this->keypad.read();
        if (key_event.bit.EVENT == 1) // a key was pressed - at releasing it is 0
        {
            return static_cast<char>(key_event.bit.KEY); // value of the pressed key
        }
    }
    return '\0';
}
*/
