#include "FGUI/FGUI.hpp"

FGUI::ProgressBar::ProgressBar(WindowBase *_parent, position _upper_left,
                               position _lower_right, uint8_t _border_weight = 1) : Widget(_parent, _upper_left, _lower_right, _border_weight), progress(0)
{
    this->released_background_color = this->parent_window->get_background_color();
}

void FGUI::ProgressBar::_draw_released_content()
{
    if (this->get_draw_border())
    {
        this->_draw_released_border();
    }
    this->_draw_released_background();

    if (this->progress > 0)
    {
        uint16_t progress_bar_length = double(progress) / 100 * (this->width() - this->get_border_weight() * 2 - border_to_bar_gap * 2);

        // clear the space inside the progressbar - not the border - progressbar could also go back...
        this->display->setColor(this->parent_window->get_background_color());
        this->display->fillRect(this->upper_left.x_pos + this->get_border_weight() + border_to_bar_gap, this->upper_left.y_pos + this->get_border_weight() + border_to_bar_gap,
                                this->lower_right.x_pos - this->get_border_weight() - border_to_bar_gap, this->lower_right.y_pos - this->get_border_weight() - border_to_bar_gap);

        this->display->setColor(this->bar_color);
        this->display->fillRect(this->upper_left.x_pos + this->get_border_weight() + border_to_bar_gap, this->upper_left.y_pos + this->get_border_weight() + border_to_bar_gap,
                                this->upper_left.x_pos + this->get_border_weight() + border_to_bar_gap + progress_bar_length, this->lower_right.y_pos - this->get_border_weight() - border_to_bar_gap);
    }
}

void FGUI::ProgressBar::set_progress(uint8_t _progress)
{
    _progress = _progress > 100 ? 100 : _progress; // ensure that _progress isnt more than 100
    this->progress = _progress;
    this->update_widget(); // maybe draw an animation...
}
