#pragma once

#include "system_clock.hpp"
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
    Lock(const unsigned short _lock_timer, lock_state _lock_state = lock_state::LOCKED, bool _allow_unlocking = true);

    ~Lock();

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
    SinglyLinkedList<uint16_t> unlock_object_ids;
    SinglyLinkedList<Unlock_Object *> unlock_objects;
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
    {DEBUG_PRINT("unlocking the lock"); return true; };
};
