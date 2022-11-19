#pragma once
#include "FGUI/FGUI.hpp"

class RFID_delete_tag_by_id : public FGUI::Window
{
public:
    RFID_delete_tag_by_id(FGUI::WindowBase *_parent_window);

protected:
    void _remove_tag_by_id(FGUI::Touch_Widget<RFID_delete_tag_by_id> *_widget);
    void _on_type_tag_id_check(FGUI::Touch_Widget<RFID_delete_tag_by_id> *_widget, const char *_input_buffer);

    /**
     * @return true if the id is valid
     */
    bool _check_tag_id(const char *_id);

    void _exit_window(FGUI::Touch_Widget<RFID_delete_tag_by_id> *_widget);

private:
    FGUI::TextLabel window_title;
    FGUI::TextLabel tag_id_label;
    FGUI::InputField<2, RFID_delete_tag_by_id> tag_id_to_remove;
    FGUI::TextLabel status_label;
    FGUI::Button<RFID_delete_tag_by_id> delete_btn;
    FGUI::Button<RFID_delete_tag_by_id> exit_btn;
};
