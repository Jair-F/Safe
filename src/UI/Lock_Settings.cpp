#include "UI/Lock_Settings.hpp"
#include "Config.hpp"

extern const unsigned short back_33_33[];

extern Fingerprint::Fingerprint *fingerprint;
extern RFID::RFID *rfid;
extern Pin pin;
extern Lock lock;

extern StaticJsonDocument<1024> config;

Lock_Setings::Lock_Setings(FGUI::WindowBase *_parent_window) : Window(_parent_window),
                                                               window_title(this, {33, 0}, 287, "Lock", arial_bold, 0, 0),
                                                               back_btn(this, {0, 0}, 33, 33, this, back_33_33, (uint8_t)0, (uint8_t)0),
                                                               div_h(this, {0, 34}, 319, 3, FGUI::Divider::d_alignment::AL_HORIZONTAL, VGA_BLUE),
                                                               div_v(this, {159, 45}, 144, 3, FGUI::Divider::d_alignment::AL_VERTICAL, VGA_WHITE),
                                                               allowed_unlocks_tries_label(this, {168, 50}, 95, "unlock\ntries:", SmallFont, 0, 0),
                                                               locking_time_label(this, {168, 100}, 95, "locking\nperiod(s):", SmallFont, 0, 0),
                                                               relocking_timer_label(this, {168, 153}, 95, "relock\ntimer(s):", SmallFont, 0, 0),
                                                               RFID_label(this, {6, 64}, 117, "RFID:", SmallFont, 0, 0),
                                                               FINGERPRINT_label(this, {6, 109}, 117, "FINGERPRINT:", SmallFont, 0, 0),
                                                               PIN_label(this, {6, 151}, 117, "PIN:", SmallFont, 0, 0),
                                                               allowed_unlocks_tries_input(this, {263, 56}, 50, 27, this, FGUI::InputField<(uint8_t)2U, Lock_Setings>::IN_INPUT_TYPE::IN_TEXT, 2),
                                                               locking_period_input(this, {263, 107}, 50, 27, this, FGUI::InputField<(uint8_t)5U, Lock_Setings>::IN_INPUT_TYPE::IN_TEXT, 2),
                                                               relocking_timer_input(this, {263, 158}, 50, 27, this, FGUI::InputField<(uint8_t)2U, Lock_Setings>::IN_INPUT_TYPE::IN_TEXT, 2),
                                                               RFID_chbox(this, {123, 59}, 25, this, 2),
                                                               FINGERPRINT_chbox(this, {123, 102}, 25, this, 2),
                                                               PIN_chbox(this, {123, 145}, 25, this, 2),
                                                               save_btn(this, {110, 201}, 100, 28, this, 2, 2)
{
    this->window_title.set_text_alignment(FGUI::TextLabel::text_alignment::AL_CENTER);

    this->back_btn.on_release = &Lock_Setings::_exit_window;

    this->RFID_chbox.touched_background_color = VGA_BLACK;
    this->FINGERPRINT_chbox.touched_background_color = VGA_BLACK;
    this->PIN_chbox.touched_background_color = VGA_BLACK;

    this->save_btn.setText("save");
    this->save_btn.on_release = &Lock_Setings::_handle_save;
}

void Lock_Setings::_exit_window(FGUI::Touch_Widget<Lock_Setings> *_widget)
{
    this->_get_main_window()->set_active_window(this->_get_parent_window());
}

void Lock_Setings::_pre_show()
{
    this->RFID_chbox.set_checked(rfid->is_enabled());
    this->FINGERPRINT_chbox.set_checked(fingerprint->is_enabled());
    this->PIN_chbox.set_checked(pin.is_enabled());

    this->allowed_unlocks_tries_input.set_input_buffer(String(lock.get_allowed_unauthorized_unlock_tries()));
    this->locking_period_input.set_input_buffer(String(lock.get_locking_period()));
    this->relocking_timer_input.set_input_buffer(String(lock.get_relock_timer()));

    this->_on_type_input_check(&this->allowed_unlocks_tries_input, this->allowed_unlocks_tries_input.get_input_buffer());
}

void Lock_Setings::_handle_save(FGUI::Touch_Widget<Lock_Setings> *_widget)
{
    if (this->RFID_chbox.is_checked())
        rfid->enable();
    else
        rfid->disable();

    if (this->FINGERPRINT_chbox.is_checked())
        fingerprint->enable();
    else
        fingerprint->disable();

    if (this->PIN_chbox.is_checked())
        pin.enable();
    else
        pin.disable();

    lock.set_allowed_unauthorized_unlock_tries(String(this->allowed_unlocks_tries_input.get_input_buffer()).toInt());
    lock.set_locking_period(String(this->locking_period_input.get_input_buffer()).toInt());
    lock.set_lock_timer(String(this->relocking_timer_input.get_input_buffer()).toInt());
}

void Lock_Setings::_on_type_input_check(FGUI::Touch_Widget<Lock_Setings> *_widget, const char *_input_buffer)
{
    if (strlen(this->allowed_unlocks_tries_input.get_input_buffer()) > 0 &&
        strlen(this->locking_period_input.get_input_buffer()) > 0 &&
        strlen(this->relocking_timer_input.get_input_buffer()) > 0)
        this->save_btn.enable();
    else
        this->save_btn.disable();
}
