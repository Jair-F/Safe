#include "Lock/UNOB_handler.hpp"

void UNOB_handler::add_unob(Unlock_Object *_unob)
{
    if (_unob != nullptr)
        this->unob_objects.push_back(_unob);
}

Unlock_Object::unlock_authentication_reports UNOB_handler::read_unobs()
{
    auto unob = this->unob_objects.begin();
    while (unob != this->unob_objects.end())
    {
        if (unob->is_enabled())
        {
            auto read_result = unob->read();
            if (read_result != Unlock_Object::unlock_authentication_reports::NO_UNLOCK_OBJECT_PRESENT &&
                // at an error keep listening - error will be ignored - handled by unob class
                read_result != Unlock_Object::unlock_authentication_reports::UNLOCK_OBJECT_READ_ERROR)
                return read_result;
        }
        ++unob;
    }
    return Unlock_Object::unlock_authentication_reports::NO_UNLOCK_OBJECT_PRESENT;
}

bool UNOB_handler::unob_authorized_database_set()
{
    auto unob = this->unob_objects.begin();
    bool least_one_set = false;
    while (unob != this->unob_objects.end())
    {
        if (!unob->authorized_unob_database_empty() &&
            unob->is_enabled())
        {
            least_one_set = true;
            break;
        }
        ++unob;
    }
    return least_one_set;
}
