#pragma once

#include "ButtonBase.hpp"
#include <UTFT.h>
#include <URTouch.h>

extern uint8_t SmallFont[];
extern uint8_t BigFont[];

namespace FGUI
{
    /**
     * \defgroup Button
     * \ingroup FGUI
     *
     * @brief class of a normal clickable button labled with text.
     *
     * @{
     */
    template <typename CALL_OBJECT_TYPE>
    class Button : public ButtonBase<CALL_OBJECT_TYPE>
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
         */
        Button(WindowBase *parent,
               position _upper_left, position _lower_right,
               CALL_OBJECT_TYPE *_call_object,
               uint8_t _border_weight = 1,
               uint8_t _border_content_gap = 0) : ButtonBase<CALL_OBJECT_TYPE>(parent,
                                                                               _upper_left, _lower_right,
                                                                               _call_object,
                                                                               _border_weight, _border_content_gap) {}
        /**
         * @param _parent the parent window to which the widget will register to
         * @param _upper_left upper left corner in relation to the parent window zero point
         * @param _width the width of the button in pixels.
         * @param _height the height of the button in pixels.
         * @param _call_object a instance of the class of which the callback functions for on_touch, on_release and on_focus_loose are called with.
         */
        Button(WindowBase *parent,
               position _upper_left,
               uint16_t _width, uint16_t _height,
               CALL_OBJECT_TYPE *_call_object,
               uint8_t _border_weight = 1,
               uint8_t _border_content_gap = 0) : ButtonBase<CALL_OBJECT_TYPE>(parent, _upper_left,
                                                                               _width, _height,
                                                                               _call_object,
                                                                               _border_weight, _border_content_gap) {}

        Button(WindowBase *parent,
               uint16_t _content_width, uint16_t _content_height,
               position _lower_right,
               CALL_OBJECT_TYPE *_call_object,
               uint8_t _border_weight = 1,
               uint8_t _border_content_gap = 0) : ButtonBase<CALL_OBJECT_TYPE>(parent,
                                                                               _content_width, _content_height,
                                                                               _lower_right,
                                                                               _call_object,
                                                                               _border_weight, _border_content_gap) {}

        virtual ~Button() {}

        /**
         * @param _font pointer to font array for the text.
         */
        void setFont(uint8_t *_font)
        {
            this->_text_font = _font;
        }

        /**
         * @param _text text to releable the button
         * @note the button isn't redrawn with this call - redraw the button manually.
         */
        void setText(String _text)
        {
            text = _text;
        }

        /**
         * @details colors for different states of the widget.
         * The color values are RGB-565 values(16-bit value).
         * RGB-565 color picker: https://chrishewett.com/blog/true-rgb565-colour-picker/
         * @{
         */
        unsigned int released_text_color = VGA_WHITE;
        unsigned int touched_text_color = VGA_BLACK;
        unsigned int disabled_text_color = VGA_GRAY;
        /** @} */

    protected:
        void _draw_content(Widget::w_status _st) override;

    private:
        String text;
        uint8_t *_text_font = SmallFont;
    };

    /** @} */

}

// ------------- template implementation -------------

template <typename CALL_OBJECT_TYPE>
void FGUI::Button<CALL_OBJECT_TYPE>::_draw_content(Widget::w_status _st)
{
    switch (_st)
    {
    case Widget::w_status::S_DISABLED:
    {
        this->display->setColor(this->disabled_text_color);
        this->display->setBackColor(this->disabled_background_color);
        break;
    }
    case Widget::w_status::S_TOUCHED:
    {
        this->display->setColor(this->touched_text_color);
        this->display->setBackColor(this->touched_background_color);
        break;
    }
    case Widget::w_status::S_RELEASED:
    {
        this->display->setColor(this->released_text_color);
        this->display->setBackColor(this->released_background_color);
        break;
    }
    default:
        break;
    }

    this->display->setFont(this->_text_font);
    uint8_t font_height = this->display->getFontYsize();
    uint8_t font_width = this->display->getFontXsize();

    // print the button-label

    this->display->print(this->text, this->upper_left.x_pos + (this->width() / 2) - (font_width * text.length() / 2),
                         this->lower_right.y_pos - (this->height() / 2) - font_height / 2);
}
