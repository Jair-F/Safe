#pragma once

#include "Touch_Widget.hpp"

namespace FGUI
{
    template <typename CALL_OBJECT_TYPE>
    class ButtonBase : public Touch_Widget<CALL_OBJECT_TYPE>
    {
    public:
        ButtonBase(WindowBase *parent, position _upper_left, position _lower_right,
                   CALL_OBJECT_TYPE *_call_object, uint8_t _border_weight = 1) : Touch_Widget<CALL_OBJECT_TYPE>(parent, _upper_left, _lower_right, _call_object, _border_weight) {}

        virtual ~ButtonBase() {}

    protected:
        virtual void _draw_widget() override;
        virtual void _draw_released_content() override {}
        virtual void _draw_touched_content() override {}

    private:
    };
}

// ------------- template implementation -------------

template <typename CALL_OBJECT_TYPE>
void FGUI::ButtonBase<CALL_OBJECT_TYPE>::_draw_widget()
{
    if (!this->is_hidden())
    {
        if (this->is_touched())
        {
            this->_draw_touched_border();
            this->_draw_touched_background();
            this->_draw_touched_content();
        }
        else
        {
            this->_draw_released_border();
            this->_draw_released_background();
            this->_draw_released_content();
        }
    }
}