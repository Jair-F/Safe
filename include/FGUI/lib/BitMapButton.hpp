#pragma once

#include "ButtonBase.hpp"

namespace FGUI
{
    /**
     * \defgroup BitmapButton
     * \ingroup FGUI
     * \ingroup Bitmap
     *
     * @brief button which is a clickable picture/bitmap
     *
     * @note Limitations: all the bitmaps - the on_release and on_touched... need to have the same size(height and width).
     *
     * @{
     */
    template <typename CALL_OBJECT_TYPE>
    class BitmapButton : public ButtonBase<CALL_OBJECT_TYPE>
    {
        // documentating the template parameter
        /**
         * @tparam CALL_OBJECT_TYPE a instance of the class of which the callback functions for on_touch, on_release and on_focus_loose are called with.
         */

    public:
        /**
         * @param _parent the parent window to which the widget will register to
         * @param _upper_left upper left corner in relation to the parent window zero point
         * @param _bitmap_width width of the bitmap in pixels.
         * @param _bitmap_height height of the bitmap in pixels.
         * @param _call_object a instance of the class of which the callback functions for on_touch, on_release and on_focus_loose are called with.
         * @param _on_touch_bitmap the bitmap which should be drawn if the button is touched.
         * @param _on_release_bitmap the bitmap which should be drawn if the button is released.
         * @param _on_disabled_bitmap the bitmap which should be drawn if the button is disabled.
         * @param _border_weight size of the border in pixels
         * @param _border_to_bitmap_gap the gap between the border and the bitmap in pixels.
         *
         * @details the bitmapButton  gets its height and widht from the bitmaps sizes, border_weight and border_to_bitmap_gap.
         *
         * @note if _call_object nullptr the programm will crash due a assertion!
         */
        BitmapButton(WindowBase *_parent, position _upper_left,
                     uint16_t _bitmap_width, uint16_t _bitmap_height,
                     CALL_OBJECT_TYPE *_call_object,
                     const unsigned short *_on_touch_bitmap,
                     const unsigned short *_on_release_bitmap,
                     const unsigned short *_on_disabled_bitmap,
                     uint8_t _border_weight = 3,
                     uint8_t _border_to_bitmap_gap = 0) : ButtonBase<CALL_OBJECT_TYPE>(_parent,
                                                                                       _upper_left,
                                                                                       _bitmap_width, _bitmap_height,
                                                                                       _call_object,
                                                                                       _border_weight, _border_to_bitmap_gap),
                                                          on_touch_bitmap(_on_touch_bitmap), on_release_bitmap(_on_release_bitmap), on_disabled_bitmap(_on_disabled_bitmap)
        {
            // -1 we start counting the pixels at 0...
            // this->set_size(_bitmap_width - 1 + this->border_weight * 2 + _border_to_bitmap_gap * 2, _bitmap_height - 1 + this->border_weight * 2 + _border_to_bitmap_gap * 2);
            this->touched_background_color = this->_get_parent_window()->get_background_color();
            this->released_background_color = this->_get_parent_window()->get_background_color();
        }

