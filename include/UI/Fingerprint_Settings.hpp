#pragma once
#include "FGUI/FGUI.hpp"
#include "Fingerprint.hpp"

class Fingerprint_Settings : public FGUI::Window
{
public:
    Fingerprint_Settings(FGUI::WindowBase *_parent_window);

    virtual ~Fingerprint_Settings() {}

    void loop() override;

protected:
    void _exit_window(FGUI::Touch_Widget<Fingerprint_Settings> *_widget);

    // funcs for adjusting the window UI
    void _show_add(FGUI::Touch_Widget<Fingerprint_Settings> *_widget);
    void _show_delete_by_id(FGUI::Touch_Widget<Fingerprint_Settings> *_widget);
    void _show_delete_by_scan(FGUI::Touch_Widget<Fingerprint_Settings> *_widget);
    void _show_clear_database(FGUI::Touch_Widget<Fingerprint_Settings> *_widget);

    /**
     * handler for the confirm button depending on the actual window status
     */
    void _handle_confirm(FGUI::Touch_Widget<Fingerprint_Settings> *_widget);
    /**
     * handle the reset button
     */
    void _handle_reset(FGUI::Touch_Widget<Fingerprint_Settings> *_widget);
    /**
     * handle the delete button
     */
    void _handle_delete(FGUI::Touch_Widget<Fingerprint_Settings> *_widget);

    /**
     * prints info about the id while typing into the input_field
     */
    void _print_id_info(FGUI::Touch_Widget<Fingerprint_Settings> *_widget, const char *_input_buffer);

    void _pre_show() override; // disables all buttons which needs interaction first

    bool _id_ok(uint8_t _id);

    void _redraw_chagned_window();

    enum class window_status
    {
        FINGER_add,
        FINGER_add_waiting_for_2, // waiting for the second confirm scan
        FINGER_delete_by_id,
        FINGER_delete_by_scan,
        FINGER_clear_database,

        // waiting for touching second time the delete button to confirm
        FINGER_clear_database_waiting_confirm
    };

private:
    FGUI::TextLabel window_title;
    FGUI::BitmapButton<Fingerprint_Settings> back_btn;

    FGUI::BitmapButton<Fingerprint_Settings> add_btn;
    FGUI::BitmapButton<Fingerprint_Settings> delete_by_id_btn;
    FGUI::BitmapButton<Fingerprint_Settings> delete_by_scan_btn;
    FGUI::BitmapButton<Fingerprint_Settings> clear_database_btn;

    FGUI::Divider div_v;
    FGUI::Divider div_h;

    FGUI::TextLabel id_label;
    FGUI::InputField<2, Fingerprint_Settings> id_input;
    FGUI::TextLabel status_label;
    FGUI::ProgressBar p_bar;
    FGUI::Button<Fingerprint_Settings> confirm_btn; // for confirming the add/delete with scan
    FGUI::Button<Fingerprint_Settings> reset_btn;   // resets the scan result
    FGUI::Button<Fingerprint_Settings> delete_btn;

    window_status w_status;
};