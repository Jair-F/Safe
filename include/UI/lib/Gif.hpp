#pragma once
#include "Widget.hpp"
#include <LinkedList.hpp>
#include <stdarg.h>

namespace UI
{
    class Giff : public Widget
    {
    public:
        /*
            all the bitmaps must have the same size!!
            @param _change_speed num of milliseconds to wait between bitmap change
            @param num_of_bitmaps number of bitmaps you gonna pass to the constructor
            @param ... the bitmaps in the order they should be displayed afterwards on the display
        */
        Giff(WindowBase *_parent, position _upper_left, uint16_t _bitmap_width, uint16_t _bitmap_height,
             uint8_t _border_weight, uint8_t _border_to_bitmap_gap, uint16_t _change_speed,
             uint8_t num_of_bitmaps, ...);
        ~Giff() {}

        inline void set_bitmap_change_speed(uint16_t _speed) { this->bitmap_change_speed = _speed; }
        inline uint16_t get_bitmap_change_speed() const { return this->bitmap_change_speed; }

        void loop() override;

        /*
            stop running the gif -
        */
        void stop();

        /*
            resume the stopped gif at the last shown picture
        */
        void resume();

        /*
            @return true if the giff is changing bitmaps - running
                    false if the giff is paused/stopped
        */
        inline bool is_running() const { return this->running; }

    protected:
        void _draw_released_content() override;

    private:
        uint8_t border_to_bitmap_gap;
        SinglyLinkedList<unsigned short *> bitmaps;
        SinglyListNodeIterator<unsigned short *> current_bitmap; // to loop over the images easily
        uint16_t bitmap_change_speed;

        unsigned long last_bitmap_change_tm_pt = 0; // last bitmap change time point

        bool running = true; // true if the gif is running(not stopped), false if stopped(resume to continue running)
    };
} // namespace UI
