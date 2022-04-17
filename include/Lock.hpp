#pragma once

#include "system_clock.hpp"

namespace Lock
{
    class Lock;
    class unlock_token;

    enum lock_state
    {
        LOCKED = 0,
        UNLOCKED
    };

    /*
        - Handles the connection between the lock and the unlock_objects
        - Locks the lock automatically when its unlocked after a specific timer
    */
    class Lock
    {
    public:
        /*
            @param _lock_timer timer in seconds after how many secons the lock should be locked automatically after its unlocked
            @param lock_state initial state of the lock if true(lock unlocked) the lock will be locked after the timer is passed
            @param _allow_unlocking whether to allow unlock_requests or not
        */
        Lock(const unsigned short _lock_timer, lock_state _lock_state = lock_state::LOCKED, bool _allow_unlocking = true);

        ~Lock();

        /*
            @return a new generated unlock_token !!! You are responsible to delete
                    this object afterwards !!!
        */
        unlock_token *create_unlock_token();

        // ignoring unlocking_allowed and unlock the lock
        void force_unlock();

        // lock the lock imidieately
        void lock();

        /*
            @return true if the lock is locked
        */
        bool is_locked() { return this->state == lock_state::LOCKED; }

        enum lock_state get_state();

        /*
            the loop function which should be called in a loop - checks every time if the timer
            is passed and then locks the lock.
        */
        void loop();

        /*
            if it was tried to unlock the lock with a unlock_method/unlock_object and the fingerprint/RFID_tag
            wasnt in the allowed_tags... the object will report a report_unathorized_unlock_try. a text will be
            displayed on the display and maybe after a number of tries the lock will be locked.
        */
        void report_unauthorized_unlock_try();

        /*
            @return true if the lock was unlocked false if forbidden
        */
        bool request_unlock();

        // allow unlocking_requests
        void allow_unlocking() { this->unlocking_allowed = true; }

        // forbid unlocking_requests
        void forbid_unlocking() { this->unlocking_allowed = false; }

    protected:
        // switches the state and calls the requred functions to perform the state-switch
        void _lock();
        // switches the state and calls the requred functions to perform the state-switch !!! doesnt check if unlocking is alowed !!!
        void _unlock();

        /*
            save the locked_until_time_point on the system_clock memory
            @return true on success
        */
        bool _save_locked_until_time_point();
        /*
            delete the locked_until_time_point from the system_clock memory
            @return true on successful
        */
        bool _delete_locked_until_time_point();

    private:
        lock_state state;
        // true if unlocking is allowed
        bool unlocking_allowed;

        byte unauthorized_unlock_try_counter;

        // after how much time the lock will lock itself if it is being unlocked - in seconds
        const unsigned short lock_timer;

        /*
            allowed num of unlock_tries until the lock locks itself for a specific time
        */
        const byte allowed_num_of_unauthorized_unlock_tries = 2;
        /*
            - if there were "allowed_num_of_unauthorized_unlock_tries" unauthorized_unlock_try-reports the lock locks
                itself for this time_period
            - locking_period is in seconds
        */
        const unsigned short locking_period = 60;

        // time point when the lock was unlocked the last time - for the timer
        RtcDateTime unlock_time_point;

        /*
            - if the lock is locked due to too many try's(report_unauthorized_unlock_try) the lock will be locked
            until this time-point. Also the unlocking_allowed will be set on false until this time-point passed.
            - This timepoint will be stored in the RTC-Memory. if its not set - not locked
            it will have the value nullptr
        */
        RtcDateTime *locked_until_time_point;

        /*
            function which will be called when lock will be locked
            @return true if locking was successful
        */
        bool (*on_locking)(void) = []()
        {DEBUG_PRINT("locking the lock"); return true; };
        /*
            function which will be called when lock will be unlocked
            @return true if unlocking was successful
        */
        bool (*on_unlocking)(void) = []()
        {DEBUG_PRINT("unlocking the lock"); DEBUG_PRINT(system_clock.GetDateTime()); return true; };
    };

    /*
        - a wrapper for "clients" due safety reasons
        - for the unlock_objects
        - a tocken to request unlocking - to not accidentially or
            intentionally unlock the Lock
        or do something else with the lock
    */
    class unlock_token
    {
    public:
        unlock_token(Lock *_lock);
        unlock_token(const unlock_token &) = delete;
        unlock_token &operator=(const unlock_token &) = delete;
        ~unlock_token() {}
        /*
            @return true if the lock was unlocked false if forbidden
        */
        bool request_unlock();
        /*
            if it was tried to unlock the lock with a unlock_method/unlock_object and the fingerprint/RFID_tag
            wasnt in the allowed_tags... the object will report a report_unathorized_unlock_try. a text will be
            displayed on the display and maybe after a number of tries the lock will be locked.
        */
        void report_unauthorized_unlock_try();
        /*
            @return true if the lock is locked
        */
        bool is_locked();
        lock_state get_lock_state();
        // lock the lock imideately
    private:
        Lock *lock;
    };
}

