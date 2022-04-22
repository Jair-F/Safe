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
#define COLUMNS 4
    /*
        KEYPAD_PID3844 for 4x4
        KEYPAD_PID1824 for 3x4

        #define KEYPAD_TYPE KEYPAD_PID KEYPAD_PID3844
    */

    byte ROW_PINS[ROWS] = {PIN_R1, PIN_R2, PIN_R3, PIN_R4};
    byte COLUMN_PINS[COLUMNS] = {PIN_C1, PIN_C2, PIN_C3, PIN_C4};
    char KEYS[ROWS][COLUMNS] = {{'1', '2', '3', 'A'},
                                {'4', '5', '6', 'B'},
                                {'7', '8', '9', 'C'},
                                {'*', '0', '#', 'D'}};
#define CANCEL_KEY '*'
#define ENTER_KEY '#'

    class Keypad
    {
    public:
        Keypad();
        Keypad(char **userKeymap, byte *rows, byte *columns, int numRows, int numColumns);
        ~Keypad() {}

        /*
            for reading the keyboard in a loop and filling the buffer...
        */
        void read();
        String get_buffer()
        {
            String tmp = this->key_buffer;
            this->clear_buffer();
            return tmp;
        }

        void clear_buffer() { this->key_buffer = ""; }

        /*
            read the buffer without clearing him - !! you'l get a pointer dont modify the buffer !!
        */
        const String *const peek_buffer() { return &key_buffer; }

        /*
            read a key without adding it to the buffer
            @return if no key was pressed '\0' else the key_value
        */
        char get();

        /*
            @return true if buffer is empty
        */
        bool is_buffer_empty() { return this->key_buffer.length() == 0; }

        void begin();

        void (*buffer_changed)();

    private:
        Adafruit_Keypad keypad;
        String key_buffer; // buffer which fills until the user presses "Enter"
        const byte max_bufer_length = 50;

        unsigned long last_read_timepoint = 0;   // this is the last timepoint of keyboard reading
        const unsigned short reading_loop = 300; // the keypad reads only every 300 milliseconds to prevent continiously reading of the same key by pressing it only once
    };
} // namespace Keypad

Keypad::Keypad::Keypad(char **userKeymap, byte *rows,
                       byte *columns, int numRows, int numColumns) : keypad(makeKeymap(userKeymap), rows, columns, numRows, numColumns), key_buffer("")
{
}

Keypad::Keypad::Keypad() : keypad(makeKeymap(KEYS), ROW_PINS, COLUMN_PINS, ROWS, COLUMNS), key_buffer(F(""))
{
}

void Keypad::Keypad::begin()
{
    this->keypad.begin();
}

char Keypad::Keypad::get()
{
    keypadEvent key_event;

    key_event = this->keypad.read();
    if (key_event.bit.EVENT == 1) // a key was pressed - at releasing it is 0
    {
        return key_event.bit.KEY; // value of the pressed key
    }
    else
    {
        return '\0';
    }
}

void Keypad::Keypad::read()
{
    if (this->last_read_timepoint + this->reading_loop < millis())
    {
        this->keypad.tick(); // read the keypad
        last_read_timepoint = millis();
    }

    char key_value = this->get();
    if (key_value != '\0') // a key was pressed - at releasing it is 0
    {
        switch (key_value)
        {
        case CANCEL_KEY:
        {
            if (this->key_buffer.length() > 0)
                this->key_buffer.remove(this->key_buffer.length() - 1); // remove last char
            break;
        }
        case ENTER_KEY:
        {
            // call the on_enter func
        }
        default:
        {
            if (this->key_buffer.length() < this->max_bufer_length + 1)
            {
                this->key_buffer += key_value;
            }
            break;
        }
        }
    }
}
