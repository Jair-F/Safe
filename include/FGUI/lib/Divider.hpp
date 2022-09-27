#pragma once
#include "Widget.hpp"

namespace FGUI
{
    /**
     * \defgroup Divider
     * \ingroup FGUI
     *
     * \brief horizontal or vertical line to devide the screen
     *
     * \details defineable position and weight of the divider.
     *
     * @{
     */
    class Divider : public Widget
    {
    public:
        /**
         * @brief type to define wether the divider is placed horizontally or vertically from the _start_position.
         */
        enum d_alignment
        {
            AL_HORIZONTAL = 0,
            AL_VERTICAL
        };
        /**
         * @param _parent the parent window to which the widget will register to.
         * @param _start_position the start position of the divider(always the upper_left).
         * @param _length length of the divider.
         * @param _width width of the divider.
         * @param _alignment alignment of the divider
         * @param _color color of the divider.
         */
        Divider(WindowBase *_parent, position _start_position, uint16_t _length,
                uint8_t _width, d_alignment _alignment, unsigned int _color);

        /**
         * @brief calculate the width no matter how the Divider is aligned
         * @return width of the divider.
         */
        inline uint8_t get_width() const
        {
            if (this->alignment == d_alignment::AL_HORIZONTAL)
                return this->height() + 1;
            else
                return this->width() + 1;
        }

        /**
         * @note wrapper for released_background_color
         * the Divider uses the background_color of the widget as its color value
         */
        void set_color(unsigned int color) { this->released_background_color = color; }

    protected:
    private:
        d_alignment alignment;
    };

    /** @} */

}
