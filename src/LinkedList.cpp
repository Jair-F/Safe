#include "LinkedList.hpp"
#include <assert.h>

template <typename T>
SinglyLinkedList<T>::SinglyLinkedList() : _last(T(), nullptr), _end(&_last), _begin(_end), _size(0) {}

template <typename T>
SinglyLinkedList<T>::SinglyLinkedList(unsigned long size, T &_standard_initializer) : _last(T(), nullptr), _end(&_last), _begin(_end), _size(0)
{
    for (unsigned long i = 0; i < size; i++)
    {
        this->push_back(_standard_initializer);
    }
}

template <typename T>
SinglyLinkedList<T>::~SinglyLinkedList()
{
    this->clear();
}

template <typename T>
void SinglyLinkedList<T>::push_back(T &_data)
{
    if (this->begin() == this->end()) // no element in the list...
    {
        this->_end = new SinglyListNode<T>(_data, &_last);
        this->_begin = this->_end;
    }
    else // at least one element in the list
    {
        SinglyListNode<T> *tmp = new SinglyListNode<T>(_data, &_last);
        this->_end->next = tmp;
        this->_end = tmp;
    }
    ++_size;
}

template <typename T>
void SinglyLinkedList<T>::push_front(T &_data)
{
    if (this->begin() == this->end()) // list is empty
    {
        this->push_back(_data);
    }
    else
    {
        SinglyListNode<T> *tmp = new SinglyListNode<T>(_data);
        tmp->next = this->_begin;
        this->_begin = tmp;
        ++_size;
    }
}

template <typename T>
void SinglyLinkedList<T>::insert(unsigned long _position, T &_data)
{
    assert(_position <= this->_size);
    if (this->begin() == this->end()) // list is empty
    {
        this->_end = new SinglyListNode<T>(_data, &_last);
        this->_begin = _end;
    }
    else // at least one element in the list
    {
        if (_position == 0)
        {
            SinglyListNode<T> *tmp = new SinglyListNode<T>(_data, this->_begin->next());
            this->_begin = tmp;
        }
        else
        {
            SinglyListNodeIterator<T> iterator = this->begin();
            unsigned long counter = 0;
            while (counter < _position - 1) // hold on one element before position and insert there the element so the element will be element no _positoin
            {
                ++iterator;
                ++counter;
            }
            SinglyListNode<T> *tmp = new SinglyListNode<T>(_data, iterator.next());
            iterator._node->next = tmp;
        }
    }
    ++this->_size;
}

template <typename T>
T SinglyLinkedList<T>::pop_back()
{
    if (this->_size > 0)
    {
        SinglyListNode<T> *element_to_delete = this->_end;
        T data_backup = element_to_delete->data;
        // finding previous element of _end
        SinglyListNodeIterator<T> tmp(this->_begin());
        while (tmp.next() != this->_end)
            ++tmp;
        tmp._node->next = this->_last; // setting next poniter from the element before _end

        delete element_to_delete;
        --this->_size;
        return data_backup;
    }
    return;
}

template <typename T>
T SinglyLinkedList<T>::pop_front()
{
    return this->erase(0);
}

template <typename T>
T SinglyLinkedList<T>::erase(unsigned long _position)
{
    assert(this->_size > 0);
    if (this->_size > _position)
    {
        T data_backup;
        SinglyListNode<T> *element_to_delete;
        SinglyListNodeIterator<T> tmp = this->begin();

        unsigned long counter = 0;
        while (counter < (_position - 1)) // holding one element before the element to delete
        {
            ++tmp;
            ++counter;
        }
        element_to_delete = tmp.next();
        data_backup = element_to_delete->data;
        tmp._node->next = element_to_delete->next();
        delete element_to_delete;
        --this->_size;
        return data_backup;
    }
}

template <typename T>
void SinglyLinkedList<T>::clear()
{
    SinglyListNodeIterator<T> iterator = this->begin();
    SinglyListNode<T> *element_to_delete;
    while (iterator != this->end())
    {
        element_to_delete = iterator._node;
        ++iterator;
        delete element_to_delete;
    }
    this->_size = 0;
}

template <typename T>
unsigned long SinglyLinkedList<T>::length() const
{
    return this->_size;
}
template <typename T>
unsigned long SinglyLinkedList<T>::size() const
{
    return this->_size;
}

template <typename T>
T &SinglyLinkedList<T>::at(unsigned long _index)
{
    assert(_index < this->_size);
    SinglyListNodeIterator<T> iterator = this->begin();
    unsigned long counter = 0;
    while (counter < this->_size) // after this while the iterator is on the requested position
    {
        ++iterator;
        ++counter;
    }
    return iterator._node->data;
}

template <typename T>
const T &SinglyLinkedList<T>::at(unsigned long _index) const
{
    assert(_index < this->_size);
    SinglyListNodeIterator<T> iterator = this->begin();
    unsigned long counter = 0;
    while (counter < this->_size) // after this while the iterator is on the requested position
    {
        ++iterator;
        ++counter;
    }
    return iterator._node->data;
}
