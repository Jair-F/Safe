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
        unsigned int touched_cross_color = VGA_BLACK;
        unsigned int disabled_cross_color = VGA_GRAY;

    protected:
        void _draw_content(Widget::w_status _st) override;
        void _draw_widget() override;

    private:
    };
} // namespace FGUI

template <typename CALL_OBJECT_TYPE>
void FGUI::CloseButton<CALL_OBJECT_TYPE>::_draw_content(Widget::w_status _st)
{
    unsigned int *background_color = nullptr,
                 *border_color = nullptr,
                 *cross_color = nullptr;

    switch (_st)
    {
    case Widget::w_status::S_DISABLED:
    {
        background_color = &this->disabled_background_color;
        border_color = &this->disabled_border_color;
        cross_color = &this->disabled_cross_color;
        break;
    }
    case Widget::w_status::S_TOUCHED:
    {
        background_color = &this->touched_background_color;
        border_color = &this->touched_border_color;
        cross_color = &this->touched_cross_color;
        break;
    }
    case Widget::w_status::S_RELEASED:
    {
        background_color = &this->released_background_color;
        border_color = &this->released_border_color;
        cross_color = &this->released_cross_color;
        break;
    }
    default:
    {
        background_color = &this->released_background_color;
        border_color = &this->released_border_color;
        cross_color = &this->released_cross_color;
        break;
    }
    }

    // draw the background color
    this->display->setColor(*background_color);
    this->display->fillRect(this->upper_left.x_pos, this->upper_left.y_pos,
                            this->lower_right.x_pos, this->lower_right.y_pos);

    // draw the border
    this->display->setColor(*border_color);
    this->display->drawRect(this->upper_left.x_pos, this->upper_left.y_pos,
                            this->lower_right.x_pos, this->lower_right.y_pos);

    // draw the X
    this->display->setColor(*cross_color);
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
            this->_draw_content(Widget::w_status::S_TOUCHED);
        }
        else
        {
            this->_draw_content(Widget::w_status::S_RELEASED);
        }
    }
}