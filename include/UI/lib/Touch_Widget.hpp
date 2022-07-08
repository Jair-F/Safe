#pragma once
#include "Widget.hpp"

namespace UI
{
    /*
        Widget which can be touched for class member functions
    */
    template <typename CALL_OBJECT_TYPE>
    class Touch_Widget : public Widget
    {
    private:
        CALL_OBJECT_TYPE *call_object; // call object which the function pionter will be called with
        bool focused;                  // if the object has the focus - the last element that was clicked
        bool touched;                  // true if the widget is touched at the time - else false

    public:
        Touch_Widget(Window *_parent, const position _upper_left_pos,
                     const position _lower_right_pos, CALL_OBJECT_TYPE *_call_object) : Widget(_parent, _upper_left_pos, _lower_right_pos),
                                                                                        call_object(_call_object), on_click(nullptr), on_release(nullptr) {}
        Touch_Widget(Window *_parent, const position _upper_left_pos,
                     const position _lower_right_pos, CALL_OBJECT_TYPE *_call_object,
                     void (CALL_OBJECT_TYPE::*_on_click)(), void (CALL_OBJECT_TYPE::*_on_release)()) : Widget(_parent, _upper_left_pos, _lower_right_pos),
                                                                                                       call_object(_call_object), on_click(_on_click), on_release(_on_release) {}

        /*
            function to be called if the widget is touched
            if it is  a class member function it has to be a static function
            @param widget the widget from where the function call is comming
        */
        void (CALL_OBJECT_TYPE::*on_click)(Touch_Widget *_widget);
        /*
            function to be called if the widget is released - instantly after the finger is put of from the widget
            if it is  a class member function it has to be a static function
            @param widget the widget from where the function call is comming
        */
        void (CALL_OBJECT_TYPE::*on_release)(Touch_Widget *_widget);

        void _click() override;
        virtual void _release() override;
        virtual void _reset_click() override;

        /*
            if the element has focus it gets the input of the keypad - the last element that was touched
        */
        bool is_focused() { return this->focused; }

        /*
            @return true if the widget is touched at the time, else false
        */
        bool is_touched() { return this->touched; }
    };

}

// ------------- template implementation -------------

template <typename CALL_OBJECT_TYPE>
void UI::Touch_Widget<CALL_OBJECT_TYPE>::_click()
{
    if (!this->is_hidden())
    {
        this->touched = true;
        this->_draw_widget(); // draw the widget according to the actual state
        // this->_parent_window->request_focus(this);
        if (this->on_click != nullptr)
        {
            (this->call_object->*this->on_click)(this); // calling the on_release func which is set by the user with the object
        }
    }
}

template <typename CALL_OBJECT_TYPE>
void UI::Touch_Widget<CALL_OBJECT_TYPE>::_release()
{
    if (!this->is_hidden())
    {
        this->touched = false;
        this->_draw_widget(); // draw the widget according to the actual state
        if (this->on_release != nullptr)
        {
            (this->call_object->*this->on_release)(this); // calling the on_release func which is set by the user with the object
        }
    }
}

template <typename CALL_OBJECT_TYPE>
void UI::Touch_Widget<CALL_OBJECT_TYPE>::_reset_click()
{
    if (!this->is_hidden())
    {
        this->touched = false;
        this->_draw_released_widget();
    }
}
