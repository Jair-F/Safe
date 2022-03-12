#pragma once

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
        Lock(const unsigned short _lock_timer, lock_state _lock_state = lock_state::LOCKED, bool _allow_unlocking = true) : unlocking_allowed(_allow_unlocking),
                                                                                                                            state(_lock_state), lock_timer(_lock_timer),
                                                                                                                            unlock_time_point(0) {}

        /*
            @return a new generated unlock_token !!! You are responsible to delete
                    this object afterwards !!!
        */
        unlock_token *create_unlock_token();

        // ignoring unlocking_allowed and unlock the lock
        void force_unlock();

        // lock the lock imidieately
        // void lock();

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
        void report_unathorized_unlock_try();

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

    private:
        lock_state state;
        bool unlocking_allowed;
        // after how much seconds the lock will lock itself if it is being unlocked
        const unsigned short lock_timer;

        // time point when the lock was unlocked the last time - for the timer
        unsigned long unlock_time_point;

        // function which will be called when lock will be locked
        bool (*on_locking)(void) = []()
        {Serial.println("locking the lock"); return true; };
        // function which will be called when lock will be unlocked
        bool (*on_unlocking)(void) = []()
        {Serial.println("unlocking the lock"); return true; };
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
        unlock_token(Lock &_lock) : lock(_lock) {}
        unlock_token(const unlock_token &) = delete;
        unlock_token &operator=(const unlock_token &) = delete;
        ~unlock_token() {}
        /*
            @return true if the lock was unlocked false if forbidden
        */
        bool request_unlock() { return lock.request_unlock(); }
        /*
            if it was tried to unlock the lock with a unlock_method/unlock_object and the fingerprint/RFID_tag
            wasnt in the allowed_tags... the object will report a report_unathorized_unlock_try. a text will be
            displayed on the display and maybe after a number of tries the lock will be locked.
        */
        void report_unathorized_unlock_try() { this->lock.report_unathorized_unlock_try(); }
        /*
            @return true if the lock is locked
        */
        bool is_locked() { return lock.is_locked(); }
        lock_state get_lock_state() { return lock.get_state(); }
        // lock the lock imideately
        // void lock() { lock->lock(); }
    private:
        Lock &lock;
    };
}

// ------------- Implementations -------------

// Lock

Lock::unlock_token *Lock::Lock::create_unlock_token()
{
    return new unlock_token(*this);
}

void Lock::Lock::report_unathorized_unlock_try()
{
    Serial.println("Unauthorized unlock_object...");
}

void Lock::Lock::_lock()
{
    bool success = on_locking(); // calling the "switch_state" function which will lock the physical lock
    if (!success)
    {
        Serial.println("Error locking the physical Lock!!");
    }
    this->state = lock_state::LOCKED;
}
void Lock::Lock::_unlock()
{
    bool success = on_unlocking(); // calling the "switch_state" function which will unlock the physical lock
    if (!success)
    {
        Serial.println("Error unlocking the physical Lock!!");
    }
    this->unlock_time_point = millis();
    this->state = lock_state::UNLOCKED;
}

bool Lock::Lock::request_unlock()
{
    if (unlocking_allowed)
    {
        this->_unlock();
        return true;
    }
    return false;
}

void Lock::Lock::force_unlock()
{
    this->_unlock();
}

enum Lock::lock_state Lock::Lock::get_state()
{
    return this->state;
}

void Lock::Lock::loop()
{
    if (state == lock_state::UNLOCKED)
    {
        if (millis() > (this->lock_timer * 1000 + unlock_time_point)) // if the timer is passed
        {
            this->_lock();
        }
    }
}
