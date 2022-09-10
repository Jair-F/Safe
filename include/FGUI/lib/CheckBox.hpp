#pragma once
#include "Touch_Widget.hpp"
#include "../display_geometry_funcs.hpp"

namespace FGUI
{
    template <typename CALL_OBJECT_TYPE>
    class CheckBox : public Touch_Widget<CALL_OBJECT_TYPE>
    {
    public:
        /*
            @param _size height and width in pixels
        */
        CheckBox(Window *_parent, position _upper_left, uint16_t _size, CALL_OBJECT_TYPE *_call_object);

        virtual ~CheckBox() {}

        /*
            @return true if the checkbox is checked
        */
        inline bool is_checked() const { return this->checked; }

        /*
            @param true if checked
        */
        void set_checked(bool _checked)
        {
            this->checked = _checked;
        }

        /*
            will be called by the main_window
            we change the state of the check_box only on the release of the touch
        */
        void _release(const position &_touch_data) override;

        /*
            the color values are RGB-565 values(16-bit value)
            RGB-565 color picker: https://chrishewett.com/blog/true-rgb565-colour-picker/
        */
        unsigned int check_sign_color = VGA_WHITE;

    protected:
        void _draw_widget() override;
        void _draw_touched_content() override;

    private:
        bool checked; // true if the checkbox is checked, false if not
    };
} // namespace FGUI

// ------------- template implementation -------------

template <typename CALL_OBJECT_TYPE>
FGUI::CheckBox<CALL_OBJECT_TYPE>::CheckBox(Window *_parent, const position _upper_left,
                                           uint16_t _size, CALL_OBJECT_TYPE *_call_object) : Touch_Widget<CALL_OBJECT_TYPE>(_parent, _upper_left, _size, _size, _call_object), checked(false)
{
}

template <typename CALL_OBJECT_TYPE>
void FGUI::CheckBox<CALL_OBJECT_TYPE>::_draw_touched_content()
{
    uint8_t gap_from_box = round(this->width() * 0.035714); // the gap minimal between the box and the begin of the check mark

    // upper_left and lower_right of the widget - calculated border_weight out
    position content_upper_left = this->get_content_upper_left();
    position content_lower_right = this->get_content_lower_right();
    content_upper_left.x_pos = content_upper_left.x_pos + gap_from_box;
    content_upper_left.y_pos = content_upper_left.y_pos + gap_from_box;
    content_lower_right.x_pos = content_lower_right.x_pos - gap_from_box;
    content_lower_right.y_pos = content_lower_right.y_pos - gap_from_box;

    double content_height = content_lower_right.x_pos - content_upper_left.x_pos; // content width/height - square
    uint8_t check_sign_weight = round(content_height * 0.15);

    this->display->setColor(this->check_sign_color);

    double small_check_length = content_height * 0.45195; // length of the small check sign
    double long_check_length = content_height * 1.03413;  // length of the long check sign
    double center_gap = content_height * 0.05;            // gap in order to center the check mark in the content space

    position small_check_middle_pos,
        long_check_middle_pos;
    small_check_middle_pos.x_pos = round(content_upper_left.x_pos + content_height * 0.223);
    small_check_middle_pos.y_pos = round(content_lower_right.y_pos - center_gap - content_height * 0.2934);
    long_check_middle_pos.x_pos = round(content_lower_right.x_pos - content_height * 0.38239);
    long_check_middle_pos.y_pos = round(content_lower_right.y_pos - center_gap - content_height * 0.45);

    /*

















    */
    fill_rect(this->display, long_check_middle_pos, long_check_length, check_sign_weight, 51);
    fill_rect(this->display, small_check_middle_pos, small_check_length, check_sign_weight, 123);

    return;

    // content_upper_left.y_pos = content_upper_left.y_pos + check_sign_weight / 2;
    // content_lower_right.y_pos = content_lower_right.y_pos + check_sign_weight / 2;

    FGUI::position c_m_lower_peak, // c_m - check_mark
        c_m_s_line_upper_peak,     // short line...
        c_m_l_line_upper_peak;     // long line...

    c_m_lower_peak.x_pos = content_upper_left.x_pos + (this->get_content_width() - gap_from_box * 2) * 0.3; // end of small check line is 30 % of the content width
    c_m_lower_peak.y_pos = content_lower_right.y_pos;
    c_m_s_line_upper_peak.x_pos = content_upper_left.x_pos;
    c_m_s_line_upper_peak.y_pos = content_upper_left.y_pos + (this->get_content_height() - gap_from_box * 2) * 0.4; // small check_line is  20 % of the content height
    c_m_l_line_upper_peak.x_pos = content_lower_right.x_pos;
    c_m_l_line_upper_peak.y_pos = content_upper_left.y_pos;

    // short line
    uint16_t width, height;
    height = c_m_lower_peak.y_pos - c_m_s_line_upper_peak.y_pos;
    width = c_m_lower_peak.x_pos - c_m_s_line_upper_peak.x_pos;

    for (unsigned short i = 0; i < check_sign_weight; ++i)
    {
        // check-mark - short line
        this->display->drawLine(content_upper_left.x_pos + gap_from_box,
                                (content_upper_left.y_pos + gap_from_box + this->get_content_height() * 0.4) - i,
                                (content_upper_left.x_pos + gap_from_box + this->get_content_width() * 0.3),
                                content_lower_right.y_pos - gap_from_box - i);

        // check-mark - long line
        this->display->drawLine((content_upper_left.x_pos + gap_from_box + this->get_content_width() * 0.3),
                                content_lower_right.y_pos - gap_from_box - i,
                                content_lower_right.x_pos - gap_from_box,
                                content_upper_left.y_pos + gap_from_box - i);
    }
}

template <typename CALL_OBJECT_TYPE>
void FGUI::CheckBox<CALL_OBJECT_TYPE>::_draw_widget()
{
    if (!this->is_hidden())
    {
        // change the state only on touch. otherwise it would change on touch to checked and on release return to not checked!
        if (this->is_checked())
        {
            if (this->get_draw_border())
            {
                this->_draw_touched_border();
            }
            this->_draw_touched_background();
            this->_draw_touched_content();
        }
        else
        {
            if (this->get_draw_border())
            {
                this->_draw_released_border();
            }
            this->_draw_released_background();
            this->_draw_released_content();
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
