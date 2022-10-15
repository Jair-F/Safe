#include "Lock.hpp"
#include "logging/Log.hpp"
#include "system_clock/system_clock.hpp"
#include <uEEPROMLib.h>

extern Log::Log logger;
extern Clock::Clock system_clock;
extern uEEPROMLib system_clock_eeprom;

Lock::Lock::Lock(const unsigned short _lock_timer, lock_state _lock_state, bool _allow_unlocking) : unlock_object_ids(), unlock_objects(),
                                                                                                    state(_lock_state), unlocking_allowed(_allow_unlocking),
                                                                                                    unauthorized_unlock_try_counter(0), lock_timer(_lock_timer),
                                                                                                    unlock_time_point(), locked_until_time_point(nullptr)
{
    // this->state = lock_state::UNLOCKED; // overwrite to trigger a real lock...
    // this->_lock();
}

bool Lock::begin()
{
    if (!system_clock.lost_power()) // if system_clock lost power the data we will read isnt valid - skipp
    {
        if (!static_cast<bool>(system_clock_eeprom.eeprom_read(0)))
        {
            // error with eeprom reading - abort the read of the locked_until time point
        }
        else
        {
            // reading the last memory-sector of the RTC to look up if a lock-time-point is stored in the memory
            if (static_cast<bool>(system_clock_eeprom.eeprom_read(SYSTEM_CLOCK_EEPROM_LAST_ADRESS)))
            {
                uint16_t time_point_size = sizeof(Clock::time_point);
                byte *buffer = new byte[time_point_size]; // storage for the locked_until_time_point
                if (!system_clock_eeprom.eeprom_read(0, buffer, time_point_size))
                {
                    // print error while reading the the locked_until_time_point
                }
                else
                {
                    // if (this->locked_until_time_point == nullptr)
                    //     this->locked_until_time_point = new Clock::time_point();
                    this->locked_until_time_point = reinterpret_cast<Clock::time_point *>(buffer);
                    auto now = system_clock.now();
                    if ((*locked_until_time_point) > now) // if the time hasnt passed yet
                    {
                        this->unlocking_allowed = false;
                        this->_lock();
                    }
                    else
                    {
                        // unlocking is allowed
                        this->unlocking_allowed = true;
                    }
                }

                // delete[] buffer;
            }
            else
            {
                Serial.println(F("no locked_until_time_point saved in system_clock"));
            }
        }

        /*
        // reading the last memory-sector of the RTC to look up if a lock-time-point is stored in the memory
        if (static_cast<bool>(system_clock.GetMemory(SYSTEM_CLOCK_MEMORY_LENGTH)))
        {
            uint8_t *buffer = new uint8_t[sizeof(RtcDateTime)];
            // we save the timepoint from pos 0
            uint8_t num_read_bytes = system_clock.GetMemory(buffer, sizeof(RtcDateTime));
            if (num_read_bytes < sizeof(RtcDateTime))
            {
                logger.log(F("LOCK: Failed to read locked_until_timepoint from the system_clock"), Log::log_level::L_WARNING);
            }
            locked_until_time_point = reinterpret_cast<RtcDateTime *>(buffer); // RtcDateTime is exactly 6*sizeof(uint8_t)=48 bits
            RtcDateTime now = system_clock.GetDateTime();

            if ((*locked_until_time_point) > now)
            {
                this->unlocking_allowed = false;
                _lock();
            }
            else
            {
                DEBUG_PRINT(F("unlocking is allowed"))
            }
            DEBUG_PRINT(F("read locked_until_time_point"))
        }
        else
        {
            DEBUG_PRINT(F("no locked_until_time_point saved in system_clock"))
        }
        */
    }
    else
    {
        logger.log(F("LOCK: RTC-Module lost power - locked_until_time_point will not be read - replace battary"), Log::log_level::L_WARNING);
        /*
            writing to the clock module that there is no locked_unitl time_point - could be that now the random data
            is true and if we change the battary it could lock the lock to a random time_point
        */
        uint8_t buffer = static_cast<uint8_t>(false);
        system_clock_eeprom.eeprom_write(SYSTEM_CLOCK_EEPROM_LAST_ADRESS, buffer);
    }

    if (!this->unlocking_allowed)
    {
        Serial.print(F("unlocking is forbidden until "));
        Serial.println(this->locked_until_time_point->to_string());
    }
}

