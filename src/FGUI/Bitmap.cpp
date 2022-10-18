#include "FGUI/FGUI.hpp"

FGUI::Bitmap::Bitmap(WindowBase *_parent, position _upper_left,
                     uint16_t _bitmap_width, uint16_t _bitmap_height,
                     unsigned short *_bitmap, uint8_t _border_weight,
                     uint8_t _border_to_bitmap_gap) : Widget(_parent, _upper_left,
                                                             _bitmap_width - 1 + _border_weight * 2 + _border_to_bitmap_gap * 2,
                                                             _bitmap_height - 1 + _border_weight * 2 + border_to_bitmap_gap * 2,
                                                             _border_weight),
                                                      border_to_bitmap_gap(_border_to_bitmap_gap),
                                                      bitmap(_bitmap)
{
}

FGUI::Bitmap::Bitmap(WindowBase *_parent, position _upper_left,
                     uint16_t _bitmap_width, uint16_t _bitmap_height,
                     uint8_t _border_weight, uint8_t _border_to_bitmap_gap) : Widget(_parent, _upper_left,
                                                                                     _bitmap_width - 1 + _border_weight * 2 + _border_to_bitmap_gap * 2,
                                                                                     _bitmap_height - 1 + _border_weight * 2 + border_to_bitmap_gap * 2,
                                                                                     _border_weight),
                                                                              border_to_bitmap_gap(_border_to_bitmap_gap),
                                                                              bitmap(nullptr)
{
}

uint16_t
FGUI::Bitmap::get_bitmap_height() const
{
    /*
        if we subtract the border_weight and the gap between the bitmap and the border we get the bitmap size...
    */
    return this->height() - this->get_border_weight() * 2 - this->border_to_bitmap_gap * 2 + 1; // +1 we start count the pixels at 0 in the upper_left and lower_right - here not...
}
uint16_t FGUI::Bitmap::get_bitmap_width() const
{
    /*
        if we subtract the border_weight and the gap between the bitmap and the border we get the bitmap size...
    */
    return this->width() - this->get_border_weight() * 2 - this->border_to_bitmap_gap * 2 + 1; // +1 we start count the pixels at 0 in the upper_left and lower_right - here not...
}

void FGUI::Bitmap::_draw_content(Widget::w_status _st)
{
    if (this->bitmap != nullptr)
    {
        this->display->drawBitmap(this->upper_left.x_pos + this->get_border_weight() + this->border_to_bitmap_gap,
                                  this->upper_left.y_pos + this->get_border_weight() + this->border_to_bitmap_gap,
                                  this->get_bitmap_width(), this->get_bitmap_height(),
                                  this->bitmap, 1);
    }
}