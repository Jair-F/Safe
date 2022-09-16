#pragma once

#include "ButtonBase.hpp"

namespace FGUI
{
    template <typename CALL_OBJECT_TYPE>
    class BitMapButton : public ButtonBase<CALL_OBJECT_TYPE>
    {
    public:
        BitMapButton(WindowBase *_parent, position _upper_left,
                     uint16_t _bitmap_width, uint16_t _bitmap_height, CALL_OBJECT_TYPE *_call_object,
                     const unsigned short *_on_touch_bitmap, const unsigned short *_on_release_bitmap,
                     const unsigned short *_on_disabled_bitmap,
                     uint8_t _border_weight = 3,
                     uint8_t _border_to_bitmap_gap = 0) : ButtonBase<CALL_OBJECT_TYPE>(_parent, _upper_left, {_upper_left.x_pos + _bitmap_width - 1 + _border_weight * 2 + _border_to_bitmap_gap * 2, _upper_left.y_pos + _bitmap_height - 1 + _border_weight * 2 + _border_to_bitmap_gap * 2}, _call_object, _border_weight), // -1 we start counting the pixels at 0...
                                                          on_touch_bitmap(_on_touch_bitmap), on_release_bitmap(_on_release_bitmap), on_disabled_bitmap(_on_disabled_bitmap), border_to_bitmap_gap(_border_to_bitmap_gap)
        {
            // -1 we start counting the pixels at 0...
            // this->set_size(_bitmap_width - 1 + this->border_weight * 2 + _border_to_bitmap_gap * 2, _bitmap_height - 1 + this->border_weight * 2 + _border_to_bitmap_gap * 2);
            this->touched_background_color = this->_get_parent_window()->get_background_color();
            this->released_background_color = this->_get_parent_window()->get_background_color();
        }

        virtual ~BitMapButton() {}

    protected:
        void _draw_content(Widget::w_status _st) override;

    private:
        const unsigned short *on_touch_bitmap;
        const unsigned short *on_release_bitmap;
        const unsigned short *on_disabled_bitmap;

        uint8_t border_to_bitmap_gap; // in pixels
    };
} // namespace FGUI

template <typename CALL_OBJECT_TYPE>
void FGUI::BitMapButton<CALL_OBJECT_TYPE>::_draw_content(Widget::w_status _st)
{
    /*
        if we subtract the border_weight and the gap between the bitmap and the border we get the bitmap size...
    */
    uint16_t bitmap_width = this->width() - this->get_border_weight() * 2 - this->border_to_bitmap_gap * 2 + 1;   // +1 we start count the pixels at 0 in the upper_left and lower_right - here not...
    uint16_t bitmap_height = this->height() - this->get_border_weight() * 2 - this->border_to_bitmap_gap * 2 + 1; // +1 we start count the pixels at 0 in the upper_left and lower_right - here not...

    unsigned short *bitmap_to_draw;
    switch (_st)
    {
    case Widget::w_status::S_DISABLED:
    {
        bitmap_to_draw = const_cast<unsigned short *>(on_disabled_bitmap);
        break;
    }
    case Widget::w_status::S_TOUCHED:
    {
        bitmap_to_draw = const_cast<unsigned short *>(on_touch_bitmap);
        break;
    }
    case Widget::w_status::S_RELEASED:
    {
        bitmap_to_draw = const_cast<unsigned short *>(on_release_bitmap);
        break;
    }
    default:
    {
        break;
    }
    }

    this->display->drawBitmap(this->upper_left.x_pos + this->get_border_weight() + this->border_to_bitmap_gap,
                              this->upper_left.y_pos + this->get_border_weight() + this->border_to_bitmap_gap,
                              bitmap_width, bitmap_height,
                              bitmap_to_draw, 1);
}
