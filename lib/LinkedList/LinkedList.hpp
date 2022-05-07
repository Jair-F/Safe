#pragma once
#include <assert.h>

/*
    ListNode for simple singly linked list - one direction linked
*/
template <typename T>
class SinglyListNode
{
public:
    SinglyListNode(T _data, SinglyListNode<T> *_next) : data(_data), next(_next) {}
    T &operator*() const { return data; }

    /*
        consider the next pointer and the data...
    */
    bool operator==(const SinglyListNode &_node) const { return (this->data == _node.data && this->next == _node.next); }
    bool operator!=(const SinglyListNode &_node) const { return !(*this == _node); }

    SinglyListNode<T> *next;
    T data;
};

/*
    Iterator for singly linked list - linked in one direction
*/
template <typename T>
class SinglyListNodeIterator
{
public:
    SinglyListNodeIterator(SinglyListNode<T> *_ln) : _node(_ln) {}
    SinglyListNodeIterator(SinglyListNode<T> &_ln) : _node(&_ln) {}
    /*
        post increment-operator(a++) - return the current data - before the switch
    */
    SinglyListNode<T> *operator++(int)
    {
        SinglyListNode<T> *tmp = this->_node;
        this->_node = this->_node->next;
        return tmp;
    }

    /*
        pre increment-operator(++a) - return the data after the switch
    */
    SinglyListNode<T> &operator++()
    {
        this->_node = this->_node->next;
        return *(this->_node);
    }

    // consider only the data
    bool operator==(const SinglyListNodeIterator &_list_iterator) const { return this->_node->data == _list_iterator.data(); }
    bool operator!=(const SinglyListNodeIterator &_list_iterator) const { return !(*this == _list_iterator); }

    bool operator>(const SinglyListNodeIterator &_list_iterator) const { return (this->_node->data() > _list_iterator.data()); }
    bool operator<(const SinglyListNodeIterator &_list_iterator) const { return !(*this > _list_iterator); }

    bool operator>=(const SinglyListNodeIterator &_list_iterator) const { return (*this > _list_iterator || *this == _list_iterator); }
    bool operator<=(const SinglyListNodeIterator &_list_iterator) const { return (*this < _list_iterator || *this == _list_iterator); }

    T &data() { return this->_node->data; }
    T &operator->() { return this->data(); }
    T *operator*() { return &(this->data()); }
    SinglyListNode<T> *next() { return this->_node->next; }

    SinglyListNode<T> *_node;
};

/*
    Simple linked list - linked only in one direction

    works for every element which has implemented following methods:
        - copy constructor
        - compare-operator - operator==()
        - bigger than operator - operator>()
*/
template <typename T>
class SinglyLinkedList
{
public:
    SinglyLinkedList();
    /*
        @param _size size of the list
        @param _standard_initializer initialize the whole list with this value
    */
    SinglyLinkedList(unsigned long _size, T _standard_initializer);
    ~SinglyLinkedList();

    void push_front(T _data);
    void push_back(T _data);

    /*
        insert _data on the _position and push the data that is now on this positon
        one back
    */
    void insert(unsigned long _position, T _data);

    /*
        insert the data by moving it inside
    */
    // void emplace(unsigned long _position, T &_data);

    /*
        sort the list ascending
    */
    // void sort();

    /*
        @return the deleted element - standard-constructor of T if there isn't any more elements in the list
    */
    T pop_back();
    /*
        @return the deleted element - standard-constructor of T if there isn't any more elements in the list
    */
    T pop_front();
    T erase(unsigned long _position);
    /*
        delete the whole list
    */
    void clear();

    unsigned long length() const;
    // same as length()
    unsigned long size() const;

    T &operator[](unsigned long _index) { return this->at(_index); }
    const T &operator[](unsigned long _index) const { return this->at(_index); }
    T &at(unsigned long _index);
    const T &at(unsigned long _index) const;

    SinglyListNodeIterator<T> begin() { return this->_begin; }
    SinglyListNodeIterator<T> end() { return this->_last; }

private:
    SinglyListNode<T> _last; // the element which will be returned in a iterator at calling end(element past the last element of data)
    SinglyListNode<T> *_end;
    SinglyListNode<T> *_begin;
    unsigned long _size;
};