// ------------- Implementations -------------

// Lock implementations
Lock::Lock::Lock(const unsigned short _lock_timer, lock_state _lock_state = lock_state::LOCKED, bool _allow_unlocking = true) : unlocking_allowed(_allow_unlocking),
                                                                                                                                state(_lock_state), lock_timer(_lock_timer),
                                                                                                                                unlock_time_point(0), unauthorized_unlock_try_counter(0)
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
                DEBUG_PRINT(F("Failed to read locked_until_timepoint from the system_clock"))
            }
            locked_until_time_point = reinterpret_cast<RtcDateTime *>(buffer); // RtcDateTime is exactly 6*sizeof(uint8_t)=48 bits

            RtcDateTime now = system_clock.GetDateTime();

            if ((*locked_until_time_point) > now)
            {
                DEBUG_PRINT(F("still locked"))
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
        DEBUG_PRINT(F("RTC-Module lost power - we dont read locked_until_time_point"))
    }
}

Lock::Lock::~Lock()
{
    if (this->locked_until_time_point != nullptr)
    {
        delete this->locked_until_time_point;
        this->locked_until_time_point = nullptr;
    }
}

Lock::unlock_token *Lock::Lock::create_unlock_token()
{
    return new unlock_token(this);
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
            DEBUG_PRINT(msg)
            if (locked_until_time_point == nullptr)
            {
                locked_until_time_point = new RtcDateTime(0);
            }
            *locked_until_time_point = (system_clock.GetDateTime());
            *locked_until_time_point += locking_period;

            if (!_save_locked_until_time_point())
            {
                DEBUG_PRINT(F("[ERROR] Failed to write locked_until_time_point to the system_clock"))
            }

            this->unlocking_allowed = false;
            _lock();
            this->unauthorized_unlock_try_counter = 0;
        }
    }
    else
    {
        DEBUG_PRINT(F("unlocking is currently forbidden"))
    }
}

void Lock::Lock::_lock()
{
    if (this->state != lock_state::LOCKED)
    {
        bool success = on_locking(); // calling the "switch_state" function which will lock the physical lock
        if (!success)
        {
            DEBUG_PRINT(F("Error locking the physical Lock!!"))
        }
        this->state = lock_state::LOCKED;
    }
}
void Lock::Lock::_unlock()
{
    if (this->state != lock_state::UNLOCKED)
    {
        bool success = on_unlocking(); // calling the "switch_state" function which will unlock the physical lock
        if (!success)
        {
            DEBUG_PRINT(F("Error unlocking the physical Lock!!"))
        }
        this->unlock_time_point = system_clock.GetDateTime();
        this->state = lock_state::UNLOCKED;
    }
}

bool Lock::Lock::_save_locked_until_time_point()
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
bool Lock::Lock::_delete_locked_until_time_point()
{
    system_clock.SetMemory(SYSTEM_CLOCK_MEMORY_LENGTH, static_cast<uint8_t>(false));
}

bool Lock::Lock::request_unlock()
{
    if (this->unlocking_allowed)
    {
        this->unauthorized_unlock_try_counter = 0; // reset the unauthorized_unlock_tries
        this->_unlock();
        return true;
    }
    else
    {
        DEBUG_PRINT(F("unloking currently not allowed - disableds"))
    }
    return false;
}

void Lock::Lock::force_unlock()
{
    this->_unlock();
}

void Lock::Lock::lock()
{
    _lock();
}

enum Lock::lock_state Lock::Lock::get_state()
{
    return this->state;
}

void Lock::Lock::loop()
{
    RtcDateTime now = system_clock.GetDateTime();
    if (state == lock_state::UNLOCKED)
    {
        RtcDateTime lock_time_point(this->unlock_time_point); // when this timepoint is passed the lock has to be locked (lock_time_point+lock_timer)
        lock_time_point += this->lock_timer;                  // addition is in seconds
        DEBUG_PRINT(now);

        if (now > lock_time_point)
        {
            this->_lock();
        }
    }

    // unlocking not allowed due to too many tries
    if (this->locked_until_time_point != nullptr)
    {
        if (now > *locked_until_time_point)
        {
            DEBUG_PRINT(F("unlocking is now allowed"))
            this->unlocking_allowed = true;
            _delete_locked_until_time_point();
            delete locked_until_time_point;
            locked_until_time_point = nullptr;
        }
    }
}

// unlock_token implementations
Lock::unlock_token::unlock_token(Lock *_lock) : lock(_lock)
{
}

bool Lock::unlock_token::request_unlock()
{
    return lock->request_unlock();
}

void Lock::unlock_token::report_unauthorized_unlock_try()
{
    this->lock->report_unauthorized_unlock_try();
}

bool Lock::unlock_token::is_locked()
{
    return lock->is_locked();
}

enum Lock::lock_state Lock::unlock_token::get_lock_state()
{
    return lock->get_state();
}
