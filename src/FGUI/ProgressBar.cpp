#include "FGUI/FGUI.hpp"

FGUI::ProgressBar::ProgressBar(WindowBase *_parent,
                               position _upper_left, position _lower_right,
                               uint8_t _border_weight,
                               uint8_t _border_to_bar_gap) : Widget(_parent,
                                                                    _upper_left, _lower_right,
                                                                    _border_weight, _border_to_bar_gap),
                                                             progress(0)
{
    this->released_background_color = this->parent_window->get_background_color();
}

FGUI::ProgressBar::ProgressBar(WindowBase *_parent,
                               position _upper_left,
                               uint8_t _width, uint8_t _height,
                               uint8_t _border_weight,
                               uint8_t _border_to_bar_gap) : Widget(_parent,
                                                                    _upper_left,
                                                                    _width, _height,
                                                                    _border_weight, _border_to_bar_gap),
                                                             progress(0)
{
    this->released_background_color = this->parent_window->get_background_color();
}

void FGUI::ProgressBar::_draw_content(Widget::w_status _st)
{
    if (!this->is_hidden())
    {
        if (this->get_draw_border())
        {
            this->_draw_border(_st);
        }
        else
            this->_clear_border_space();
        this->_draw_background(_st);

        if (this->progress > 0)
        {
            uint16_t progress_bar_length = double(progress) / 100 * this->get_content_width();

            this->display->setColor(this->bar_color);
            this->display->fillRect(this->get_content_upper_left().x_pos,
                                    this->get_content_upper_left().y_pos,
                                    this->get_content_upper_left().x_pos + progress_bar_length,
                                    this->get_content_lower_right().y_pos);
        }
        else
            this->clear_content_space();
    }
}

void FGUI::ProgressBar::set_progress(uint8_t _progress)
{
    _progress = _progress > 100 ? 100 : _progress; // ensure that _progress isnt more than 100
    this->progress = _progress;
    this->draw(); // maybe draw an animation...
}
