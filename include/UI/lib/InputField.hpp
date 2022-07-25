#pragma once

#include "Touch_Widget.hpp"
#include "Window.hpp"

namespace UI
{
    /*
        @param MAX_NUM_OF_CHARS maximum number of input-digits/chars/letters that the input_field can storage and will accept. If the user
                tries to input more the field will simply not accept more.
    */
    template <uint8_t MAX_NUM_OF_CHARS, typename CALL_OBJECT_TYPE>
    class InputField : public Touch_Widget<CALL_OBJECT_TYPE>
    {
    public:
        enum class IN_INPUT_TYPE
        {
            IN_TEXT,
            IN_PASSWORD
        };

        void (CALL_OBJECT_TYPE::*on_enter)(Touch_Widget<CALL_OBJECT_TYPE> *_widget) = nullptr;

        InputField(Window *_parent, const position _upper_left, const position _lower_right,
                   CALL_OBJECT_TYPE *_call_object, IN_INPUT_TYPE _input_type);
        InputField(Window *_parent, const position _upper_left, uint16_t _width, uint16_t _height,
                   CALL_OBJECT_TYPE *_call_object, IN_INPUT_TYPE _input_type);

        virtual ~InputField() {}

        // function called from the keypad from the MainWindow
        void send_input(char _input_data) override;
        void send_backspace() override;
        void send_enter() override;

        /*
            @return pointer to the input_buffer of the class - make sure that the instance of the InputField class exists at accessing the data!!!
        */
        const char *get_input_buffer() { return this->input_buffer; }
        void set_input_buffer(String _input_data);
        void set_input_buffer(const char *_input_data);
        void clear_input_buffer();

        /*
            @return true if the buffer is empty
        */
        bool buffer_is_empty();

        void set_font(uint8_t *_font) { this->_text_font = _font; }
        uint8_t *get_font() { return this->_text_font; }

    protected:
        void _draw_widget() override;
        void _draw_released_content() override;
        /*
            at this time this isnt when the widget is pressed. its when the widget is focused and ready for input - then we show the cursor
        */
        void _draw_touched_content() override;

        void _focus_lose() override;

    private:
        char input_buffer[MAX_NUM_OF_CHARS];

        IN_INPUT_TYPE input_type;

        /*
            the value which is set to the input_buffer[i] if it is not set via the input. - we know the size of the input_buffer-array so
            we can set it to '\0' to indicate that this element is not set
        */
        static constexpr char INPUT_UNSET_VALUE = '\0';

        /*
            the color values are RGB-565 values(16-bit value)
            RGB-565 color picker: https://chrishewett.com/blog/true-rgb565-colour-picker/
        */
        unsigned int touched_text_color = VGA_BLACK;
        unsigned int touched_cursor_color = VGA_BLACK;
        unsigned int released_text_color = VGA_WHITE;

        uint8_t *_text_font = SmallFont;

        uint8_t _text_gap; // gap between the left border and the start of the text and the right border and the end of the text
    };
} // namespace UI

// ------------- template implementation -------------

template <uint8_t MAX_NUM_OF_CHARS, typename CALL_OBJECT_TYPE>
UI::InputField<MAX_NUM_OF_CHARS, CALL_OBJECT_TYPE>::InputField(Window *_parent, const position _upper_left, const position _lower_right,
                                                               CALL_OBJECT_TYPE *_call_object, IN_INPUT_TYPE _input_type) : Touch_Widget<CALL_OBJECT_TYPE>(_parent, _upper_left, _lower_right, _call_object),
                                                                                                                            input_type(_input_type)
{
    assert(MAX_NUM_OF_CHARS > 0);

    this->_text_gap = 0.5 * this->display->getFontXsize(); // 1/2 of the font width

    // initializing input_buffer with default unset values
    for (uint8_t i = 0; i < MAX_NUM_OF_CHARS; ++i)
    {
        this->input_buffer[i] = this->INPUT_UNSET_VALUE;
    }
}

template <uint8_t MAX_NUM_OF_CHARS, typename CALL_OBJECT_TYPE>
UI::InputField<MAX_NUM_OF_CHARS, CALL_OBJECT_TYPE>::InputField(Window *_parent, const position _upper_left, uint16_t _width, uint16_t _height, CALL_OBJECT_TYPE *_call_object,
                                                               IN_INPUT_TYPE _input_type) : InputField<MAX_NUM_OF_CHARS, CALL_OBJECT_TYPE>(_parent, _upper_left, {_upper_left.x_pos + _width, _upper_left.y_pos + _height}, _call_object, _input_type)
{
}

