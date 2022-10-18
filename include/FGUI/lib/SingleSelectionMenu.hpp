#pragma once

#include <Arduino.h>
#include <stdarg.h>
#include "Touch_Widget.hpp"
#include "LinkedList.hpp"

namespace FGUI
{
    /**
     * \defgroup SingleSelectionMenu
     * \ingroup FGUI
     *
     * @brief A menu to slecet one option from a list of options.
     * the interaction with the menu is through the send_input(keypad).
     *
     * @details the selection menu gets a list of keys which are scroll forward keys and the same with scroll backward keys.
     * then when a input is sent the menu scrolls itself forward or backwards according to the input which was sent.
     *
     * @{
     */
    template <typename CALL_OBJECT_TYPE>
    class SingleSelectionMenu : public Touch_Widget<CALL_OBJECT_TYPE>
    {
        // documentating the template parameter
        /**
         * @tparam CALL_OBJECT_TYPE a instance of the class of which the callback functions for on_touch, on_release and on_focus_loose are called with.
         */

    public:
        /**
         * @param _parent the parent window to which the widget will register to
         * @param _upper_left_pos upper left corner in relation to the parent window zero point
         * @param _lower_right lower right corner in relation to the parent window zero point
         * @param _call_object a instance of the class of which the callback functions for on_touch, on_release and on_focus_loose are called with.
         * @param _border_weight size of the border in pixels
         * @note if _call_object nullptr the programm will crash due a assertion!
         */
        SingleSelectionMenu(WindowBase *_parent, const position _upper_left,
                            const position _lower_right, CALL_OBJECT_TYPE *_call_object,
                            uint8_t _border_weight = 1);
        virtual ~SingleSelectionMenu();

        /**
         * @param _entry the new entry to add to the list
         */
        void add_entry(String &_entry);
        /**
         * @param _entry the new entry to add to the list
         */
        void add_entry(const char *_entry);
        /**
         * @brief add multiple entries to the list of options.
         * @param num_of_entrys number of entries to add.
         * @param ... the entrys to add with type of const char* or String.
         */
        void add_entrys(uint8_t num_of_entrys, ...);
        /**
         * @return the actual selection
         */
        const String &get_selection() const;

        /**
         * @param _key add _key to the scroll forward keys list.
         */
        void add_scroll_forward_key(char _key);
        /**
         * @param _key add _key to the scroll backward keys list.
         */
        void add_scroll_backward_key(char _key);

        /**
            @details sets the selection manually if _selection is found in the _entrys list
            @param _selection selection to set
            @return true if a matching selection was found in the _entrys list and the selection
            was changed, false if not(nothing was changed).
        */
        bool set_selection(String &_selection);
        /**
            @details sets the selection manually if _selection is found in the _entrys list
            @param _selection selection to set
            @return true if a matching selection was found in the _entrys list and the selection
            was changed, false if not(nothing was changed).
        */
        bool set_selection(const char *_selection);

        /**
         * @brief send the input to trigger a scroll forward or backward if
         * a matching key was found in the scroll forward or backward list.
         */
        void send_input(char _input_data) override;
        void send_enter() override {}

        /**
         * @brief set the font for the selection text printing.
         */
        void set_font(uint8_t *_font) { this->_text_font = _font; }
        /**
         * @return pionter to the text font array.
         */
        uint8_t *get_font() { return this->_text_font; }

        /**
         * @details colors for different states of the widget.
         * The color values are RGB-565 values(16-bit value).
         * RGB-565 color picker: https://chrishewett.com/blog/true-rgb565-colour-picker/
         * @{
         */
        unsigned int touched_text_color = VGA_BLACK;
        unsigned int released_text_color = VGA_WHITE;
        unsigned int disabled_text_color = VGA_GRAY;
        /** @} */

    protected:
        void _draw_widget() override;
        void _draw_content(Widget::w_status _st) override;
        void _focus_lose() override;

