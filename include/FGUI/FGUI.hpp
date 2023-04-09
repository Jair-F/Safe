#pragma once

#define fontdatatype uint8_t

#include "lib/position.hpp"
#include "lib/PopUp_Window.hpp"
#include "lib/MainWindow.hpp"
#include "lib/Window.hpp"

#include "lib/Widget.hpp"
#include "lib/Button.hpp"
#include "lib/CloseButtons.hpp"
#include "lib/BitMapButton.hpp"
#include "lib/Bitmap.hpp"
#include "lib/TextLabel.hpp"
#include "lib/CheckBox.hpp"
#include "lib/InputField.hpp"
#include "lib/Divider.hpp"
#include "lib/ProgressBar.hpp"
#include "lib/Gif.hpp"
#include "lib/SingleSelectionMenu.hpp"

// fonts
extern fontdatatype SmallFont[];
extern fontdatatype BigFont[];
extern fontdatatype arial_bold[];
extern fontdatatype arial_normal[];
extern fontdatatype mykefont2[];
extern fontdatatype Retro8x16[];
extern fontdatatype Sinclair_S[];
extern fontdatatype TinyFont[];
extern fontdatatype various_symbols[];

// the index documentation page for the doxygen documentation!!

/**
 * \defgroup FGUI FGUI
 */

/**
 * \mainpage FGUI Documentation
 *
 * Description of the FGUI library
 *
 * list of classes in the library
 *
 * - \subpage MainWindow
 * - \subpage WindowBase
 * - \subpage Window
 * - \subpage PopUpWindow
 * - \subpage Widget
 * - \subpage TouchWidget
 * - \subpage InputField
 * - \subpage CheckBox
 * - \subpage TextLabel
 * - \subpage SingleSelectionMenu
 * - \subpage ProgressBar
 * - \subpage Position
 * - \subpage Giff
 * - \subpage Divider
 * - \subpage CloseButton
 * - \subpage ButtonBase
 * - \subpage Button
 * - \subpage BitmapButton
 * - \subpage Bitmap
 * - \subpage DisplayGeometryFunctions
 */