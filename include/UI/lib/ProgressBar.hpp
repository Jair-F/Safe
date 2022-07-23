#pragma once

#include "Widget.hpp"

namespace UI
{
    class ProgressBar : public Widget
    {
    public:
        ProgressBar(WindowBase *_parent, position _upper_left,
                    position _lower_right, uint8_t _border_weight);
        ~ProgressBar() {}

        void set_progress(uint8_t _progress);
        uint8_t get_progress() const { return this->progress; }

        void set_border_weight(uint8_t _border_weight) { this->border_weight = _border_weight; }

        void set_border_to_bar_gap(uint8_t _border_to_bar_gap) { this->border_to_bar_gap = _border_to_bar_gap; }

        unsigned int border_color = VGA_WHITE;
        unsigned int bar_color = VGA_GREEN;

    protected:
        void _draw_released_widget() override;

        void _draw_pressed_widget() override { this->_draw_released_widget(); }
        void _draw_widget() override { this->_draw_released_widget(); }

    private:
        uint8_t border_weight;
        uint8_t progress;              // progress of the progress bar in precent(0-100)
        uint8_t border_to_bar_gap = 3; // gap between the border and the progress bar in pixels
    };
} // namespace UI
