#include "UI/Status_Info.hpp"

extern const unsigned short back_icon[];
extern const unsigned short back_33_33[];

Status_Info_window::Status_Info_window(FGUI::WindowBase *_parent_window) : FGUI::Window(_parent_window),
                                                                           window_title(this, {33, 0}, 286, F("Status Info"), arial_bold, 0, 0),
                                                                           back_btn(this, {0, 0}, 33, 33, this, back_33_33, (uint8_t)0, (uint8_t)0),
                                                                           div(this, {0, 34}, 320, 3, FGUI::Divider::d_alignment::AL_HORIZONTAL, VGA_BLUE),
                                                                           info_label(this, {0, 36}, 319, 202, F(""), SmallFont, 0, 0)
{
    this->back_btn.on_release = &Status_Info_window::_exit_window;

    this->window_title.set_text_alignment(FGUI::TextLabel::text_alignment::AL_CENTER);
    this->window_title.released_background_color = this->get_background_color();

    this->info_label.set_text_alignment(FGUI::TextLabel::text_alignment::AL_LEFT);
}

void Status_Info_window::_exit_window(FGUI::Touch_Widget<Status_Info_window> *_widget)
{
    this->_get_main_window()->set_active_window(this->_get_parent_window());
}

void Status_Info_window::_pre_show()
{
    Serial.println(F("begin pre_show"));

    auto log_list = logger.log_list();

    String log_dump = F("");

    auto ll_iterator = log_list.begin();
    while (ll_iterator != log_list.end())
    {
        String log_level = F("");
        switch (ll_iterator.data().Level())
        {
        case Log::log_level::L_CRITICAL:
        {
            log_level = F("CRITICAL");
            break;
        }
        case Log::log_level::L_DEBUG:
        {
            log_level = F("DEBUG");
            break;
        }
        case Log::log_level::L_ERROR:
        {
            log_level = F("ERROR");
            break;
        }
        case Log::log_level::L_INFO:
        {
            log_level = F("INFO");
            break;
        }
        case Log::log_level::L_WARNING:
        {
            log_level = F("WARNING");
            break;
        }
        default:
        {
            log_level = F("");
            break;
        }
        }

        log_dump += F("[");
        log_dump += ll_iterator.data().time_point().to_string_time() + F("] ") + log_level + F(": ") + ll_iterator.data().message() + F("\n");
        ++ll_iterator;
    }

    Serial.print("pre show text: ");
    Serial.println(log_dump);
    this->info_label.set_text(log_dump);
}