        /**
         * @param _parent the parent window to which the widget will register to
         * @param _upper_left upper left corner in relation to the parent window zero point
         * @param _bitmap_width width of the bitmap in pixels.
         * @param _bitmap_height height of the bitmap in pixels.
         * @param _call_object a instance of the class of which the callback functions for on_touch, on_release and on_focus_loose are called with.
         * @param _bitmap the bitmap which should be drawn if the button is touched, released and disabled
         * @param _border_weight size of the border in pixels
         * @param _border_to_bitmap_gap the gap between the border and the bitmap in pixels.
         *
         * @details the bitmapButton  gets its height and widht from the bitmaps sizes, border_weight and border_to_bitmap_gap.
         *
         * @note if _call_object nullptr the programm will crash due a assertion!
         */
        BitmapButton(WindowBase *_parent, position _upper_left,
                     uint16_t _bitmap_width, uint16_t _bitmap_height,
                     CALL_OBJECT_TYPE *_call_object,
                     const unsigned short *_bitmap,
                     uint8_t _border_weight = 3,
                     uint8_t _border_to_bitmap_gap = 0) : ButtonBase<CALL_OBJECT_TYPE>(_parent, _upper_left,
                                                                                       _bitmap_width, _bitmap_height,
                                                                                       _call_object,
                                                                                       _border_weight, _border_to_bitmap_gap),
                                                          on_touch_bitmap(_bitmap), on_release_bitmap(_bitmap), on_disabled_bitmap(_bitmap)
        {
            // -1 we start counting the pixels at 0...
            // this->set_size(_bitmap_width - 1 + this->border_weight * 2 + _border_to_bitmap_gap * 2, _bitmap_height - 1 + this->border_weight * 2 + _border_to_bitmap_gap * 2);
            this->touched_background_color = this->_get_parent_window()->get_background_color();
            this->released_background_color = this->_get_parent_window()->get_background_color();
        }

        virtual ~BitmapButton() {}

        /**
         * @return height of the bitmaps in pixels
         */
        uint16_t get_bitmaps_height() const;
        /**
         * @return width of the bitmaps in pixels
         */
        uint16_t get_bitmaps_width() const;

        /**
         * set_border_weight()
         * set_border_content_gap()
         *
         * these two functions will change the content_height/width and
         * lead to incorrect drawing of the bitmap!!!
         */

        void set_border_weight(uint8_t _border_weight) override;           // throws an error on the display
        void set_content_border_gap(uint8_t _border_content_gap) override; // throws an error on the display

    protected:
        void _draw_content(Widget::w_status _st) override;

    private:
        const unsigned short *on_touch_bitmap;
        const unsigned short *on_release_bitmap;
        const unsigned short *on_disabled_bitmap;

        // uint8_t border_to_bitmap_gap = 0; // in pixels
        uint16_t bitmap_width, bitmap_height;
    };

    /** @} */

} // namespace FGUI

template <typename CALL_OBJECT_TYPE>
void FGUI::BitmapButton<CALL_OBJECT_TYPE>::_draw_content(Widget::w_status _st)
{
    unsigned short *bitmap_to_draw = nullptr;
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
    if (bitmap_to_draw != nullptr)
    {
        this->display->drawBitmap(this->get_content_upper_left().x_pos,
                                  this->get_content_upper_left().y_pos,
                                  this->get_bitmaps_width(), this->get_bitmaps_height(),
                                  bitmap_to_draw, 1);
    }
}

template <typename CALL_OBJECT_TYPE>
uint16_t FGUI::BitmapButton<CALL_OBJECT_TYPE>::get_bitmaps_height() const
{
    return this->get_content_height();
}
template <typename CALL_OBJECT_TYPE>
uint16_t FGUI::BitmapButton<CALL_OBJECT_TYPE>::get_bitmaps_width() const
{
    return this->get_content_width();
}

template <typename CALL_OBJECT_TYPE>
void FGUI::BitmapButton<CALL_OBJECT_TYPE>::set_border_weight(uint8_t _border_weight)
{
    this->display->clrScr();
    this->display->setFont(SmallFont);
    this->display->setBackColor(VGA_BLACK);
    this->display->setColor(VGA_RED);
    this->display->print(F("changing the border weight of BitmapButton's is not allowed"), 0, 0);
}
template <typename CALL_OBJECT_TYPE>
void FGUI::BitmapButton<CALL_OBJECT_TYPE>::set_content_border_gap(uint8_t _border_content_gap)
{
    this->display->clrScr();
    this->display->setFont(SmallFont);
    this->display->setBackColor(VGA_BLACK);
    this->display->setColor(VGA_RED);
    this->display->print(F("changing the content_border_gap of BitmapButton's is not allowed"), 0, 0);
}
