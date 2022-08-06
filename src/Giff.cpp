#include "UI/UI.hpp"
#include <stdarg.h>

UI::Giff::Giff(WindowBase *_parent, position _upper_left, uint16_t _bitmap_width, uint16_t _bitmap_height,
               uint8_t _border_weight, uint8_t _border_to_bitmap_gap, uint16_t _change_speed,
               uint8_t num_of_bitmaps, ...) : Widget(_parent, _upper_left, _bitmap_width - 1 + _border_weight * 2 + _border_to_bitmap_gap * 2, _bitmap_height - 1 + _border_weight * 2 + _border_to_bitmap_gap * 2, _border_weight), // -1 we start counting the pixels at 0...
                                              border_to_bitmap_gap(_border_to_bitmap_gap), current_bitmap(bitmaps.begin()),
                                              bitmap_change_speed(_change_speed)
{
    va_list arguments;
    va_start(arguments, num_of_bitmaps);
    for (uint8_t i = 0; i < num_of_bitmaps; ++i)
    {
        this->bitmaps.push_back(va_arg(arguments, unsigned int *));
    }
    va_end(arguments);
    this->current_bitmap = this->bitmaps.begin();
}

void UI::Giff::_draw_released_content()
{
    /*
        if we subtract the border_weight and the gap between the bitmap and the border we get the bitmap size...
    */
    uint16_t bitmap_width = this->width() - this->get_border_weight() * 2 - this->border_to_bitmap_gap * 2 + 1;   // +1 we start count the pixels at 0 in the upper_left and lower_right - here not...
    uint16_t bitmap_height = this->height() - this->get_border_weight() * 2 - this->border_to_bitmap_gap * 2 + 1; // +1 we start count the pixels at 0 in the upper_left and lower_right - here not...

    this->display->drawBitmap(this->upper_left.x_pos + this->get_border_weight() + this->border_to_bitmap_gap,
                              this->upper_left.y_pos + this->get_border_weight() + this->border_to_bitmap_gap,
                              bitmap_width, bitmap_height,
                              this->current_bitmap.data(), 1);
}

void UI::Giff::loop()
{
    if (this->running)
    {
        if (this->last_bitmap_change_tm_pt + bitmap_change_speed < millis())
        {
            uint16_t bitmap_width = this->width() - this->get_border_weight() * 2 - this->border_to_bitmap_gap * 2 + 1;   // +1 we start count the pixels at 0 in the upper_left and lower_right - here not...
            uint16_t bitmap_height = this->height() - this->get_border_weight() * 2 - this->border_to_bitmap_gap * 2 + 1; // +1 we start count the pixels at 0 in the upper_left and lower_right - here not...

            this->display->drawBitmap(this->upper_left.x_pos + this->get_border_weight() + this->border_to_bitmap_gap,
                                      this->upper_left.y_pos + this->get_border_weight() + this->border_to_bitmap_gap,
                                      bitmap_width, bitmap_height,
                                      this->current_bitmap.data(), 1);
            this->last_bitmap_change_tm_pt = millis();

            ++current_bitmap;
            if (this->current_bitmap == this->bitmaps.end())
            {
                this->current_bitmap = this->bitmaps.begin();
            }
        }
    }
}

void UI::Giff::resume()
{
    this->running = true;
    this->last_bitmap_change_tm_pt = millis(); // reset the bitmap change timer for the current bitmap
}

void UI::Giff::stop()
{
    this->running = false;
}