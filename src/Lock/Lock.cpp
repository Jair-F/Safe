#include "Lock/Lock.hpp"
#include "logging/Log.hpp"
#include "system_clock/system_clock.hpp"
#include "Config.hpp"
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
                                                                             unauthorized_unlock_try_counter(0), relock_timer(_lock_timer),
                                                                             unlock_time_point(), locked_until_time_point(nullptr)
{
    // this->state = lock_state::UNLOCKED; // overwrite to trigger a real lock...
    // this->_lock();
}

bool Lock::begin()
{
    if (!Config::locked_until_tm_pt_stored()) // if system_clock lost power the data we will read isnt valid - skipp
    {

        this->locked_until_time_point = new Clock::time_point;
        if (!Config::read_locked_until_tm_pt(this->locked_until_time_point))
        {
            // on error delete straight the locked_until_time_point
            delete this->locked_until_time_point;
            this->locked_until_time_point = nullptr;
        }
        else
        {
            logger.log("LOCK: SUCCESSFUL read locked_until_time_point from system_clock_eeprom", Log::log_level::L_INFO);
            Serial.print("read locked until time_point: ");
            Serial.println(this->locked_until_time_point->to_string());

            if (*(this->locked_until_time_point) > system_clock.now())
                this->unlocking_allowed = false;
        }
    }
    else
    {
        logger.log(F("LOCK: RTC-Module lost power or no locked_until_time_point saved in system_clock"), Log::log_level::L_WARNING);
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

            if (!Config::store_locked_until_tm_pt(*locked_until_time_point))
            {
                logger.log(F("LOCK: Failed to write locked_until_time_point to the system_clock"), Log::log_level::L_ERROR);
            }
            else
            {
                logger.log(F("LOCK: SUCESSFULLY wrote locked_until_time_piont to system_clock"), Log::log_level::L_INFO);
            }
            logger.serial_dump();

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
            Serial.print("Time now: ");
            Serial.println(system_clock.now().to_string());
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
        Clock::time_point lock_time_point(this->unlock_time_point); // when this timepoint is passed the lock has to be locked (lock_time_point+relock_timer)
        lock_time_point += this->relock_timer;                      // addition is in seconds
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
            Config::clear_locked_until_tm_pt(); // delete the locked until time point in the system clock memory
            delete locked_until_time_point;
            locked_until_time_point = nullptr;
        }
    }

    // iterate over the unlock_objects
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