    private:
        /**
         * @brief entrys in the selection menu
         */
        DoublyLinkedList<String> entrys;
        /**
         * @brief pointer to the actual selected entry inthe entrys list
         */
        DoublyListNodeIterator<String> *actual_entry;

        /**
         * @brief list of keys which trigger scroll backward.
         */
        SinglyLinkedList<char> scroll_back_keys;
        /**
         * @brief list of keys which trigger scroll forward.
         */
        SinglyLinkedList<char> scroll_forward_keys; // scroll to next selection

        uint8_t *_text_font = SmallFont;
    };

    /** @} */

} // namespace FGUI

// Implementations

template <typename CALL_OBJECT_TYPE>
FGUI::SingleSelectionMenu<CALL_OBJECT_TYPE>::SingleSelectionMenu(WindowBase *_parent, const position _upper_left,
                                                                 const position _lower_right, CALL_OBJECT_TYPE *_call_object,
                                                                 uint8_t _border_weight) : Touch_Widget<CALL_OBJECT_TYPE>(_parent, _upper_left, _lower_right,
                                                                                                                          _call_object, _border_weight),
                                                                                           entrys(), actual_entry(nullptr),
                                                                                           scroll_back_keys(), scroll_forward_keys()
{
}

template <typename CALL_OBJECT_TYPE>
FGUI::SingleSelectionMenu<CALL_OBJECT_TYPE>::~SingleSelectionMenu()
{
    if (this->actual_entry != nullptr)
    {
        delete this->actual_entry;
        this->actual_entry = nullptr;
    }
}

template <typename CALL_OBJECT_TYPE>
void FGUI::SingleSelectionMenu<CALL_OBJECT_TYPE>::add_scroll_forward_key(char _key)
{
    if (this->scroll_forward_keys.search(_key) != -1) // add only if not already in the list
        this->scroll_forward_keys.push_back(_key);
}

template <typename CALL_OBJECT_TYPE>
void FGUI::SingleSelectionMenu<CALL_OBJECT_TYPE>::add_scroll_backward_key(char _key)
{
    if (this->scroll_back_keys.search(_key) != -1) // add only if not already in the list
        this->scroll_back_keys.push_back(_key);
}

template <typename CALL_OBJECT_TYPE>
void FGUI::SingleSelectionMenu<CALL_OBJECT_TYPE>::add_entry(String &_entry)
{
    this->entrys.push_back(_entry);
}

template <typename CALL_OBJECT_TYPE>
void FGUI::SingleSelectionMenu<CALL_OBJECT_TYPE>::add_entry(const char *_entry)
{
    this->entrys.push_back(_entry);
}

template <typename CALL_OBJECT_TYPE>
void FGUI::SingleSelectionMenu<CALL_OBJECT_TYPE>::add_entrys(uint8_t num_of_entrys, ...)
{
    va_list arguments;
    va_start(arguments, num_of_entrys);
    for (uint8_t i = 0; i < num_of_entrys; ++i)
    {
        this->entrys.push_back(va_arg(arguments, String));
    }
    va_end(arguments);
    //#error "implement sort and unique functions for linked list"
    // this->entrys.sort();
    // this->entrys.unique();
}

template <typename CALL_OBJECT_TYPE>
void FGUI::SingleSelectionMenu<CALL_OBJECT_TYPE>::_focus_lose()
{
    Touch_Widget<CALL_OBJECT_TYPE>::_focus_lose();
    this->_draw_widget();
}

