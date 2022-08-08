#include "Unlock_Object.hpp"

Unlock_Object::Unlock_Object(Lock *_lock, bool _enabled) : enabled(_enabled), unlock_object_id(_lock->_register_unlock_object(this))
{
}

Unlock_Object::~Unlock_Object()
{
}

/*
void Unlock_Object::loop()
{
    Unlock_Object::unlock_authentication_reports authentication_report = this->read();
    switch (authentication_report)
    {
    case Unlock_Object::unlock_authentication_reports::AUTHORIZED_UNLOCK_OBJECT:
    {
        this->utoken->request_unlock();
        break;
    }
    case Unlock_Object::unlock_authentication_reports::UNAUTHORIZED_UNLOCK_OBJECT:
    {
        this->utoken->report_unauthorized_unlock_try();
        break;
    }

    default:
    {
        this->utoken->report_unauthorized_unlock_try();
        break;
    }
    }
}
*/