template <uint8_t MAX_NUM_OF_CHARS, typename CALL_OBJECT_TYPE>
void UI::InputField<MAX_NUM_OF_CHARS, CALL_OBJECT_TYPE>::send_input(char _input_data)
{
    if (_input_data != this->INPUT_UNSET_VALUE) // INPUT_UNSET_VALUE is not allowed...
    {
        for (uint8_t i = 0; i < MAX_NUM_OF_CHARS; ++i)
        {
            if (this->input_buffer[i] == this->INPUT_UNSET_VALUE)
            {
                this->input_buffer[i] = _input_data;
                break;
            }
        }
    }
    this->_draw_widget();
}

template <uint8_t MAX_NUM_OF_CHARS, typename CALL_OBJECT_TYPE>
void UI::InputField<MAX_NUM_OF_CHARS, CALL_OBJECT_TYPE>::send_backspace()
{
    // searching for the last set input-pos
    uint8_t i = 0;
    for (; i < MAX_NUM_OF_CHARS; ++i)
    {
        if (this->input_buffer[i] == this->INPUT_UNSET_VALUE)
        {
            break;
        }
    }
    --i;
    this->input_buffer[i] = this->INPUT_UNSET_VALUE;
    this->_draw_widget();
}

template <uint8_t MAX_NUM_OF_CHARS, typename CALL_OBJECT_TYPE>
void UI::InputField<MAX_NUM_OF_CHARS, CALL_OBJECT_TYPE>::send_enter()
{
    if (this->on_enter != nullptr)
        (this->call_object->*on_enter)(this);
}

template <uint8_t MAX_NUM_OF_CHARS, typename CALL_OBJECT_TYPE>
void UI::InputField<MAX_NUM_OF_CHARS, CALL_OBJECT_TYPE>::clear_input_buffer()
{
    for (uint8_t i = 0; i < MAX_NUM_OF_CHARS; ++i)
    {
        this->input_buffer[i] = this->INPUT_UNSET_VALUE;
    }
}

template <uint8_t MAX_NUM_OF_CHARS, typename CALL_OBJECT_TYPE>
void UI::InputField<MAX_NUM_OF_CHARS, CALL_OBJECT_TYPE>::set_input_buffer(const char *_input_data)
{
    size_t _input_data_length = strlen(_input_data);

    uint8_t i = 0;
    for (; i < (_input_data_length < MAX_NUM_OF_CHARS ? _input_data_length : MAX_NUM_OF_CHARS); ++i) // ensure that we dont go over _input_data buffer/length and not over this->input_buffer
    {
        this->input_buffer[i] = _input_data[i];
    }

    // filling up the buffer with unset_values from the position we stopped above and continue until the end of the input_buffer
    for (; i < MAX_NUM_OF_CHARS; ++i)
    {
        this->input_buffer[i] = this->INPUT_UNSET_VALUE;
    }
}

template <uint8_t MAX_NUM_OF_CHARS, typename CALL_OBJECT_TYPE>
void UI::InputField<MAX_NUM_OF_CHARS, CALL_OBJECT_TYPE>::set_input_buffer(String _input_buffer)
{
    this->set_input_buffer(_input_buffer.c_str());
}

template <uint8_t MAX_NUM_OF_CHARS, typename CALL_OBJECT_TYPE>
bool UI::InputField<MAX_NUM_OF_CHARS, CALL_OBJECT_TYPE>::buffer_is_empty()
{
    // we fill the buffer from pos-0 to ...
    return this->input_buffer[0] == this->INPUT_UNSET_VALUE;
}

