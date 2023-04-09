#pragma once

#include "MainWindow.hpp"
#include "WindowBase.hpp"
#include "CloseButtons.hpp"

namespace FGUI
{
    class MainWindow;

    /**
     * \defgroup PopUpWindow
     * \ingroup FGUI
     *
     * @briefa pop up window which will be centerd in the display and is shown above another window or pop up window.
     *
     * @{
     */
    class PopUp_Window : public WindowBase
    {
    public:
        /**
         * @param _parent_window pointer to the parent window above which the pop up window is shown.
         * @param _width the width of the pop up window.
         * @param _height the height of the pop up window.
         * @param _border_weight size of the border in pixels.
         * @param _show_close_button true to show the close button
         * @param _close_button_size the size of the close button in pixels.
         *
         * @details the pop up window is centered in the parent window
         * @note !! the pop up window cant be larger than the parent window in the height and the width !!
         */
        PopUp_Window(WindowBase *_parent_window, uint16_t _width, uint16_t _height,
                     uint8_t _border_weight = 2, bool _show_close_button = true,
                     uint16_t _close_button_size = 22);
        virtual ~PopUp_Window();

        Widget *get_focused_widget() const override;
        bool request_focus(Widget *_widget) override;

        /**
         * @brief hides the close button.
         */
        void hide_close_button();
        /**
         * @brief shows the close button if hidden.
         */
        void show_close_button();
        /**
         * @return true if the close button is hidden.
         */
        bool is_close_button_hidden();

        UTFT *_get_display() const override;
        URTouch *_get_touch() const override;
        MainWindow *_get_main_window() const override;

        /**
         * @return the size of the close button
         * @details the close button is a square...
         */
        uint16_t _get_close_button_size() const { return this->close_button.width(); }

    protected:
        /**
         * @brief function to exit and hide the pop up window properly.
         *
         * @details if the X of the pop_up_window is touched this method will be executed to hide the pop_up_window properly
         * and adjust the parent window variables accordingly to hide the pop_up_window and set the parent window
         * as active window in the MainWindow.
         */
        void _exit_pop_up_window(Touch_Widget<PopUp_Window> *_widget);

    private:
        CloseButton<PopUp_Window> close_button;
    };

    /** @} */

} // namespace FGUI
