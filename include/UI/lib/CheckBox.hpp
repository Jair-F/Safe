#pragma once
#include "UI/UI.hpp"

namespace UI
{
    template <typename CALL_OBJECT_TYPE>
    class CheckBox : public Touch_Widget<CALL_OBJECT_TYPE>
    {
    public:
        CheckBox(Window *_parent, position _upper_left, position _lower_right, CALL_OBJECT_TYPE *_call_object);
        CheckBox(Window *_parent, position _upper_left, uint16_t _width, uint16_t _height, CALL_OBJECT_TYPE *_call_object);

        /*
            @return true if the checkbox is checked
        */
        inline bool is_checked() const { return this->checked; }

        /*
            @param true if checked
        */
        void set_checked(bool _checked)
        {
            this->checked = _checked;
        }

        /*
            we change the state of the check_box only on the release of the touch
        */
        void _release(const position &_touch_data) override;

        /*
            the color values are RGB-565 values(16-bit value)
            RGB-565 color picker: https://chrishewett.com/blog/true-rgb565-colour-picker/
        */
        unsigned int background_color = VGA_BLACK;
        unsigned int check_sign_color = VGA_WHITE;
        unsigned int border_color = VGA_WHITE;

    protected:
        void _draw_widget() override;
        void _draw_pressed_widget() override;
        void _draw_released_widget() override;

    private:
        bool checked; // true if the checkbox is checked, false if not
    };
} // namespace UI

// ------------- template implementation -------------

template <typename CALL_OBJECT_TYPE>
UI::CheckBox<CALL_OBJECT_TYPE>::CheckBox(Window *_parent, position _upper_left,
                                         position _lower_right, CALL_OBJECT_TYPE *_call_object) : Touch_Widget<CALL_OBJECT_TYPE>(_parent, _upper_left, _lower_right, _call_object), checked(false)
{
}

template <typename CALL_OBJECT_TYPE>
UI::CheckBox<CALL_OBJECT_TYPE>::CheckBox(Window *_parent, const position _upper_left,
                                         uint16_t _width, uint16_t _height, CALL_OBJECT_TYPE *_call_object) : Touch_Widget<CALL_OBJECT_TYPE>(_parent, _upper_left, _width, _height, _call_object), checked(false)
{
}

template <typename CALL_OBJECT_TYPE>
void UI::CheckBox<CALL_OBJECT_TYPE>::_draw_released_widget()
{
    this->display->setColor(this->background_color);
    this->display->fillRoundRect(this->upper_left.x_pos, this->upper_left.y_pos, this->lower_right.x_pos, this->lower_right.y_pos);

    this->display->setColor(this->border_color);
    this->display->drawRoundRect(this->upper_left.x_pos, this->upper_left.y_pos, this->lower_right.x_pos, this->lower_right.y_pos);
}

template <typename CALL_OBJECT_TYPE>
void UI::CheckBox<CALL_OBJECT_TYPE>::_draw_pressed_widget()
{
    this->display->setColor(this->background_color);
    this->display->fillRoundRect(this->upper_left.x_pos, this->upper_left.y_pos, this->lower_right.x_pos, this->lower_right.y_pos);

    this->display->setColor(this->border_color);
    this->display->drawRoundRect(this->upper_left.x_pos, this->upper_left.y_pos, this->lower_right.x_pos, this->lower_right.y_pos);

    uint8_t gap_from_box = 2; // the gap between the box and the begin of the check mark

    this->display->setColor(this->check_sign_color);
    // check-mark - short line
    this->display->drawLine(this->upper_left.x_pos + gap_from_box, (this->upper_left.y_pos + this->height() * 0.5),
                            (this->upper_left.x_pos + this->width() * 0.5) - gap_from_box, this->lower_right.y_pos - gap_from_box);

    // check-mark - long line
    this->display->drawLine((this->upper_left.x_pos + this->width() * 0.5) - gap_from_box, this->lower_right.y_pos - gap_from_box,
                            this->lower_right.x_pos - gap_from_box - 1, this->upper_left.y_pos + gap_from_box + 1);
}

template <typename CALL_OBJECT_TYPE>
void UI::CheckBox<CALL_OBJECT_TYPE>::_draw_widget()
{
    // change the state only on touche. otherwise it would change on touch to checked and on release return to not checked!
    if (this->is_checked())
    {
        // this->_clear_widget_space();
        this->_draw_pressed_widget();
    }
    else
    {
        this->_draw_released_widget();
    }
}

template <typename CALL_OBJECT_TYPE>
void UI::CheckBox<CALL_OBJECT_TYPE>::_release(const position &_touch_data)
{
    if (!this->is_hidden())
    {
        this->checked = !this->checked;
        Touch_Widget<CALL_OBJECT_TYPE>::_release(_touch_data); // draw_widget is called in the touch_widget::click func
    }
}
