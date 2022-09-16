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
    private:
    };
}
