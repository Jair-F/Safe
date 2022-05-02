#include "LinkedList.hpp"

template <typename T>
SinglyLinkedList<T>::SinglyLinkedList() : _end(nullptr), _begin(_end), _size(0) {}

template <typename T>
SinglyLinkedList<T>::SinglyLinkedList(unsigned long size, T &_standard_initializer) : _end(nullptr), _begin(end), _size(size)
{
    if (size > 0)
    {
        for (unsigned long i = 0; i < size; i++)
        {
            this->push_back(_standard_initializer);
        }
    }
}

template <typename T>
void SinglyLinkedList<T>::push_back(T &_data)
{
    if (this->_begin == nullptr) // no element in the list...
    {
        this->_end = new SinglyListNode<T>(_data, nullptr);
        this->_begin = this->_end;
    }
    else // at least one element in the list
    {
        SinglyListNode<T> *tmp = new SinglyListNode<T>(_data, nullptr);
        this->_end->next = tmp;
        this->_end = tmp;
    }
}

template <typename T>
void SinglyLinkedList<T>::push_front(T &_data)
{
    SinglyListNode<T> *tmp = new SinglyListNode<T>(_data);
    tmp->next = this->_begin;
    if (this->_begin == this->_end) // this is the first element - _end and _begin point to the same address
        this->_end == tmp;
    this->_begin = tmp;
}