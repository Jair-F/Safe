#pragma once
#include <UTFT.h>
#include <URTouch.h>

#include "LinkedList.hpp"

#include "position.hpp"
#include "WindowBase.hpp"
#include "MainWindow.hpp"

#include "Widget.hpp"

namespace FGUI
{
    class MainWindow;
    class Widget;

    /**
     * \defgroup Window
     * \ingroup FGUI
     *
     * @brief class for making derived customized window classes to show on the MainWindow
     *
     * @{
     */
    class Window : public WindowBase
    {
    public:
        Window(MainWindow *main_window);
        virtual ~Window() {}

        /**
         * @param _widget request focus for _widget to get keyboard_input
         * @return true if the request was accepted else false
         */
        bool request_focus(Widget *_widget) override;
        Widget *get_focused_widget() const override;

        UTFT *_get_display() const override;
        URTouch *_get_touch() const override;
        MainWindow *_get_main_window() const override { return _main_window; }

    private:
        MainWindow *_main_window; ///< @brief pionter to the parent window.
    };

    /** @} */

} // namespace FGUI
