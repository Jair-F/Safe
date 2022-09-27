#pragma once
#include "Widget.hpp"
#include <LinkedList.hpp>
#include <stdarg.h>

namespace FGUI
{
    /**
     * \defgroup Giff
     * \ingroup FGUI
     *
     * @brief a class which iterates over list of bitmaps and prints it in defined periods of time like a giff.
     *
     * @details this class holds a list of bitmaps and prints them priodically in a defined amount of time like a giff.
     *
     * @note Limitations: all the bitmaps must have the same height and width. The maximum number of bitmaps is 255.
     *
     * @{
     */
    class Giff : public Widget
    {
    public:
        /**
         * @note all the bitmaps must have the same size!!
         * @param _change_speed num of milliseconds to wait between bitmap change
         * @param num_of_bitmaps number of bitmaps you gonna pass to the constructor
         * @param ... the bitmaps in the order they should be displayed afterwards on the display. type of the bitmap is unsigned short*
         */
        Giff(WindowBase *_parent, position _upper_left, uint16_t _bitmap_width, uint16_t _bitmap_height,
             uint8_t _border_weight, uint8_t _border_to_bitmap_gap, uint16_t _change_speed,
             uint8_t num_of_bitmaps, ...);
        ~Giff() {}

        /**
         * @param _speed time to wait between bitmap changes in milliseconds.
         */
        inline void set_bitmap_change_speed(uint16_t _speed) { this->bitmap_change_speed = _speed; }
        /**
         * @return wat time between bitmap changes in milliseconds.
         */
        inline uint16_t get_bitmap_change_speed() const { return this->bitmap_change_speed; }

        void loop() override;

        /**
         * @brief stop running the gif. holds until resume() is called.
         */
        void stop();

        /**
         * @brief resume the stopped gif at the last shown picture
         */
        void resume();

        /**
         * @return true if the giff is changing bitmaps - running
         */
        inline bool is_running() const { return this->running; }

    protected:
        void _draw_content(Widget::w_status _st) override;

    private:
        uint8_t border_to_bitmap_gap;
        SinglyLinkedList<unsigned short *> bitmaps;              ///< @brief list of bitmaps in the order they are displayed.
        SinglyListNodeIterator<unsigned short *> current_bitmap; ///< @brief iterator or the current bitmap.
        uint16_t bitmap_change_speed;                            ///< @brief change speed of the bitmaps in milliseconds.

        unsigned long last_bitmap_change_tm_pt = 0; ///< @brief last bitmap change time point

        bool running = true; ///< @brief true if the gif is running(not stopped)
    };

    /** @} */

} // namespace FGUI
