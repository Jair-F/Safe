#include "Lock.hpp"
#include "logging/Log.hpp"
#include "system_clock.hpp"

extern Log::Log logger;
extern Clock::Clock<ThreeWire> system_clock;

Lock::Lock::Lock(const unsigned short _lock_timer, lock_state _lock_state, bool _allow_unlocking) : state(_lock_state), unlocking_allowed(_allow_unlocking),
                                                                                                    unauthorized_unlock_try_counter(0), lock_timer(_lock_timer),
                                                                                                    unlock_time_point(0), unlock_object_ids(), unlock_objects()
{
    if (!system_clock.lost_power()) // if system_clock lost power the data we will read isnt valid - skipp
    {
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
    }
    else
    {
        logger.log(F("LOCK: RTC-Module lost power - locked_until_time_point will not be read - replace battary"), Log::log_level::L_WARNING);
        /*
            writing to the clock module that there is no locked_unitl time_point - could be that now the random data
            is true and if we change the battary it could lock the lock to a random time_point
        */
        uint8_t buffer = static_cast<uint8_t>(false);
        system_clock.SetMemory(&buffer, SYSTEM_CLOCK_MEMORY_LENGTH);
    }
    if (!this->unlocking_allowed)
    {
        String msg = F("unlocking is forbidden until ");
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
    }
}

uint16_t Lock::_register_unlock_object(Unlock_Object *_unlock_object)
{
    uint16_t unlock_object_id = this->unlock_objects.size();
    this->unlock_object_ids.push_back(unlock_object_id); // please make better ids!!!!
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
        DEBUG_PRINT(F("Unauthorized unlock_object..."))
        // count the unauthorized unlock tries and after a defined amount of tries lock the lock for a specific time
        ++this->unauthorized_unlock_try_counter;
        if (this->unauthorized_unlock_try_counter >= allowed_num_of_unauthorized_unlock_tries)
        {
            String msg = F("Locking the lock for ");
            msg += locking_period;
            msg += F(" seconds due to to often tried to unlock");
            DEBUG_PRINTLN(msg)
            if (locked_until_time_point == nullptr)
            {
                locked_until_time_point = new RtcDateTime(0);
            }
            *locked_until_time_point = (system_clock.GetDateTime());
            *locked_until_time_point += locking_period;

            if (!_save_locked_until_time_point())
            {
                logger.log(F("LOCK: Failed to write locked_until_time_point to the system_clock"), Log::log_level::L_ERROR);
            }

            this->unlocking_allowed = false;
            _lock();
            this->unauthorized_unlock_try_counter = 0;
        }
    }
    else
    {
        String msg = F("unlocking is forbidden until ");
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
    }
}

void Lock::_lock()
{
    if (this->state != lock_state::LOCKED)
    {
        bool success = on_locking(); // calling the "switch_state" function which will lock the physical lock
        if (!success)
        {
            logger.log(F("LOCK: Error locking the physical Lock"), Log::log_level::L_ERROR);
        }
        this->state = lock_state::LOCKED;
    }
}
void Lock::_unlock()
{
    if (this->state != lock_state::UNLOCKED)
    {
        bool success = on_unlocking(); // calling the "switch_state" function which will unlock the physical lock
        if (!success)
        {
            logger.log(F("LOCK: Error unlocking the physical Lock"), Log::log_level::L_ERROR);
        }
        this->unlock_time_point = system_clock.GetDateTime();
        this->state = lock_state::UNLOCKED;
    }
}

bool Lock::_save_locked_until_time_point()
{
    if (locked_until_time_point == nullptr)
        return false;

    system_clock.SetMemory(SYSTEM_CLOCK_MEMORY_LENGTH, static_cast<uint8_t>(true)); // setting the flag that we have saved a time_point in the memory
    uint8_t num_written_bytes = system_clock.SetMemory(reinterpret_cast<uint8_t *>(locked_until_time_point), sizeof(RtcDateTime));
    if (num_written_bytes < sizeof(RtcDateTime))
    {
        return false;
    }
    return true;
}
bool Lock::_delete_locked_until_time_point()
{
    system_clock.SetMemory(SYSTEM_CLOCK_MEMORY_LENGTH, static_cast<uint8_t>(false));
    return true;
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

void Lock::loop()
{
    RtcDateTime now = system_clock.GetDateTime();
    if (state == lock_state::UNLOCKED)
    {
        RtcDateTime lock_time_point(this->unlock_time_point); // when this timepoint is passed the lock has to be locked (lock_time_point+lock_timer)
        lock_time_point += this->lock_timer;                  // addition is in seconds
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
                    DEBUG_PRINTLN("Unlock_Object is disabled")
                    break;
                }
                case Unlock_Object::unlock_authentication_reports::UNLOCK_OBJECT_READ_ERROR:
                {
                    DEBUG_PRINTLN("Error while reading Unlock_Object!!!!")
                    break;
                }
                case Unlock_Object::unlock_authentication_reports::NO_UNLOCK_OBJECT_PRESENT:
                {
                    DEBUG_PRINTLN("No Unlock_Object available")
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