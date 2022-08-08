#pragma once
#include <Arduino.h>
#include <Adafruit_Keypad.h>
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
#define BACKSPACE_KEY '*'
#define ENTER_KEY '#'

    template <typename CALL_OBJECT_TYPE>
    class Keypad
    {
    public:
        /*
            for using the default predefined layout with the default set pins
        */
        Keypad(CALL_OBJECT_TYPE *_call_object);
        /*
           for setting self the pinouts and layout
        */
        Keypad(char **userKeymap, byte *rows, byte *columns, int numRows, int numColumns, CALL_OBJECT_TYPE *_call_object);
        ~Keypad() {}

        void (CALL_OBJECT_TYPE::*on_input)(char _input_data) = nullptr;
        void (CALL_OBJECT_TYPE::*on_backspace)() = nullptr;
        void (CALL_OBJECT_TYPE::*on_enter)() = nullptr;

        void loop();

        void begin();

    private:
        Adafruit_Keypad keypad;
        CALL_OBJECT_TYPE *call_object;
    };
} // namespace Keypad
template <typename CALL_OBJECT_TYPE>
Keypad::Keypad<CALL_OBJECT_TYPE>::Keypad(char **userKeymap, byte *rows,
                                         byte *columns, int numRows, int numColumns,
                                         CALL_OBJECT_TYPE *_call_object) : keypad(makeKeymap(userKeymap), rows, columns, numRows, numColumns), call_object(_call_object)
{
}

template <typename CALL_OBJECT_TYPE>
Keypad::Keypad<CALL_OBJECT_TYPE>::Keypad(CALL_OBJECT_TYPE *_call_object) : keypad(makeKeymap(KEYS), ROW_PINS, COLUMN_PINS, ROWS, COLUMNS), call_object(_call_object)
{
}

template <typename CALL_OBJECT_TYPE>
void Keypad::Keypad<CALL_OBJECT_TYPE>::begin()
{
    this->keypad.begin();
}

template <typename CALL_OBJECT_TYPE>
void Keypad::Keypad<CALL_OBJECT_TYPE>::loop()
{
    this->keypad.tick(); // read for input

    if (this->keypad.available() >= 1) // some input in the buffer
    {
        keypadEvent key_event = this->keypad.read();
        if (key_event.bit.EVENT == KEY_JUST_PRESSED) // a key was pressed - not released...
        {
            char pressed_key = static_cast<char>(key_event.bit.KEY);
            if (pressed_key == BACKSPACE_KEY)
            {
                if (this->on_backspace != nullptr)
                    (this->call_object->*this->on_backspace)();
            }
            else if (pressed_key == ENTER_KEY)
            {
                if (this->on_enter != nullptr)
                    (this->call_object->*this->on_enter)();
            }
            else
            {
                if (this->on_input != nullptr)
                    (this->call_object->*this->on_input)(pressed_key);
            }
        }

        Serial.println("keypad-input: ");
        Serial.print("Key: ");
        Serial.println(static_cast<char>(key_event.bit.KEY));

        Serial.print("Key-Event: ");
        Serial.print(key_event.bit.EVENT);
        Serial.print(" -> ");
        Serial.println(key_event.bit.EVENT == KEY_JUST_PRESSED ? "pressed" : "released");
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
