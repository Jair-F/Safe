#pragma once
#include "FGUI/FGUI.hpp"
#include "RFID/RFID.hpp"

class RFID_settings_window : public FGUI::Window
{
public:
    RFID_settings_window(FGUI::WindowBase *_parent_window);

    virtual ~RFID_settings_window() {}

    void loop() override;

protected:
    void _exit_window(FGUI::Touch_Widget<RFID_settings_window> *_widget);
    void _show_add(FGUI::Touch_Widget<RFID_settings_window> *_widget);
    void _show_delete_by_id(FGUI::Touch_Widget<RFID_settings_window> *_widget);
    void _show_delete_by_scan(FGUI::Touch_Widget<RFID_settings_window> *_widget);
    void _show_clear_database(FGUI::Touch_Widget<RFID_settings_window> *_widget);

    /**
     * handler for the confirm button depending on the actual window status
     */
    void _handle_confirm(FGUI::Touch_Widget<RFID_settings_window> *_widget);
    void _handle_reset(FGUI::Touch_Widget<RFID_settings_window> *_widget);
    /**
     * handle the delete button
     */
    void _handle_delete(FGUI::Touch_Widget<RFID_settings_window> *_widget);

    /**
     * prints info about the id while typing into the input_field
     */
    void _print_id_info(FGUI::Touch_Widget<RFID_settings_window> *_widget, const char *_input_buffer);

    void _redraw_chagned_window();

    /**
     * status of the window to know which widgets to show and hide
     */
    enum class window_status
    {
        RFID_add,
        RFID_delete_by_id,
        RFID_delete_by_scan,
        RFID_clear_database,

        // waiting for touching second time the delete button to confirm
        RFID_clear_database_waiting_confirm
    };

private:
    FGUI::TextLabel window_title;
    FGUI::BitmapButton<RFID_settings_window> back_btn;

    FGUI::BitmapButton<RFID_settings_window> add_btn;
    FGUI::BitmapButton<RFID_settings_window> delete_by_id_btn;
    FGUI::BitmapButton<RFID_settings_window> delete_by_scan_btn;
    FGUI::BitmapButton<RFID_settings_window> clear_database_btn;

    FGUI::Divider div_v;
    FGUI::Divider div_h;

    FGUI::TextLabel tag_id_label;
    FGUI::InputField<2, RFID_settings_window> tag_id_input;
    FGUI::TextLabel status_label;
    FGUI::Button<RFID_settings_window> confirm_btn; // for confirming the add/delete with scan
    FGUI::Button<RFID_settings_window> reset_btn;   // resets the scan result
    FGUI::Button<RFID_settings_window> delete_btn;

    window_status w_status;

    RFID::UID tmp_uid; // uid for temporary use between the loop(scan) and the press of the delete button
};
