#pragma once
#include "FGUI/FGUI.hpp"

#include "Lock/Lock.hpp"
#include "RFID/RFID.hpp"
#include "Fingerprint.hpp"
#include "Pin.hpp"

class Lock_Setings : public FGUI::Window
{
public:
    Lock_Setings(FGUI::WindowBase *_parent_window);

    virtual ~Lock_Setings() {}

    /**
     * sets the widgets to the current values
     */
    void _pre_show() override;

protected:
    void _exit_window(FGUI::Touch_Widget<Lock_Setings> *_widget);

    /**
     * handles the apply button touch and sets the Pin
     */
    void _handle_save(FGUI::Touch_Widget<Lock_Setings> *_widget);

    /**
     * checks the input fields for valid input and updates the UI
     * matching to the input
     */
    void _on_type_input_check(FGUI::Touch_Widget<Lock_Setings> *_widget, const char *_input_buffer);

private:
    FGUI::TextLabel window_title;
    FGUI::BitmapButton<Lock_Setings> back_btn;
    FGUI::Divider div_h; // horizontal div under the window_title

    FGUI::Divider div_v; // vertical div between the window sections

    FGUI::TextLabel allowed_unlocks_tries_label;
    FGUI::TextLabel locking_time_label;
    FGUI::TextLabel relocking_timer_label;
    FGUI::TextLabel RFID_label;
    FGUI::TextLabel FINGERPRINT_label;
    FGUI::TextLabel PIN_label;

    FGUI::InputField<2, Lock_Setings> allowed_unlocks_tries_input;
    FGUI::InputField<5, Lock_Setings> locking_period_input;
    FGUI::InputField<2, Lock_Setings> relocking_timer_input;

    FGUI::CheckBox<Lock_Setings> RFID_chbox;
    FGUI::CheckBox<Lock_Setings> FINGERPRINT_chbox;
    FGUI::CheckBox<Lock_Setings> PIN_chbox;

    FGUI::Button<Lock_Setings> save_btn;
};
