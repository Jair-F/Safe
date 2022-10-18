#include "Lock.hpp"
#include "logging/Log.hpp"
#include "system_clock/system_clock.hpp"
#include <uEEPROMLib.h>

extern Log::Log logger;
extern Clock::Clock system_clock;
extern uEEPROMLib system_clock_eeprom;

/*
    indicator address in the system clock eeprom memeory which indicates if
    a locked until time pint is stored in the memory.
*/
#define LOCKED_UNTIL_TIME_POINT_STORED SYSTEM_CLOCK_EEPROM_LAST_ADRESS

Lock::Lock::Lock(const unsigned short _lock_timer, lock_state _lock_state) : unlock_object_ids(), unlock_objects(),
                                                                             state(_lock_state), unlocking_allowed(true),
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
            // reading the last memory-sector of the RTC which is the indicator if a lock-time-point is stored in the memory
            if (static_cast<bool>(system_clock_eeprom.eeprom_read(LOCKED_UNTIL_TIME_POINT_STORED)))
            {
                this->locked_until_time_point = new Clock::time_point;
                if (!system_clock_eeprom.eeprom_read(0, reinterpret_cast<byte *>(locked_until_time_point), sizeof(Clock::time_point)))
                {
                    // error while reading...

                    delete this->locked_until_time_point;
                    this->locked_until_time_point = nullptr;
                    // print error while reading the the locked_until_time_point
                }
                else
                {
                    // success at reading...

                    // this->locked_until_time_point = reinterpret_cast<Clock::time_point *>(buffer);
                    if ((*locked_until_time_point) > system_clock.now()) // if the locked time hasnt passed yet
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
            }
            else
            {
                Serial.println(F("no locked_until_time_point saved in system_clock"));
            }
        }
    }
    else
    {
        logger.log(F("LOCK: RTC-Module lost power - locked_until_time_point will not be read - replace battary"), Log::log_level::L_WARNING);
        /*
            writing to the clock module that there is no locked_unitl time_point - could be that now the random data
            is true and if we change the battary it could lock the lock to a random time_point - so overwrite it!
        */
        uint8_t buffer = static_cast<uint8_t>(false);
        system_clock_eeprom.eeprom_write(SYSTEM_CLOCK_EEPROM_LAST_ADRESS, buffer);
        this->unlocking_allowed = true;
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
            *locked_until_time_point += Clock::duration(locking_period); // adding in seconds

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
        if (this->locked_until_time_point != nullptr)
        {
            Serial.print(F("unlocking is forbidden until "));
            Serial.println(this->locked_until_time_point->to_string());
        }
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

    // delete the flag that a locked until time point is stored
    if (!system_clock_eeprom.eeprom_write(LOCKED_UNTIL_TIME_POINT_STORED, static_cast<uint8_t>(true)))
        return false; // if writing failed

    if (system_clock_eeprom.eeprom_write(0, reinterpret_cast<uint8_t *>(locked_until_time_point), sizeof(Clock::time_point)))
        return false; // if writing failed

    return true; // writing was successful
}
bool Lock::_delete_locked_until_time_point()
{
    // returns true if wirting was successful

    // delete the flag that a locked until time point is stored
    return system_clock_eeprom.eeprom_write(LOCKED_UNTIL_TIME_POINT_STORED, static_cast<uint8_t>(false));
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
        if (now > *locked_until_time_point) // locked until time point passed - reset the locked until time point
        {
            DEBUG_PRINTLN(F("unlocking is now allowed"))
            this->unlocking_allowed = true;
            _delete_locked_until_time_point(); // delete the locked until time point in the system clock memory
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
