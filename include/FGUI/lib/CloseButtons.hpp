#pragma once

#include "ButtonBase.hpp"

namespace FGUI
{
    template <typename CALL_OBJECT_TYPE>
    class CloseButton : public ButtonBase<CALL_OBJECT_TYPE>
    {
    public:
        /*
            close_button is a square - height and width are equal...
        */
        CloseButton(WindowBase *parent, position _upper_left, uint16_t width_height,
                    CALL_OBJECT_TYPE *_call_object) : ButtonBase<CALL_OBJECT_TYPE>(parent, _upper_left, {_upper_left.x_pos + width_height, _upper_left.y_pos + width_height}, _call_object)
        {
            this->released_border_color = VGA_WHITE;
            this->released_background_color = VGA_BLACK;
            this->touched_border_color = VGA_WHITE;
            this->touched_background_color = VGA_RED;
        }

        virtual ~CloseButton() {}

        unsigned int released_cross_color = VGA_RED;
        unsigned int pressed_cross_color = VGA_BLACK;

    protected:
        void _draw_released_content() override;
        void _draw_touched_content() override;
        void _draw_widget() override;

    private:
    };
} // namespace FGUI

template <typename CALL_OBJECT_TYPE>
void FGUI::CloseButton<CALL_OBJECT_TYPE>::_draw_released_content()
{
    // draw the background color
    this->display->setColor(this->released_background_color);
    this->display->fillRect(this->upper_left.x_pos, this->upper_left.y_pos,
                            this->lower_right.x_pos, this->lower_right.y_pos);

    // draw the border
    this->display->setColor(this->released_border_color);
    this->display->drawRect(this->upper_left.x_pos, this->upper_left.y_pos,
                            this->lower_right.x_pos, this->lower_right.y_pos);

    // draw the X
    this->display->setColor(this->released_cross_color);
    this->display->drawLine(this->upper_left.x_pos + this->width() * 0.2, this->upper_left.y_pos + this->height() * 0.2,
                            this->lower_right.x_pos - this->width() * 0.2, this->lower_right.y_pos - this->height() * 0.2);
    this->display->drawLine(this->lower_right.x_pos - this->width() * 0.2, this->upper_left.y_pos + this->height() * 0.2,
                            this->upper_left.x_pos + this->width() * 0.2, this->lower_right.y_pos - this->height() * 0.2);
}

template <typename CALL_OBJECT_TYPE>
void FGUI::CloseButton<CALL_OBJECT_TYPE>::_draw_touched_content()
{
    // draw the background color
    this->display->setColor(this->touched_background_color);
    this->display->fillRect(this->upper_left.x_pos, this->upper_left.y_pos,
                            this->lower_right.x_pos, this->lower_right.y_pos);

    // draw the border
    this->display->setColor(this->touched_border_color);
    this->display->drawRect(this->upper_left.x_pos, this->upper_left.y_pos,
                            this->lower_right.x_pos, this->lower_right.y_pos);

    // draw the X
    this->display->setColor(this->pressed_cross_color);
    this->display->drawLine(this->upper_left.x_pos + this->width() * 0.2, this->upper_left.y_pos + this->height() * 0.2,
                            this->lower_right.x_pos - this->width() * 0.2, this->lower_right.y_pos - this->height() * 0.2);
    this->display->drawLine(this->lower_right.x_pos - this->width() * 0.2, this->upper_left.y_pos + this->height() * 0.2,
                            this->upper_left.x_pos + this->width() * 0.2, this->lower_right.y_pos - this->height() * 0.2);
}

template <typename CALL_OBJECT_TYPE>
void FGUI::CloseButton<CALL_OBJECT_TYPE>::_draw_widget()
{
    if (!this->is_hidden())
    {
        if (this->is_touched())
        {
            this->_draw_touched_content();
        }
        else
        {
            this->_draw_released_content();
        }
    }
}