template <uint8_t MAX_NUM_OF_CHARS, typename CALL_OBJECT_TYPE>
void UI::InputField<MAX_NUM_OF_CHARS, CALL_OBJECT_TYPE>::_draw_released_content()
{
    // print the print the text in the InputField if there is text in the buffer
    if (!this->buffer_is_empty())
    {
        this->display->setFont(this->_text_font);
        uint8_t font_height = this->display->getFontYsize();
        uint8_t font_width = this->display->getFontXsize();

        this->display->setBackColor(this->released_background_color);
        this->display->setColor(this->released_text_color);

        // calculate the num of chars to print in the field - in case there is more input than the widget ca display
        uint8_t input_buffer_size = strlen(this->input_buffer);

        uint8_t space_for_text = (this->width() - this->_text_gap * 2); // in pixels
        uint8_t num_of_chars_to_print = space_for_text / font_width;    // num of characters we can print in the widget
        String text_to_print;

        if (input_buffer_size < num_of_chars_to_print) // if the field isnt filled up entirely print out the text just like normal
        {
            text_to_print = this->input_buffer;
        }
        else
        {
            for (signed int i = num_of_chars_to_print; i > 0; --i)
            {
                text_to_print += this->input_buffer[input_buffer_size - i];
            }
        }

        // if the input-field is a password-input-field display only *
        if (this->input_type == IN_INPUT_TYPE::IN_PASSWORD)
        {
            for (uint8_t i = 0; i < text_to_print.length(); ++i)
            {
                text_to_print[i] = '*';
            }
        }

        // center the text horizontally in the widget
        this->display->print(text_to_print, this->upper_left.x_pos + this->_text_gap,
                             this->upper_left.y_pos + (this->height() / 2) - font_height / 2); // this->INPUT_UNSET_VALUE is '\0' - end of text...
    }
}

template <uint8_t MAX_NUM_OF_CHARS, typename CALL_OBJECT_TYPE>
void UI::InputField<MAX_NUM_OF_CHARS, CALL_OBJECT_TYPE>::_draw_touched_content()
{
    this->display->setFont(this->_text_font);
    uint8_t font_height = this->display->getFontYsize();
    uint8_t font_width = this->display->getFontXsize();

    String text_to_print; // the text to print int the widget

    // print the print the text in the InputField if there is text in the buffer
    if (!this->buffer_is_empty())
    {
        this->display->setBackColor(this->touched_background_color);
        this->display->setColor(this->touched_text_color);

        // calculate the num of chars to print in the field - in case there is more input than the widget ca display
        uint8_t input_buffer_size = strlen(this->input_buffer);

        uint8_t space_for_text = (this->width() - this->_text_gap * 2); // in pixels
        uint8_t num_of_chars_to_print = space_for_text / font_width;    // num of characters we can print in the widget

        if (input_buffer_size < num_of_chars_to_print) // if the field isnt filled up entirely print out the text just like normal
        {
            text_to_print = this->input_buffer;
        }
        else
        {
            for (signed int i = num_of_chars_to_print; i > 0; --i)
            {
                text_to_print += this->input_buffer[input_buffer_size - i];
            }
        }

        // if the input-field is a password-input-field display only *
        if (this->input_type == IN_INPUT_TYPE::IN_PASSWORD)
        {
            for (uint8_t i = 0; i < text_to_print.length(); ++i)
            {
                text_to_print[i] = '*';
            }
        }

        // center the text horizontally in the widget
        this->display->print(text_to_print, this->upper_left.x_pos + this->_text_gap,
                             this->upper_left.y_pos + (this->height() / 2) - font_height / 2); // this->INPUT_UNSET_VALUE is '\0' - end of text...
    }
    // draw the cursor
    this->display->setColor(this->touched_cursor_color);
    this->display->drawVLine(this->upper_left.x_pos + this->_text_gap + font_width * text_to_print.length(),
                             this->upper_left.y_pos + (this->height() / 2) - font_height / 2, font_height); // height of the curser is the font-height and the upper-pos is the upper-pos of the text
}

template <uint8_t MAX_NUM_OF_CHARS, typename CALL_OBJECT_TYPE>
void UI::InputField<MAX_NUM_OF_CHARS, CALL_OBJECT_TYPE>::_focus_lose()
{
    Touch_Widget<CALL_OBJECT_TYPE>::_focus_lose();
    this->_draw_widget();
}

template <uint8_t MAX_NUM_OF_CHARS, typename CALL_OBJECT_TYPE>
void UI::InputField<MAX_NUM_OF_CHARS, CALL_OBJECT_TYPE>::_draw_widget()
{
    if (!this->is_hidden())
    {
        if (this->is_touched() || this->is_focused())
        {
            if (this->get_draw_border())
            {
                this->_draw_touched_border();
            }
            this->_draw_touched_background();
            this->_draw_touched_content();
        }
        else
        {
            if (this->get_draw_border())
            {
                this->_draw_released_border();
            }
            this->_draw_released_background();
            this->_draw_released_content();
        }
    }
}
