#pragma once

#include "Touch_Widget.hpp"

namespace FGUI
{
    /**
     * \defgroup ButtonBase
     * \ingroup FGUI
     *
     * @brief Base class for all types of buttons.
     *
     * @details only made previously to group the buttons - now actually its not useful/needed but stayed.
     *
     * @{
     */
    template <typename CALL_OBJECT_TYPE>
    class ButtonBase : public Touch_Widget<CALL_OBJECT_TYPE>
    {
        // documentating the template parameter
        /**
         * @tparam CALL_OBJECT_TYPE a instance of the class of which the callback functions for on_touch, on_release and on_focus_loose are called with.
         */

    public:
        /**
         * @param _parent the parent window to which the widget will register to
         * @param _upper_left upper left corner in relation to the parent window zero point
         * @param _lower_right lower right corner in relation to the parent window zero point
         * @param _call_object a instance of the class of which the callback functions for on_touch, on_release and on_focus_loose are called with.
         * @param _border_weight size of the border in pixels.
         */
        ButtonBase(WindowBase *parent, position _upper_left, position _lower_right,
                   CALL_OBJECT_TYPE *_call_object, uint8_t _border_weight = 1) : Touch_Widget<CALL_OBJECT_TYPE>(parent, _upper_left, _lower_right, _call_object, _border_weight) {}

        virtual ~ButtonBase() {}

    protected:
    private:
    };

    /** @} */

}
