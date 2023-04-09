#include "FGUI/FGUI.hpp"

FGUI::Bitmap::Bitmap(WindowBase *_parent, position _upper_left,
                     uint16_t _bitmap_width, uint16_t _bitmap_height,
                     const unsigned short *_bitmap,
                     uint8_t _border_weight,
                     uint8_t _border_to_bitmap_gap) : Widget(_parent, _upper_left,
                                                             _bitmap_width,
                                                             _bitmap_height,
                                                             _border_weight,
                                                             _border_to_bitmap_gap),
                                                      bitmap(const_cast<unsigned short *>(_bitmap))
{
}

FGUI::Bitmap::Bitmap(WindowBase *_parent, position _upper_left,
                     uint16_t _bitmap_width, uint16_t _bitmap_height,
                     uint8_t _border_weight, uint8_t _border_to_bitmap_gap) : Widget(_parent, _upper_left,
                                                                                     _bitmap_width + _border_to_bitmap_gap * 2,
                                                                                     _bitmap_height + _border_to_bitmap_gap * 2,
                                                                                     _border_weight,
                                                                                     _border_to_bitmap_gap),
                                                                              bitmap(nullptr)
{
}

uint16_t FGUI::Bitmap::get_bitmap_height() const
{
    /*
        if we subtract the border_weight and the gap between the bitmap and the border we get the bitmap size...
    */
    return this->height() - (uint16_t)this->get_border_weight() * 2 - this->get_content_border_gap() * 2;
}
uint16_t FGUI::Bitmap::get_bitmap_width() const
{
    /*
        if we subtract the border_weight and the gap between the bitmap and the border we get the bitmap size...
    */
    return this->width() - (uint16_t)this->get_border_weight() * 2 - this->get_content_border_gap() * 2;
}

void FGUI::Bitmap::_draw_content(Widget::w_status _st)
{
    if (this->bitmap != nullptr)
    {
        this->display->drawBitmap(this->get_content_upper_left().x_pos,
                                  this->get_content_upper_left().y_pos,
                                  this->get_bitmap_width(),
                                  this->get_bitmap_height(),
                                  this->bitmap, 1);
    }
}