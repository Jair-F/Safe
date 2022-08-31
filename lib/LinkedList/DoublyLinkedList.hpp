#pragma once
#include <assert.h>

template <typename T>
class DoublyListNode
{
public:
    DoublyListNode() : next(nullptr), prev(nullptr) {}
    DoublyListNode(DoublyListNode<T> *_prev, DoublyListNode<T> *_next) : next(_next), prev(_prev) {}
    DoublyListNode(T _data, DoublyListNode<T> *_prev, DoublyListNode<T> *_next) : data(_data), next(_next), prev(_prev) {}
    ~DoublyListNode() {}

    T &operator*() const { return data; }

    /*
        consider the next,prev pointer and the data
    */
    bool operator==(const DoublyListNode<T> &_node) const
    {
        return _node.next == this->next &&
               _node.prev == this->prev &&
               _node.data == this->data;
    }
    bool operator!=(const DoublyListNode<T> &_node) const { return !(_node == *this); }

    T data;
    DoublyListNode<T> *next, *prev;
};

template <typename T>
class DoublyListNodeIterator
{
public:
    DoublyListNodeIterator(DoublyListNode<T> *_ln) : _node(_ln) {}
    DoublyListNodeIterator(DoublyListNode<T> &_ln) : _node(&_ln) {}

    /*
        post increment-operator(a++) - return the current data - before the switch
    */
    DoublyListNode<T> *operator++(int)
    {
        this->_node = this->_node->next;
        return this->_node->prev;
    }

    /*
        pre increment-operator(++a) - return the data after the switch
    */
    DoublyListNode<T> &operator++()
    {
        this->_node = this->_node->next;
        return *(this->_node);
    }

    /*
        post decrement-operator(a--) - return the current data - before the switch
    */
    DoublyListNode<T> *operator--(int)
    {
        this->_node = this->_node->prev;
        return this->_node->next;
    }

    /*
        pre decrement-operator(--a) - return the data after the switch
    */
    DoublyListNode<T> &operator--()
    {
        this->_node = this->_node->prev;
        return *(this->_node);
    }

    // consider the data and the prev and next pointers
    bool operator==(const DoublyListNodeIterator<T> &_list_iterator) const
    {
        return this->_node == _list_iterator.node() &&
               this->prev() == _list_iterator.prev() &&
               this->next() == _list_iterator.next();
    }
    bool operator!=(const DoublyListNodeIterator<T> &_list_iterator) const { return !(*this == _list_iterator); }

    T &data() const { return this->_node->data; }
    T &operator->() const { return this->_node->data; }
    T *operator*() const { return &this->_node->data; }
    DoublyListNode<T> *next() const { return this->_node->next; }
    DoublyListNode<T> *prev() const { return this->_node->prev; }
    DoublyListNode<T> *node() const { return this->_node; }

private:
    DoublyListNode<T> *_node;
};

/*
    Simple linked list - linked only in one direction

    works for every element which has implemented following methods:
        - standard constructor(T())
        - copy constructor
        - compare-operator - operator==()
        - bigger than operator - operator>()
*/
template <typename T>
class DoublyLinkedList
{
public:
    DoublyLinkedList();
    /*
        @param _size size of the list
        @param _standard_initializer initialize the whole list with this value
    */
    DoublyLinkedList(unsigned long _size, T _standard_initializer);
    ~DoublyLinkedList();

    void push_front(T _data);
    void push_back(T _data);

    /*
        reverse the whole list - first element will be last and last will be first
    */
    void reverse();

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
        @return the deleted element - undefined return value if there isn't any more elements in the list
    */
    T pop_back();
    /*
        @return the deleted element - undefined return value if there isn't any more elements in the list
    */
    T pop_front();
    /*
        @return the deleted element - undefined return value if there isn't any more elements in the list
    */
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

    DoublyListNode<T> &front() const { return *(this->_begin->next); }
    DoublyListNode<T> &last() const { return *(this->_end->prev); }
    DoublyListNodeIterator<T> begin() { return DoublyListNodeIterator<T>(this->_begin->next); }
    /*
        @return one element past the last element in the list
    */
    DoublyListNodeIterator<T> end() { return DoublyListNodeIterator<T>(this->_end); }

private:
    // SinglyListNodeBase<T> _last; // the element which will be returned in a iterator at calling end(element past the last element of data)
    DoublyListNode<T> *_end;
    DoublyListNode<T> *_begin;
    unsigned long _size;
};

template <typename T>
DoublyLinkedList<T>::DoublyLinkedList() : _end(new DoublyListNode<T>()), _begin(new DoublyListNode<T>()), _size(0)
{
    this->_begin->next = this->_end;
    this->_end->prev = this->_begin;
}

template <typename T>
DoublyLinkedList<T>::DoublyLinkedList(unsigned long size, T _standard_initializer) : _end(new DoublyListNode<T>()), _begin(new DoublyListNode<T>()), _size(0)
{
    this->_begin->next = this->_end;
    this->_end->prev = this->_begin;
    for (unsigned long i = 0; i < size; ++i)
    {
        this->push_back(_standard_initializer);
    }
}

template <typename T>
DoublyLinkedList<T>::~DoublyLinkedList()
{
    this->clear();
}

template <typename T>
void DoublyLinkedList<T>::push_back(T _data)
{
    DoublyListNode<T> *element_to_insert = new DoublyListNode<T>(_data, this->_end->prev, this->_end);
    DoublyListNode<T> &last_element = this->last(); // last element before we insert our element
    last_element.next = element_to_insert;
    this->_end->prev = element_to_insert;

    ++_size;
}