uint16_t Lock::_register_unlock_object(Unlock_Object *_unlock_object)
{
    uint16_t unlock_object_id = this->unlock_objects.size(); // please make better ids!!!!
    this->unlock_object_ids.push_back(unlock_object_id);
    this->unlock_objects.push_back(_unlock_object);
    return unlock_object_id;
}

Lock::~Lock()
{
    if (this->locked_until_time_point != nullptr)
    {
        delete this->locked_until_time_point;
        this->locked_until_time_point = nullptr;
    }
}

void Lock::Lock::report_unauthorized_unlock_try()
{
    if (this->unlocking_allowed)
    {
        DEBUG_PRINTLN(F("Unauthorized unlock_object..."))
        // count the unauthorized unlock tries and after a defined amount of tries lock the lock for a specific time
        ++this->unauthorized_unlock_try_counter;
        if (this->unauthorized_unlock_try_counter >= allowed_unauthorized_unlock_tries)
        {
            String msg = F("Locking the lock for ");
            msg += locking_period;
            msg += F(" seconds due to to often tried to unlock");
            DEBUG_PRINTLN(msg)
            if (locked_until_time_point == nullptr)
            {
                locked_until_time_point = new Clock::time_point();
            }
            *locked_until_time_point = system_clock.now();
            *locked_until_time_point += locking_period;

            if (!_save_locked_until_time_point())
            {
                logger.log(F("LOCK: Failed to write locked_until_time_point to the system_clock"), Log::log_level::L_ERROR);
            }

            this->unlocking_allowed = false;
            _lock();
            this->unauthorized_unlock_try_counter = 0;

            Serial.print("time point now: ");
            Serial.println(system_clock.now().to_string());
            Serial.print(F("unlocking is forbidden until "));
            Serial.println(this->locked_until_time_point->to_string());
        }
    }
    else
    {
        Serial.print(F("unlocking is forbidden until "));
        Serial.println(this->locked_until_time_point->to_string());
        /*
        msg += this->locked_until_time_point->Day();
        msg += '/';
        msg += this->locked_until_time_point->Month();
        msg += '/';
        msg += this->locked_until_time_point->Year();
        msg += ' ';
        msg += this->locked_until_time_point->Hour();
        msg += ':';
        msg += this->locked_until_time_point->Minute();
        msg += ':';
        msg += this->locked_until_time_point->Second();
        DEBUG_PRINTLN(msg)
        */
    }
}

void Lock::_lock()
{
    if (this->state != lock_state::LOCKED)
    {
        if (this->on_locking != nullptr)
        {
            bool success = on_locking(); // calling the "switch_state" function which will lock the physical lock
            if (!success)
            {
                logger.log(F("LOCK: Error locking the physical Lock"), Log::log_level::L_ERROR);
            }
        }
        this->state = lock_state::LOCKED;
    }
}
void Lock::_unlock()
{
    if (this->state != lock_state::UNLOCKED)
    {
        if (this->on_unlocking != nullptr)
        {
            bool success = on_unlocking(); // calling the "switch_state" function which will unlock the physical lock
            if (!success)
            {
                logger.log(F("LOCK: Error unlocking the physical Lock"), Log::log_level::L_ERROR);
            }
        }
        this->unlock_time_point = system_clock.now();
        this->state = lock_state::UNLOCKED;
    }
}

