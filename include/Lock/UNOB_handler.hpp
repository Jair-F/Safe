#pragma once
#include "Unlock_Object.hpp"
#include "LinkedList.hpp"

/**
 * Unlock Object handler...
 * class for handling an array of unlock_objects to iterate
 * over them and read them out easily
 */
class UNOB_handler
{
public:
    UNOB_handler() : unob_objects() {}
    virtual ~UNOB_handler() {}
    UNOB_handler(const UNOB_handler &) = delete;
    UNOB_handler(const UNOB_handler &&) = delete;
    UNOB_handler &operator=(const UNOB_handler &) = delete;

    void add_unob(Unlock_Object *_unob);

    /**
     * @return true if at least at one unlock_object at least one
     *         authorized unlock_object is set in the database
     *         false if the authorized unlock object database of all
     *         unobs is empty
     */
    bool unob_authorized_database_set();

    /**
     * @return the first unlock_object that returns not NO_UNLOCK_OBJECT_PRESENT.
     *          if all return NO_UNLOCK_OBJECT_PRESENT return also NO_UNLOCK_OBJECT_PRESENT.
     */
    Unlock_Object::unlock_authentication_reports read_unobs();

    SinglyLinkedList<Unlock_Object *> &get_unob_list() { return unob_objects; }

private:
    SinglyLinkedList<Unlock_Object *> unob_objects; // unlock objects list
};
