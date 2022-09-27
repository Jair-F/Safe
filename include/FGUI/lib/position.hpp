#pragma once
#include <Arduino.h>

namespace FGUI
{
    /**
     * \defgroup Position
     * \ingroup FGUI
     *
     * @brief integral position type of the display - the only way to specify the positions
     * on the windows...
     *
     * @{
     */
    struct position
    {
        uint16_t x_pos; ///< @brief horizontal counter
        uint16_t y_pos; ///< @brief vertical counter
    };

    /** @} */

} // namespace FGUI
