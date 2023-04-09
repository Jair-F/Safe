#include "FGUI/FGUI.hpp"
#include <stdarg.h>

FGUI::Giff::Giff(WindowBase *_parent,
                 position _upper_left,
                 uint16_t _bitmap_width, uint16_t _bitmap_height,
                 uint8_t _border_weight,
                 uint8_t _border_to_bitmap_gap,
                 uint16_t _change_speed,
                 uint8_t num_of_bitmaps, ...) : Widget(_parent,
                                                       _upper_left,
                                                       _bitmap_width, _bitmap_height,
                                                       _border_weight, _border_to_bitmap_gap),
                                                current_bitmap(bitmaps.begin()),
                                                bitmap_change_speed(_change_speed)
{
    va_list arguments;
    va_start(arguments, num_of_bitmaps);
    for (uint8_t i = 0; i < num_of_bitmaps; ++i)
    {
        this->bitmaps.push_back(va_arg(arguments, unsigned short *));
    }
    va_end(arguments);
    this->current_bitmap = this->bitmaps.begin();
}

void FGUI::Giff::_draw_content(Widget::w_status _st)
{
    this->display->drawBitmap(this->get_content_upper_left().x_pos,
                              this->get_content_upper_left().y_pos,
                              this->get_content_width(), this->get_content_height(),
                              this->current_bitmap.data(), 1);
}

void FGUI::Giff::loop()
{
    if (this->running)
    {
        if (this->last_bitmap_change_tm_pt + bitmap_change_speed < millis())
        {
            this->display->drawBitmap(this->get_content_upper_left().x_pos,
                                      this->get_content_upper_left().y_pos,
                                      this->get_content_width(), this->get_content_height(),
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

void FGUI::Giff::resume()
{
    this->running = true;
    this->last_bitmap_change_tm_pt = millis(); // reset the bitmap change timer for the current bitmap
}

void FGUI::Giff::stop()
{
    this->running = false;
}