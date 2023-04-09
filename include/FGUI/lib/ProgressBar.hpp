#pragma once

#include "Widget.hpp"

namespace FGUI
{

    /**
     * \defgroup ProgressBar
     * \ingroup FGUI
     *
     * @brief progress bar where the progress can be set simply in precent(0-100) and the progress bar
     * handles the rest.
     *
     * @{
     */
    class ProgressBar : public Widget
    {
    public:
        /**
         * @param _parent the parent window to which the widget will register to
         * @param _upper_left_pos upper left corner in relation to the parent window zero point
         * @param _lower_right lower right corner in relation to the parent window zero point
         * @param _border_weight size of the border in pixels
         */
        ProgressBar(WindowBase *_parent, position _upper_left,
                    position _lower_right,
                    uint8_t _border_weight, uint8_t _border_to_bar_gap = 3);
        ~ProgressBar() {}

        /**
         * @param _progress the progress fo the bar in precent(0-100).
         */
        void set_progress(uint8_t _progress);
        /**
         * @return the progress fo the bar in precent(0-100).
         */
        uint8_t get_progress() const { return this->progress; }

        /**
         * @param _border_to_bar_gap gap between the bar and the border in pixels.
         */
        void set_border_to_bar_gap(uint8_t _border_to_bar_gap) { this->set_content_border_gap(_border_to_bar_gap); }
        /**
         * @return gap between the bar and the border in pixels.
         */
        inline uint8_t get_border_to_bar_gap() const { return this->get_content_border_gap(); }

        unsigned int bar_color = VGA_GREEN;

    protected:
        void _draw_content(Widget::w_status _st) override;

    private:
        uint8_t progress; ///< @brief progress of the progress bar in precent(0-100)
        // uint8_t border_to_bar_gap = 3; ///< @brief gap between the border and the progress bar in pixels
    };

    /** @} */

} // namespace FGUI
