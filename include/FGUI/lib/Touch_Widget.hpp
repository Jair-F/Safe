#pragma once
#include "Widget.hpp"

namespace FGUI
{
    /*
        Widget which can be touched for class member functions
    */
    template <typename CALL_OBJECT_TYPE>
    class Touch_Widget : public Widget
    {
    private:
        bool focused; // if the object has the focus - the last element that was clicked
        bool touched; // true if the widget is touched at the time - else false
    protected:
        CALL_OBJECT_TYPE *call_object; // call object which the function pionter will be called with

        virtual void _draw_touched_border();
        virtual void _draw_touched_background();
        /*
            for touch_widgets we need released and pressed widget...
        */
        virtual void _draw_touched_content() {}

        void _draw_widget() override;

    public:
        Touch_Widget(WindowBase *_parent, const position &_upper_left,
                     const position &_lower_right, CALL_OBJECT_TYPE *_call_object,
                     uint8_t _border_weight = 1) : Widget(_parent, _upper_left, _lower_right, _border_weight),
                                                   focused(false),
                                                   touched(false),
                                                   call_object(_call_object)
        {
            assert(this->call_object != nullptr);
        }
        Touch_Widget(WindowBase *_parent, const position &_upper_left,
                     const position _lower_right, CALL_OBJECT_TYPE *_call_object,
                     void (CALL_OBJECT_TYPE::*_on_touch)(), void (CALL_OBJECT_TYPE::*_on_release)(),
                     uint8_t _border_weight = 1) : Widget(_parent, _upper_left, _lower_right, _border_weight),
                                                   on_release(_on_release), focused(false), touched(false),
                                                   on_touch(_on_touch), call_object(_call_object)
        {
            assert(this->call_object != nullptr);
        }
        Touch_Widget(WindowBase *_parent, const position &_upper_left, uint16_t _width, uint16_t _height, CALL_OBJECT_TYPE *_call_object, uint8_t _border_weight = 1) : Widget(_parent, _upper_left, _width, _height, _border_weight),
                                                                                                                                                                        focused(false), touched(false), call_object(_call_object)
        {
            assert(this->call_object != nullptr);
        }
        Touch_Widget(WindowBase *_parent, const position &_upper_left,
                     uint16_t _width, uint16_t _height, CALL_OBJECT_TYPE *_call_object,
                     void (CALL_OBJECT_TYPE::*_on_touch)(), void (CALL_OBJECT_TYPE::*_on_release)(),
                     uint8_t _border_weight = 1) : Widget(_parent, _upper_left, _width, height, _border_weight),
                                                   focused(false), touched(false), call_object(_call_object),
                                                   on_touch(_on_touch), on_release(_on_release)
        {
            assert(this->call_object != nullptr);
        }

        virtual ~Touch_Widget() {}

        /*
            the color values are RGB-565 values(16-bit value)
            RGB-565 color picker: https://chrishewett.com/blog/true-rgb565-colour-picker/
        */
        unsigned int touched_background_color = VGA_WHITE;
        unsigned int touched_border_color = VGA_WHITE;

        /*
            function to be called if the widget is touched - on touch not bound to a status of the widget except of hidden status!
            it has to be a class member function - no static function
            @param widget the widget from where the function call is comming
        */
        void (CALL_OBJECT_TYPE::*on_touch)(Touch_Widget *_widget) = nullptr;
        /*
            function to be called if the widget is touched - on touch not bound to a status of the widget except of hidden status!
            it has to be a class member function - no static function
            @param widget the widget from where the function call is comming
        */
        void (CALL_OBJECT_TYPE::*on_release)(Touch_Widget *_widget) = nullptr;
        void (CALL_OBJECT_TYPE::*on_focus_lose)(Touch_Widget *_widget) = nullptr;

        /*
            @param _touch_data absolute position of the touch
        */
        void _touch(const position &_touch_data) override;
        /*
            @param _touch_data absolute position of the release of the touch
        */
        void _release(const position &_touch_data) override;
        /*
            if the release of the touch was outside of the widget - draws the widget without calling on_release
        */
        void _reset_touch() override;
        void _focus_lose() override;

        /*
            if the element has focus it gets the input of the keypad - the last element that was touched
        */
        inline bool is_focused() const { return this->parent_window->get_focused_widget() == this; }

        /*
            @return true if the widget is touched at the time, else false
        */
        inline bool is_touched() const { return this->touched; }
    };

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
void FGUI::Touch_Widget<CALL_OBJECT_TYPE>::_draw_touched_border()
{
    this->display->setColor(this->touched_border_color);
    for (uint8_t i = 0; i < this->get_border_weight(); ++i)
    {
        this->display->drawRect(this->upper_left.x_pos + i, this->upper_left.y_pos + i, this->lower_right.x_pos - i, this->lower_right.y_pos - i);
    }
}

template <typename CALL_OBJECT_TYPE>
void FGUI::Touch_Widget<CALL_OBJECT_TYPE>::_draw_touched_background()
{
    position background_upper_left = this->get_content_upper_left();
    position background_lower_right = this->get_content_lower_right();

    this->display->setColor(this->touched_background_color);
    this->display->fillRect(background_upper_left.x_pos, background_upper_left.y_pos, background_lower_right.x_pos, background_lower_right.y_pos);
}

template <typename CALL_OBJECT_TYPE>
void FGUI::Touch_Widget<CALL_OBJECT_TYPE>::_draw_widget()
{
    if (!this->is_hidden())
    {
        if (this->is_touched())
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
