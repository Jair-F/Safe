#pragma once

#include "Widget.hpp"

namespace FGUI
{
    /**
     * \defgroup Bitmap
     * \ingroup FGUI
     *
     * @brief draw pictures/bitmaps on the display
     *
     * @{
     */

    class Bitmap : public Widget
    {
    public:
        /**
         * @param _parent the parent window to which the widget will register to
         * @param _upper_left upper left corner in relation to the parent window zero point
         * @param _bitmap_width width of the bitmap in pixels.
         * @param _bitmap_height height of the bitmap in pixels.
         * @param _bitmap the bitmap which should be drawn
         * @param _border_weight size of the border in pixels
         * @param _border_to_bitmap_gap the gap between the border and the bitmap in pixels.
         *
         * @details the bitmap gets its height and widht from the bitmap size, border_weight and border_to_bitmap_gap.
         */
        Bitmap(WindowBase *_parent, position _upper_left, uint16_t _bitmap_width, uint16_t _bitmap_height,
               unsigned short *_bitmap = nullptr, uint8_t border_weight = 3, uint8_t _border_to_bitmap_gap = 0);
        /**
         * @param _parent the parent window to which the widget will register to
         * @param _upper_left upper left corner in relation to the parent window zero point
         * @param _bitmap_width width of the bitmap in pixels.
         * @param _bitmap_height height of the bitmap in pixels.
         * @param _border_weight size of the border in pixels
         * @param _border_to_bitmap_gap the gap between the border and the bitmap in pixels.
         *
         * @details the bitmap gets its height and widht from the bitmap size, border_weight and border_to_bitmap_gap.
         */
        Bitmap(WindowBase *_parent, position _upper_left, uint16_t _bitmap_width, uint16_t _bitmap_height,
               uint8_t border_weight = 3, uint8_t _border_to_bitmap_gap = 0);

        virtual ~Bitmap() {}

        /**
         * @return height of the bitmaps in pixels
         */
        uint16_t get_bitmap_height() const;
        /**
         * @return width of the bitmap in pixels
         */
        uint16_t get_bitmap_width() const;

    protected:
        void _draw_content(Widget::w_status _st) override;

    private:
        uint8_t border_to_bitmap_gap; // in pixels
        unsigned short *bitmap = nullptr;
    };

    /** @} */
}
