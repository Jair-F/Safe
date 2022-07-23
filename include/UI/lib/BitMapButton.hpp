#pragma once

#include "ButtonBase.hpp"

namespace UI
{
    template <typename CALL_OBJECT_TYPE>
    class BitMapButton : public ButtonBase<CALL_OBJECT_TYPE>
    {
    public:
        BitMapButton(WindowBase *_parent, position _upper_left,
                     uint16_t _bitmap_width, uint16_t _bitmap_height, CALL_OBJECT_TYPE *_call_object,
                     const unsigned int *_on_touch_bitmap, const unsigned int *_on_release_bitmap) : ButtonBase<CALL_OBJECT_TYPE>(_parent, _upper_left, {_upper_left.x_pos + _bitmap_width - 1 + 2, _upper_left.y_pos + _bitmap_height - 1 + 2}, _call_object), // +2 for the border... -1 we start counting the pixels at 0...
                                                                                                     on_touch_bitmap(_on_touch_bitmap), on_release_bitmap(_on_release_bitmap)
        {
        }

        virtual ~BitMapButton() {}

        /*
            @param _draw_border true if the button should have a border - false if not
        */
        void set_border(bool _draw_border) { this->draw_border = _draw_border; }
        bool get_border() const { return this->draw_border; }

    protected:
        void _draw_released_widget() override;
        void _draw_pressed_widget() override;

    private:
        const unsigned int *on_touch_bitmap;
        const unsigned int *on_release_bitmap;

        bool draw_border = true;
    };
} // namespace UI

template <typename CALL_OBJECT_TYPE>
void UI::BitMapButton<CALL_OBJECT_TYPE>::_draw_released_widget()
{
    // draw background-color
    this->display->setColor(this->released_background_color);
    this->display->fillRect(this->upper_left.x_pos + 1, this->upper_left.y_pos + 1,
                            this->lower_right.x_pos - 1, this->lower_right.y_pos - 1);

    if (this->draw_border)
    {
        // draw the border
        this->display->setColor(this->released_border_color);
        this->display->drawRect(this->upper_left.x_pos, this->upper_left.y_pos,
                                this->lower_right.x_pos, this->lower_right.y_pos);
    }

    uint16_t bitmap_width = this->width() - 2 + 1;   // -2 border thickness shift... +1 we start count the pixels at 0...
    uint16_t bitmap_height = this->height() - 2 + 1; // -2 border thickness shift... +1 we start count the pixels at 0...

    // shift 1 down - border
    this->display->drawBitmap(this->upper_left.x_pos + 1, this->upper_left.y_pos + 1, bitmap_width, bitmap_height, const_cast<unsigned int *>(this->on_touch_bitmap), 1);
}

template <typename CALL_OBJECT_TYPE>
void UI::BitMapButton<CALL_OBJECT_TYPE>::_draw_pressed_widget()
{
    // draw background-color
    this->display->setColor(this->pressed_background_color);
    this->display->fillRect(this->upper_left.x_pos + 1, this->upper_left.y_pos + 1,
                            this->lower_right.x_pos - 1, this->lower_right.y_pos - 1);

    if (this->draw_border)
    {
        // draw the border
        this->display->setColor(this->pressed_border_color);
        this->display->drawRect(this->upper_left.x_pos, this->upper_left.y_pos,
                                this->lower_right.x_pos, this->lower_right.y_pos);
    }

    uint16_t bitmap_width = this->width() - 2 + 1;   // -2 border thickness shift... +1 we start count the pixels at 0...
    uint16_t bitmap_height = this->height() - 2 + 1; // -2 border thickness shift... +1 we start count the pixels at 0...

    // shift 1 down - border
    this->display->drawBitmap(this->upper_left.x_pos + 1, this->upper_left.y_pos + 1, bitmap_width, bitmap_height, const_cast<unsigned int *>(this->on_release_bitmap), 1);
}