// ---------------- Implementations ----------------

template <typename T>
SinglyLinkedList<T>::SinglyLinkedList() : _last(T(), nullptr), _size(0)
{
    this->_end = &_last;
    this->_begin = (_end);
}

template <typename T>
SinglyLinkedList<T>::SinglyLinkedList(unsigned long size, T _standard_initializer) : _last(T(), nullptr), _size(0)
{
    this->_end = &_last;
    this->_begin = (_end);
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
void SinglyLinkedList<T>::push_back(T _data)
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
void SinglyLinkedList<T>::push_front(T _data)
{
    if (this->begin() == this->end()) // list is empty
    {
        this->push_back(_data);
    }
    else
    {
        SinglyListNode<T> *tmp = new SinglyListNode<T>(_data, this->_begin);
        this->_begin = tmp;
        ++_size;
    }
}

template <typename T>
void SinglyLinkedList<T>::insert(unsigned long _position, T _data)
{
    assert(_position <= this->size());
    if (this->begin() == this->end()) // list is empty
    {
        this->_end = new SinglyListNode<T>(_data, &_last);
        this->_begin = _end;
    }
    else // at least one element in the list
    {
        if (_position == 0)
        {
            SinglyListNode<T> *tmp = new SinglyListNode<T>(_data, this->_begin);
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
    if (this->size() > 0)
    {
        SinglyListNode<T> *element_to_delete = this->_end;
        T data_backup = element_to_delete->data;
        // finding one element before _end
        SinglyListNodeIterator<T> tmp(this->_begin);

        if (this->size() > 1)
        {
            while (tmp.next() != element_to_delete)
                ++tmp;
            tmp._node->next = &this->_last; // setting next poniter from the element before _end
            this->_end = tmp._node;
        }
        // if we only have left one element the _end and _begin point to the same element
        // and then we cant just iterate to the element before _end!!
        else
        {
            this->_end = &this->_last;
            this->_begin = this->_end;
        }

        delete element_to_delete;
        --this->_size;
        return data_backup;
    }
    return;
}

template <typename T>
T SinglyLinkedList<T>::pop_front()
{
    if (this->size() > 0)
    {
        return this->erase(0);
    }
}

template <typename T>
T SinglyLinkedList<T>::erase(unsigned long _position)
{
    assert(this->size() >= 0);
    if (this->_size > _position)
    {
        T data_backup;
        SinglyListNode<T> *element_to_delete;

        if (_position == 0)
        {
            element_to_delete = this->_begin;
            this->_begin = element_to_delete->next;
        }
        else
        {
            SinglyListNodeIterator<T> tmp = this->begin(); // one element before the element_to_delete
            unsigned long counter = 0;
            while (counter < (_position - 1)) // holding one element before the element to delete
            {
                ++tmp;
                ++counter;
            }
            element_to_delete = tmp.next();
            tmp._node->next = element_to_delete->next; // hoping over the element_to_delete
        }

        data_backup = element_to_delete->data;
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
        --this->_size;
    }
    this->_end = &_last;
    this->_begin = this->_end;
    // this->_size = 0;
}

template <typename T>
unsigned long SinglyLinkedList<T>::length() const
{
    return this->size();
}
template <typename T>
unsigned long SinglyLinkedList<T>::size() const
{
    return this->_size;
}

template <typename T>
T &SinglyLinkedList<T>::at(unsigned long _index)
{
    assert(_index < this->size());
    SinglyListNodeIterator<T> iterator = this->begin();
    unsigned long counter = 0;
    while (counter < _index) // after this while the iterator is on the requested position
    {
        ++iterator;
        ++counter;
    }
    return iterator._node->data;
}

template <typename T>
const T &SinglyLinkedList<T>::at(unsigned long _index) const
{
    assert(_index < this->size());
    SinglyListNodeIterator<T> iterator = this->begin();
    unsigned long counter = 0;
    while (counter < _index) // after this while the iterator is on the requested position
    {
        ++iterator;
        ++counter;
    }
    return iterator._node->data;
}