template <typename T>
void DoublyLinkedList<T>::push_front(T _data)
{
    DoublyListNode<T> *element_to_insert = new DoublyListNode<T>(_data, this->_begin, &this->front());

    DoublyListNode<T> *e_after = nullptr; // the element after the element we want to insert
    if (this->size() == 0)                // if there is no element in the list the element after the begin is the end
    {
        e_after = this->_end;
    }
    else
    {
        e_after = &this->front();
    }
    e_after->prev = element_to_insert;
    this->_begin->next = element_to_insert;
    ++_size;
}

template <typename T>
void DoublyLinkedList<T>::insert(unsigned long _position, T _data)
{
    assert(_position <= this->size());

    if (_position == 0)
    {
        this->push_front(_data);
    }
    else if (_position == this->size())
    {
        this->push_back(_data);
    }
    else
    {
        DoublyListNodeIterator<T> iterator(this->begin());
        unsigned long counter = 0;
        while (counter < _position - 1) // hold on one element before position and insert there the element so the element will be element no _positoin
        {
            ++iterator;
            ++counter;
        }
        DoublyListNode<T> *element_to_insert = new DoublyListNode<T>(_data, iterator.node(), iterator.next());
        DoublyListNode<T> *element_after = iterator.next(); // element after the element we want to insert

        iterator.node()->next = element_to_insert; // set the element before

        element_after->prev = element_to_insert; // set the prev pointer of the element after

        ++this->_size;
    }
}

template <typename T>
T DoublyLinkedList<T>::pop_back()
{
    if (this->size() > 0)
    {
        DoublyListNode<T> *element_to_delete = &this->last();
        T data_backup = element_to_delete->data;

        DoublyListNode<T> *e_before = element_to_delete->prev; // element before the element to delete

        e_before->next = this->_end;
        this->_end->prev = e_before;

        delete element_to_delete;
        --this->_size;
        return data_backup;
    }
    // return;
}

template <typename T>
T DoublyLinkedList<T>::pop_front()
{
    if (this->size() > 0)
    {
        return this->erase(0);
    }
}

template <typename T>
T DoublyLinkedList<T>::erase(unsigned long _position)
{
    assert(this->size() > 0);
    assert(_position < this->size());
    if (this->_size > _position)
    {
        T data_backup;
        DoublyListNode<T> *element_to_delete = nullptr;

        if (_position == 0)
        {
            element_to_delete = &this->front();
            this->_begin->next = element_to_delete->next;
            element_to_delete->next->prev = this->_begin; // setting the element after element_to_delete
        }
        else
        {
            // if (this->size() / 2 > _position)
            //{
            DoublyListNodeIterator<T> iterator(this->begin()); // one element before the element_to_delete
            unsigned long counter = 0;
            while (counter < _position - 1) // holding one element before the element to delete
            {
                ++iterator;
                ++counter;
            }
            element_to_delete = iterator.next();
            iterator.node()->next = element_to_delete->next; // the element before element_to_delete
            element_to_delete->next->prev = iterator.node(); // element after element_to_delete
            /*
            }
            else
            {
                DoublyListNodeIterator<T> iterator(this->_end); // one element before the element_to_delete
                unsigned long counter = this->size() - 1;
                while (counter > _position - 1) // holding one element before the element to delete
                {
                    --iterator;
                    ++counter;
                }
                element_to_delete = iterator.next();
                iterator.node()->next = element_to_delete->next; // the element before element_to_delete
                element_to_delete->next->prev = iterator.node(); // element after element_to_delete
            }
            */
        }
        data_backup = element_to_delete->data;
        delete element_to_delete;
        --this->_size;
        return data_backup;
    }
}

template <typename T>
void DoublyLinkedList<T>::clear()
{
    while (this->length() > 0)
    {
        this->pop_back();
    }
}

template <typename T>
unsigned long DoublyLinkedList<T>::length() const
{
    return this->size();
}
template <typename T>
unsigned long DoublyLinkedList<T>::size() const
{
    return this->_size;
}

template <typename T>
T &DoublyLinkedList<T>::at(unsigned long _index)
{
    assert(_index < this->size());

    // if element is in the first half of the list
    // if (this->size() / 2 > _index)
    //{
    DoublyListNodeIterator<T> iterator(this->begin());
    unsigned long counter = 0;
    while (counter < _index) // after this while the iterator is on the requested position
    {
        ++iterator;
        ++counter;
    }
    return iterator.data();
    /*
}
else
{
    DoublyListNodeIterator<T> iterator(this->_end);
    unsigned long counter = this->size() - 1;
    while (counter > _index)
    {
        --iterator;
        --counter;
    }
    return iterator.node()->data;
}
*/
}

template <typename T>
const T &DoublyLinkedList<T>::at(unsigned long _index) const
{
    assert(_index < this->size());

    // if element is in the first half of the list
    // if (this->size() / 2 > _index)
    //{
    DoublyListNodeIterator<T> iterator(this->begin());
    unsigned long counter = 0;
    while (counter < _index) // after this while the iterator is on the requested position
    {
        ++iterator;
        ++counter;
    }
    return iterator.data();
    /*
}
else
{
    DoublyListNodeIterator<T> iterator(this->_end);
    unsigned long counter = this->size() - 1;
    while (counter < _index)
    {
        --iterator;
        --counter;
    }
    return iterator.node()->data;
}
*/
}