template <typename CALL_OBJECT_TYPE>
void FGUI::SingleSelectionMenu<CALL_OBJECT_TYPE>::send_input(char _input_data)
{
    for (uint8_t i = 0; i < this->scroll_forward_keys.length(); ++i)
    {
        if (this->scroll_forward_keys[i] == _input_data)
        {
            if (this->actual_entry != nullptr)
            {
                // if the actual element is not the element past the last - then end node
                if (this->actual_entry->next() != this->entrys.end().node())
                {
                    //*(this->actual_entry) = this->actual_entry->next();
                    ++(*this->actual_entry);
                }
                else
                {
                    *(this->actual_entry) = this->entrys.front();
                }
            }
            else
            {
                this->actual_entry = new DoublyListNodeIterator<String>(this->entrys.front());
            }

            this->_draw_widget();
            return;
        }
    }

    for (uint8_t i = 0; i < this->scroll_back_keys.length(); ++i)
    {
        if (this->scroll_back_keys[i] == _input_data)
        {
            if (actual_entry != nullptr)
            {
                // if the actual element is not the before first element - the begin node
                if (this->actual_entry->prev() != this->entrys.front().prev)
                {
                    --(*this->actual_entry);
                }
                else
                {
                    *(this->actual_entry) = this->entrys.last();
                }
            }
            else
            {
                this->actual_entry = new DoublyListNodeIterator<String>(this->entrys.last());
            }
            this->_draw_widget();
            return;
        }
    }
}

template <typename CALL_OBJECT_TYPE>
bool FGUI::SingleSelectionMenu<CALL_OBJECT_TYPE>::set_selection(const char *_selection)
{
    DoublyListNodeIterator<String> iterator(this->entrys.front());
    while (iterator != this->entrys.end())
    {
        if (strcmp(iterator.data().c_str(), _selection) == 0)
        {
            if (this->actual_entry == nullptr)
                this->actual_entry = new DoublyListNodeIterator<String>(iterator.node());
            else
                *(this->actual_entry) = iterator;
            return true;
        }
        ++iterator;
    }
    return false;
}

template <typename CALL_OBJECT_TYPE>
bool FGUI::SingleSelectionMenu<CALL_OBJECT_TYPE>::set_selection(String &_selection)
{
    return this->set_selection(_selection.c_str());
}

template <typename CALL_OBJECT_TYPE>
const String &FGUI::SingleSelectionMenu<CALL_OBJECT_TYPE>::get_selection() const
{
    return this->actual_entry->data();
}

template <typename CALL_OBJECT_TYPE>
void FGUI::SingleSelectionMenu<CALL_OBJECT_TYPE>::_draw_content(Widget::w_status _st)
{
    unsigned int *background_color = nullptr,
                 *text_color = nullptr;

    switch (_st)
    {
    case Widget::w_status::S_DISABLED:
    {
        background_color = &this->disabled_background_color;
        text_color = &this->disabled_text_color;
        break;
    }
    case Widget::w_status::S_TOUCHED:
    {
        background_color = &this->touched_background_color;
        text_color = &this->touched_text_color;
        break;
    }
    case Widget::w_status::S_RELEASED:
    {
        background_color = &this->released_background_color;
        text_color = &this->released_text_color;
        break;
    }
    default:
    {
        background_color = &this->released_background_color;
        text_color = &this->released_text_color;
        break;
    }
    }

    if (this->actual_entry != nullptr)
    {
        this->display->setFont(this->_text_font);
        uint8_t font_height = this->display->getFontYsize();
        uint8_t font_width = this->display->getFontXsize();

        uint16_t text_pixel_length = this->actual_entry->data().length() * font_width;

        uint16_t absolute_text_starting_x_pos = this->pos().x_pos +
                                                this->width() / 2 - text_pixel_length / 2;
        uint16_t absolute_text_starting_y_pos = this->pos().y_pos +
                                                this->height() / 2 - font_height / 2; // the upper begin_pos of the text

        this->display->setBackColor(*background_color);
        this->display->setColor(*text_color);
        this->display->print(this->actual_entry->data(), absolute_text_starting_x_pos,
                             absolute_text_starting_y_pos);
    }
}

template <typename CALL_OBJECT_TYPE>
void FGUI::SingleSelectionMenu<CALL_OBJECT_TYPE>::_draw_widget()
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
        else if (this->is_focused() || this->is_touched())
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