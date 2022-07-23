#include "UI/UI.hpp"

UI::ProgressBar::ProgressBar(WindowBase *_parent, position _upper_left,
                             position _lower_right, uint8_t _border_weight) : Widget(_parent, _upper_left, _lower_right), border_weight(_border_weight), progress(0)
{
}

void UI::ProgressBar::_draw_released_widget()
{
    // draw the border
    this->display->setColor(this->border_color);
    for (uint8_t i = 0; i < this->border_weight; ++i)
    {
        this->display->drawRect(this->upper_left.x_pos + i, this->upper_left.y_pos + i, this->lower_right.x_pos - i, this->lower_right.y_pos - i);
    }

    if (this->progress > 0)
    {
        uint16_t progress_bar_length = double(progress) / 100 * (this->width() - this->border_weight * 2 - border_to_bar_gap * 2);

        // clear the space inside the progressbar - not the border - progressbar could also go back...
        this->display->setColor(this->_parent_window->get_background_color());
        this->display->fillRect(this->upper_left.x_pos + this->border_weight + border_to_bar_gap, this->upper_left.y_pos + this->border_weight + border_to_bar_gap,
                                this->lower_right.x_pos - this->border_weight - border_to_bar_gap, this->lower_right.y_pos - this->border_weight - border_to_bar_gap);

        this->display->setColor(this->bar_color);
        this->display->fillRect(this->upper_left.x_pos + this->border_weight + border_to_bar_gap, this->upper_left.y_pos + this->border_weight + border_to_bar_gap,
                                this->upper_left.x_pos + this->border_weight + border_to_bar_gap + progress_bar_length, this->lower_right.y_pos - this->border_weight - border_to_bar_gap);
    }
}

void UI::ProgressBar::set_progress(uint8_t _progress)
{
    _progress = _progress > 100 ? 100 : _progress; // ensure that _progress isnt more than 100
    this->progress = _progress;
    this->update_widget(); // maybe draw an animation...
}