bool Lock::_save_locked_until_time_point()
{
    if (locked_until_time_point == nullptr)
        return false;

    if (!system_clock_eeprom.eeprom_write(SYSTEM_CLOCK_EEPROM_LAST_ADRESS, static_cast<uint8_t>(true)))
        return false; // if writing failed
    if (system_clock_eeprom.eeprom_write(0, reinterpret_cast<uint8_t *>(locked_until_time_point), sizeof(Clock::time_point)))
        return false; // if writing failed

    /*
    system_clock.SetMemory(SYSTEM_CLOCK_MEMORY_LENGTH, static_cast<uint8_t>(true)); // setting the flag that we have saved a time_point in the memory
    uint8_t num_written_bytes = system_clock.SetMemory(reinterpret_cast<uint8_t *>(locked_until_time_point), sizeof(RtcDateTime));
    if (num_written_bytes < sizeof(RtcDateTime))
    {
        return false;
    }
    */
    return true; // writing was successful
}
bool Lock::_delete_locked_until_time_point()
{
    // returns true if wirting was successful
    return system_clock_eeprom.eeprom_write(SYSTEM_CLOCK_EEPROM_LAST_ADRESS, static_cast<uint8_t>(false));

    // system_clock.SetMemory(SYSTEM_CLOCK_MEMORY_LENGTH, static_cast<uint8_t>(false));
    // return true;
}

bool Lock::request_unlock()
{
    if (this->unlocking_allowed)
    {
        this->unauthorized_unlock_try_counter = 0; // reset the unauthorized_unlock_tries
        this->_unlock();
        return true;
    }
    else
    {
        DEBUG_PRINT(F("unloking currently not allowed - disabled"))
    }
    return false;
}

void Lock::force_unlock()
{
    this->_unlock();
}

void Lock::lock()
{
    _lock();
}

enum lock_state Lock::get_state()
{
    return this->state;
}

void Lock::allow_unlocking()
{
    if (this->locked_until_time_point != nullptr)
    {
        delete this->locked_until_time_point;
        this->locked_until_time_point = nullptr;
    }
    this->unlocking_allowed = true;
}

void Lock::loop()
{
    Clock::time_point now = system_clock.now();
    if (state == lock_state::UNLOCKED)
    {
        Clock::time_point lock_time_point(this->unlock_time_point); // when this timepoint is passed the lock has to be locked (lock_time_point+lock_timer)
        lock_time_point += this->lock_timer;                        // addition is in seconds
        DEBUG_PRINT('.');

        if (now > lock_time_point)
        {
            this->_lock();
        }
    }

    // unlocking not allowed due to too many tries - locked_until_time_point is set
    if (this->locked_until_time_point != nullptr)
    {
        if (now > *locked_until_time_point)
        {
            DEBUG_PRINTLN(F("unlocking is now allowed"))
            this->unlocking_allowed = true;
            _delete_locked_until_time_point();
            delete locked_until_time_point;
            locked_until_time_point = nullptr;
        }
    }
    else // unlocking allowed
    {
        SinglyListNodeIterator<Unlock_Object *> iterator = this->unlock_objects.begin();
        while (iterator != this->unlock_objects.end())
        {
            if (iterator.data()->is_enabled())
            {
                // DEBUG_PRINT(F("Reading Unlock Object with ID: "))
                // DEBUG_PRINTLN(iterator.data()->get_unlock_object_id())

                Unlock_Object::unlock_authentication_reports unlock_object_report = iterator.data()->read();
                switch (unlock_object_report)
                {
                case Unlock_Object::unlock_authentication_reports::AUTHORIZED_UNLOCK_OBJECT:
                {
                    this->request_unlock();
                    break;
                }
                case Unlock_Object::unlock_authentication_reports::UNAUTHORIZED_UNLOCK_OBJECT:
                {
                    this->report_unauthorized_unlock_try();
                    break;
                }
                case Unlock_Object::unlock_authentication_reports::UNLOCK_OBJECT_DISABLED:
                {
                    // DEBUG_PRINTLN()
                    // logger.log("Unlock_Object is disabled", Log::log_level::L_DEBUG);
                    break;
                }
                case Unlock_Object::unlock_authentication_reports::UNLOCK_OBJECT_READ_ERROR:
                {
                    DEBUG_PRINTLN("Error while reading Unlock_Object!!!!")
                    // logger.log("Error while reading Unlock_Object!!!!", Log::log_level::L_WARNING);
                    break;
                }
                case Unlock_Object::unlock_authentication_reports::NO_UNLOCK_OBJECT_PRESENT:
                {
                    // DEBUG_PRINTLN("No Unlock_Object available")
                    //  logger.log("No Unlock_Object available", Log::log_level::L_DEBUG);
                    break;
                }
                default:
                    break;
                }
            }
            ++iterator;
        }
    }
}
