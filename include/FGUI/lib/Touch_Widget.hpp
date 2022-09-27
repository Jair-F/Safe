#pragma once
#include "Widget.hpp"

namespace FGUI
{
    /**
     * \defgroup TouchWidget Touch_Widget
     * \ingroup FGUI
     *
     * @brief Base class for all widgets which can be touched. It handles callback functions
     * which are executed at touch, release focus_loose.
     *
     * @note !! the callback functions of every touch widget must be class member functions - cant be "standalone functions".
     *
     * @details It holds virtual functions which draw the border and background of the widget in touched and released state.
     * they are good for most cases but can be reimplemented by derived classes.
     *
     * @{
     */

    template <typename CALL_OBJECT_TYPE>
    class Touch_Widget : public Widget
    {
        // documentating the template parameter
        /**
         * @tparam CALL_OBJECT_TYPE a instance of the class of which the callback functions for on_touch, on_release and on_focus_loose are called with.
         */

    private:
        /**
         * @brief true if the widget holds the focus.
         * @details the widget with the focus is the last widget that has been touched or requested the focus manually
         * and the focus wasnt frozen. the focused widget will get the keypad input which was sent to the MainWindow.
         */
        bool focused;
        /**
         * @brief true if the widget is touched at the time
         */
        bool touched;

        /**
         * @brief true if the widget is disabled.
         * @details disabled is only for touch widget - if they are disabled the cant be touched
         */
        bool disabled = false;

    protected:
        /**
         * @brief call object which the function pionter will be called with
         * @details the callback function is a class member function and needs to be called with the corresponding class object of the class.
         */
        CALL_OBJECT_TYPE *call_object;

        virtual void _draw_border(Widget::w_status _st) override;
        virtual void _draw_background(Widget::w_status _st) override;
        /*
            for touch_widgets we need released and pressed widget...
        */
        virtual void _draw_content(Widget::w_status _st) {}
        void _draw_widget() override;

    public:
        /**
         * @param _parent the parent window to which the widget will register to
         * @param _upper_left_pos upper left corner in relation to the parent window zero point
         * @param _lower_right lower right corner in relation to the parent window zero point
         * @param _call_object a instance of the class of which the callback functions for on_touch, on_release and on_focus_loose are called with.
         * @param _border_weight size of the border in pixels
         * @note if _call_object nullptr the programm will crash due a assertion!
         */
        Touch_Widget(WindowBase *_parent, const position &_upper_left,
                     const position &_lower_right, CALL_OBJECT_TYPE *_call_object,
                     uint8_t _border_weight = 1) : Widget(_parent, _upper_left, _lower_right, _border_weight),
                                                   focused(false),
                                                   touched(false),
                                                   call_object(_call_object)
        {
            assert(this->call_object != nullptr);
        }
        /**
         * @param _parent the parent window to which the widget will register to
         * @param _upper_left_pos upper left corner in relation to the parent window zero point
         * @param _width the width of the widget.
         * @param _height the height of the widget.
         * @param _call_object a instance of the class of which the callback functions for on_touch, on_release and on_focus_loose are called with.
         * @param _border_weight size of the border in pixels
         * @note if _call_object nullptr the programm will crash due a assertion!
         */
        Touch_Widget(WindowBase *_parent, const position &_upper_left,
                     uint16_t _width, uint16_t _height, CALL_OBJECT_TYPE *_call_object,
                     uint8_t _border_weight = 1) : Widget(_parent, _upper_left, _width, _height, _border_weight),
                                                   focused(false), touched(false), call_object(_call_object)
        {
            assert(this->call_object != nullptr);
        }

        virtual ~Touch_Widget() {}

        /**
         * @details colors for different states of the widget.
         * The color values are RGB-565 values(16-bit value).
         * RGB-565 color picker: https://chrishewett.com/blog/true-rgb565-colour-picker/
         * @{
         */
        unsigned int touched_background_color = VGA_WHITE;
        unsigned int touched_border_color = VGA_WHITE;

        unsigned int disabled_background_color = VGA_BLACK;
        unsigned int disabled_border_color = VGA_GRAY;
        /** @} */

        /**
         * callback function which will be called if the widget is touched
         * it has to be a class member function - no static function
         * @param widget the widget from where the touch comming
         */
        void (CALL_OBJECT_TYPE::*on_touch)(Touch_Widget *_widget) = nullptr;
        /**
         * callback function which will be called if the widget is released
         * it has to be a class member function - no static function
         * @param widget the widget from where the release comming
         */
        void (CALL_OBJECT_TYPE::*on_release)(Touch_Widget *_widget) = nullptr;
        /**
         * callback function which will be called if the widget loses its focus
         * it has to be a class member function - no static function
         * @param widget the widget from where the focus lose comming
         */
        void (CALL_OBJECT_TYPE::*on_focus_lose)(Touch_Widget *_widget) = nullptr;

        /**
         * @details for sending a touch event.
         * @param _touch_data absolute position of the touch
         */
        void _touch(const position &_touch_data) override;
        /**
         * @details for sending a release event.
         * @param _touch_data absolute position of the release
         */
        void _release(const position &_touch_data) override;
        /**
         * @details if the release of the touch was outside of the widget the widget will be redrawn
         * to the released state without calling the on_release callback function.
         * @brief for sending a reset_touch event to the widget.
         */
        void _reset_touch() override;
        /**
         * @brief for sending a focus_lose event to the widget.
         * @details calls the _focus_lose callback function.
         */
        void _focus_lose() override;

