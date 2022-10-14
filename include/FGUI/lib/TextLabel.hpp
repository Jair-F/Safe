#pragma once

#include "Widget.hpp"

extern uint8_t SmallFont[];
extern uint8_t BigFont[];

namespace FGUI
{
    /**
     * \defgroup TextLabel
     * \ingroup FGUI
     *
     * @brief A widget for displaying easily basic formatted text on the display.
     *
     * @details This widget supports manual newlines in the text with special character('\\n') - but only with this one special
     * character. Also the text can be centered, be left bound or right bound in the widget content area.
     *
     * @{
     */
    class TextLabel : public Widget
    {
    public:
        /**
         * @brief type for defining the alignment of the text.
         */
        enum text_alignment
        {
            AL_CENTER,
            AL_LEFT,
            AL_RIGHT
        };

        /**
         * @param _parent parent window to which the widget will register to.
         * @param _upper_left_pos upper left corner in relation to the parent window zero point.
         * @param _width width of the text label
         * @param _text text to display
         * @param _font pointer to font array for the text.
         *
         * @note the widget adjusts its height atomatically based on the text and the newlines in the text.
         * The height can be read with Widget::height().
         */
        TextLabel(WindowBase *_parent, const position _upper_left, uint16_t _width, String _text = "", uint8_t *_font = SmallFont);
        virtual ~TextLabel() {}

        /**
         * @param _font pointer to font array for the text.
         */
        void set_font(uint8_t *_font);
        /**
         * @return pointer to font array for the text.
         */
        uint8_t *get_font() const { return this->text_font; };

        /**
         * @note '\\n' are interpreted as manual defined newlines!! but other special characters like tabs(\t)
         * are only displayed like they are('\\t').
         * @param _text the text to be displayed.
         */
        void set_text(String _text);

        /**
         * @param _text_alignment enum value of TextLabel::text_alignment to define the alignment of the
         * text(CENTERED, LEFT, RIGHT).
         */
        void set_text_alignment(text_alignment _text_alignment) { this->text_align = _text_alignment; }
        /**
         * @return the alignment of the text(CENTERED, LEFT, RIGHT).
         */
        inline text_alignment get_text_alignment() const { return this->text_align; }

        /**
         * @brief the color values are RGB-565 values(16-bit value).
         * RGB-565 color picker: https://chrishewett.com/blog/true-rgb565-colour-picker/
         */
        unsigned int released_text_color = VGA_WHITE;

    protected:
        void _draw_widget() override;

        // those two are doing the same - call _draw_widget
        void _draw_content(Widget::w_status _st) override { this->_draw_widget(); }

        /**
         * @brief function which calculates the height of the widget according to the text and font and some other
         * values which are essential for printing the text and store these values
         */
        void _calc_widget();

    private:
        String text;
        uint8_t *text_font = SmallFont;
        text_alignment text_align; // text_alignment

        /**
         * @brief gap between lines each printed line of text in pixels
         */
        uint8_t text_gap_height;
        /**
         * @brief gap between the begin of the widget and begin of the text and end of the widget and end of the text in pixels
         */
        uint8_t text_gap_length;
        /**
         * @brief max number of characters which can be printed in one line according to the widget widht and text font.
         */
        uint8_t max_chars_in_line;
        /**
         * @brief number of text lines the actual text string will need to print the text with the set text font.
         */
        uint8_t text_lines;
    };

    /** @} */

} // namespace FGUI
