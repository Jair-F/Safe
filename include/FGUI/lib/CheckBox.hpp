#pragma once
#include "Touch_Widget.hpp"
#include "../display_geometry_funcs.hpp"

namespace FGUI
{
    /**
     * \defgroup CheckBox
     * \ingroup FGUI
     *
     * @{
     */
    template <typename CALL_OBJECT_TYPE>
    class CheckBox : public Touch_Widget<CALL_OBJECT_TYPE>
    {
        // documentating the template parameter
        /**
         * @tparam CALL_OBJECT_TYPE a instance of the class of which the callback functions for on_touch, on_release and on_focus_loose are called with.
         */

    public:
        /**
         * @param _parent the parent window to which the widget will register to
         * @param _upper_left upper left corner in relation to the parent window zero point
         * @param _size height and width in pixels
         * @param _call_object a instance of the class of which the callback functions for on_touch, on_release and on_focus_loose are called with.
         */
        CheckBox(WindowBase *_parent,
                 position _upper_left,
                 uint16_t _size,
                 CALL_OBJECT_TYPE *_call_object,
                 uint8_t _border_weight = 2);

        virtual ~CheckBox() {}

        /**
         * @return true if the checkbox is checked
         */
        inline bool is_checked() const { return this->checked; }

        /**
         * @param _checked true to set the checkbox to checked.
         */
        void set_checked(bool _checked)
        {
            this->checked = _checked;
        }

        /**
         * @details when the MainWindow sends a _release event we change the state of the checkbox.
         */
        void _release(const position &_touch_data) override;

        /**
         * @details colors for different states of the widget.
         * The color values are RGB-565 values(16-bit value).
         * RGB-565 color picker: https://chrishewett.com/blog/true-rgb565-colour-picker/
         * @{
         */
        unsigned int normal_check_sign_color = VGA_WHITE;
        unsigned int disabled_check_sign_color = VGA_GRAY;
        /** @} */

    protected:
        void _draw_widget() override;
        void _draw_content(Widget::w_status _st) override;

    private:
        bool checked; ///< @brief true if the checkbox is checked, false if not
    };

    /** @} */

} // namespace FGUI

// ------------- template implementation -------------

template <typename CALL_OBJECT_TYPE>
FGUI::CheckBox<CALL_OBJECT_TYPE>::CheckBox(WindowBase *_parent, const position _upper_left,
                                           uint16_t _size, CALL_OBJECT_TYPE *_call_object,
                                           uint8_t _border_weight) : Touch_Widget<CALL_OBJECT_TYPE>(_parent,
                                                                                                    _upper_left,
                                                                                                    _size, _size,
                                                                                                    _call_object,
                                                                                                    _border_weight,
                                                                                                    _size * 0.035714),
                                                                     checked(false)
{
}

template <typename CALL_OBJECT_TYPE>
void FGUI::CheckBox<CALL_OBJECT_TYPE>::_draw_content(Widget::w_status _st)
{
    switch (_st)
    {
    case Widget::w_status::S_DISABLED:
    {
        this->display->setColor(this->disabled_check_sign_color);
        break;
    }
    case Widget::w_status::S_TOUCHED:
    {
        this->display->setColor(this->normal_check_sign_color);
        break;
    }
    case Widget::w_status::S_RELEASED:
    {
        this->display->setColor(this->normal_check_sign_color);
        break;
    }
    default:
    {
        break;
    }
    }

    position content_upper_left = this->get_content_upper_left();
    position content_lower_right = this->get_content_lower_right();

    uint16_t content_height = this->get_content_height();
    uint8_t check_sign_weight = round(content_height * 0.15);

    double small_check_length = content_height * 0.45195; // length of the small check sign
    double long_check_length = content_height * 0.82;  // length of the long check sign
    double center_gap = content_height * 0.05;            // gap in order to center the check mark in the content space

    position small_check_center_pos, long_check_center_pos;
    small_check_center_pos.x_pos = round((double)content_upper_left.x_pos + (double)content_height * 0.223);
    small_check_center_pos.y_pos = round((double)content_lower_right.y_pos - center_gap - (double)content_height * 0.2934);
    long_check_center_pos.x_pos = round((double)content_lower_right.x_pos - (double)content_height * 0.4);
    long_check_center_pos.y_pos = round((double)content_lower_right.y_pos - center_gap - (double)content_height * 0.45);

    fill_rect(this->display, long_check_center_pos, long_check_length, check_sign_weight, 49);
    fill_rect(this->display, small_check_center_pos, small_check_length, check_sign_weight, 137);
}

template <typename CALL_OBJECT_TYPE>
void FGUI::CheckBox<CALL_OBJECT_TYPE>::_draw_widget()
{
    if (!this->is_hidden())
    {
        if (this->is_disabled())
        {
            if (this->get_draw_border())
            {
                this->_draw_border(Widget::w_status::S_DISABLED);
            }
            else
            {
                this->_clear_border_space();
            }
            this->_draw_background(Widget::w_status::S_DISABLED);
            if (this->is_checked())
                this->_draw_content(Widget::w_status::S_DISABLED);
        }
        // change the state only on touch. otherwise it would change on touch to checked and on release return to not checked!
        else if (this->is_checked())
        {
            if (this->get_draw_border())
            {
                this->_draw_border(Widget::w_status::S_TOUCHED);
            }
            else
            {
                this->_clear_border_space();
            }
            this->_draw_background(Widget::w_status::S_TOUCHED);
            this->_draw_content(Widget::w_status::S_TOUCHED);
        }
        else
        {
            if (this->get_draw_border())
            {
                this->_draw_border(Widget::w_status::S_RELEASED);
            }
            else
            {
                this->_clear_border_space();
            }
            this->_draw_background(Widget::w_status::S_RELEASED);
            // this->_draw_content(Widget::w_status::S_RELEASED);
        }
    }
}

template <typename CALL_OBJECT_TYPE>
void FGUI::CheckBox<CALL_OBJECT_TYPE>::_release(const position &_touch_data)
{
    if (!this->is_hidden())
    {
        this->checked = !this->checked;
        Touch_Widget<CALL_OBJECT_TYPE>::_release(_touch_data); // draw_widget is called in the touch_widget::click func
    }
}