        /**
         * @return true if the widget is focused.
         * @details if the element has focus it gets the input of the keypad.
         * the focus is the last element that was touched or requested and got focus manually.
         */
        inline bool is_focused() const { return this->parent_window->get_focused_widget() == this; }

        /**
         * @return true if the widget is disabled.
         */
        inline bool is_disabled() const override { return this->disabled; }
        /**
         * @param _disabled true if the widget should be disabled
         * @note the widget will be redrawn automatically with disabled colors
         * @details if a widget is disabled it cant be touched
         */
        void set_disabled(bool _disabled) override;

        /**
         * @return true if the widget is touched at the time
         */
        inline bool is_touched() const { return this->touched; }
    };

    /** @} */

}

// ------------- template implementation -------------

template <typename CALL_OBJECT_TYPE>
void FGUI::Touch_Widget<CALL_OBJECT_TYPE>::_touch(const position &_touch_data)
{
    if (!this->is_hidden())
    {
        this->touched = true; // needs to be set first - the draw_widget draws the widget according to that
        this->_draw_widget(); // draw the widget according to the actual state
        // this->_parent_window->request_focus(this);
        if (this->on_touch != nullptr)
        {
            (this->call_object->*this->on_touch)(this); // calling the on_release func which is set by the user with the object
        }
    }
}

template <typename CALL_OBJECT_TYPE>
void FGUI::Touch_Widget<CALL_OBJECT_TYPE>::_release(const position &_touch_data)
{
    this->touched = false; // needs to be set first - the draw_widget draws the widget according to that
    this->_draw_widget();  // draw the widget according to the actual state
    if (this->on_release != nullptr)
    {
        (this->call_object->*this->on_release)(this); // calling the on_release func which is set by the user with the object
    }
}

template <typename CALL_OBJECT_TYPE>
void FGUI::Touch_Widget<CALL_OBJECT_TYPE>::_reset_touch()
{
    this->touched = false;
    this->_draw_widget();
}

template <typename CALL_OBJECT_TYPE>
void FGUI::Touch_Widget<CALL_OBJECT_TYPE>::_focus_lose()
{
    if (this->on_focus_lose != nullptr)
        (this->call_object->*this->on_focus_lose)(this);
}

template <typename CALL_OBJECT_TYPE>
void FGUI::Touch_Widget<CALL_OBJECT_TYPE>::_draw_border(Widget::w_status _st)
{
    switch (_st)
    {
    case Widget::w_status::S_DISABLED:
    {
        this->display->setColor(this->disabled_border_color);
        break;
    }
    case Widget::w_status::S_TOUCHED:
    {
        this->display->setColor(this->touched_border_color);
        break;
    }
    case Widget::w_status::S_RELEASED:
    {
        this->display->setColor(this->released_border_color);
        break;
    }
    default:
    {
        break;
    }
    }

    for (uint8_t i = 0; i < this->get_border_weight(); ++i)
    {
        this->display->drawRect(this->upper_left.x_pos + i, this->upper_left.y_pos + i, this->lower_right.x_pos - i, this->lower_right.y_pos - i);
    }
}

template <typename CALL_OBJECT_TYPE>
void FGUI::Touch_Widget<CALL_OBJECT_TYPE>::_draw_background(Widget::w_status _st)
{
    position background_upper_left = this->get_content_upper_left();
    position background_lower_right = this->get_content_lower_right();

    switch (_st)
    {
    case Widget::w_status::S_DISABLED:
    {
        this->display->setColor(this->disabled_background_color);
        break;
    }
    case Widget::w_status::S_TOUCHED:
    {
        this->display->setColor(this->touched_background_color);
        break;
    }
    case Widget::w_status::S_RELEASED:
    {
        this->display->setColor(this->released_background_color);
        break;
    }
    default:
    {
        break;
    }
    }

    this->display->fillRect(background_upper_left.x_pos, background_upper_left.y_pos, background_lower_right.x_pos, background_lower_right.y_pos);
}

template <typename CALL_OBJECT_TYPE>
void FGUI::Touch_Widget<CALL_OBJECT_TYPE>::_draw_widget()
{
    if (!this->is_hidden())
    {
        if (this->is_disabled())
        {
            if (this->get_draw_border())
            {
                this->_draw_border(Widget::w_status::S_DISABLED);
            }
            this->_draw_background(Widget::w_status::S_DISABLED);
            this->_draw_content(Widget::w_status::S_DISABLED);
        }
        else if (this->is_touched())
        {
            if (this->get_draw_border())
            {
                this->_draw_border(Widget::w_status::S_TOUCHED);
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
            this->_draw_background(Widget::w_status::S_RELEASED);
            this->_draw_content(Widget::w_status::S_RELEASED);
        }
    }
}

template <typename CALL_OBJECT_TYPE>
void FGUI::Touch_Widget<CALL_OBJECT_TYPE>::set_disabled(bool _disabled)
{
    if (_disabled != this->disabled)
    {
        this->disabled = _disabled;
        this->_draw_widget();
    }
}
