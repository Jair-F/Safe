#pragma once

#include "system_clock/system_clock.hpp"
#include "Helper.hpp"
#include "Unlock_Object.hpp"
#include "LinkedList.hpp"

class Unlock_Object;

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
    Lock(const unsigned short _lock_timer, lock_state _lock_state = lock_state::LOCKED);

    ~Lock();

    /**
     * begin the lock
     */
    bool begin();

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
    void allow_unlocking();

    bool get_unlocking_allowed() { return this->unlocking_allowed; }

    // forbid unlocking_requests
    void forbid_unlocking() { this->unlocking_allowed = false; }

    /**
     * @brief how many times we can fail to try to unlock the lock until he locks itself for a defined time period
     */
    inline void set_allowed_unauthorized_unlock_tries(uint8_t num_unlock_tries) { this->allowed_unauthorized_unlock_tries = num_unlock_tries; }
    inline uint8_t get_allowed_unauthorized_unlock_tries() const { return this->allowed_unauthorized_unlock_tries; }
    /**
     * - if there were "allowed_unauthorized_unlock_tries" unauthorized_unlock_try-reports the lock locks
     *     itself for this duration
     * - locking_period is in seconds
     */
    inline void set_locking_period(unsigned short _locking_period) { this->locking_period = _locking_period; }
    inline unsigned short get_locking_period() const { return this->locking_period; }
    /**
     * @brief after how much time the lock will lock itself if it is being unlocked - in seconds
     */
    inline void set_lock_timer(unsigned short _lock_timer) { this->relock_timer = _lock_timer; }
    /**
     * @brief after how much time the lock will lock itself if it is being unlocked - in seconds
     */
    inline unsigned short get_relock_timer() const { return this->relock_timer; }

    /*
        for the unlock_object to register them at the lock when they are created
        not a function for the user which holds the lock-class!!
        @return the unlock_object_id for that unlock_object
    */
    uint16_t _register_unlock_object(Unlock_Object *_unlock_object);

protected:
    // switches the state and calls the requred functions to perform the state-switch
    void
    _lock();
    // switches the state and calls the requred functions to perform the state-switch !!! doesnt check if unlocking is alowed !!!
    void _unlock();

private:
    SinglyLinkedList<uint16_t> unlock_object_ids;
    SinglyLinkedList<Unlock_Object *> unlock_objects;
    lock_state state;
    // true if unlocking is allowed
    bool unlocking_allowed;

    byte unauthorized_unlock_try_counter;

    // after how much time the lock will lock itself if it is being unlocked - in seconds
    unsigned short relock_timer;

    /*
        allowed num of unlock_tries until the lock locks itself for a specific time
    */
    uint8_t allowed_unauthorized_unlock_tries = 5;
    /*
        - if there were "allowed_unauthorized_unlock_tries" unauthorized_unlock_try-reports the lock locks
            itself for this duration
        - locking_period is in seconds
    */
    uint16_t locking_period = 60;

    // time point when the lock was unlocked the last time - for the timer
    Clock::time_point unlock_time_point;

    /*
        - if the lock is locked due to too many try's(report_unauthorized_unlock_try) the lock will be locked
        until this time-point. Also the unlocking_allowed will be set on false until this time-point passed.
        - This timepoint will be stored in the RTC-Memory. if its not set - not locked
        it will have the value nullptr
    */
    Clock::time_point *locked_until_time_point;

    /*
        function which will be called when lock will be locked
        @return true if locking was successful
    */
    bool (*on_locking)(void) = [](void) -> bool
    {DEBUG_PRINTLN("locking the lock"); return true; };
    /*
        function which will be called when lock will be unlocked
        @return true if unlocking was successful
    */
    bool (*on_unlocking)(void) = []() -> bool
    {DEBUG_PRINTLN("unlocking the lock"); return true; };
};
