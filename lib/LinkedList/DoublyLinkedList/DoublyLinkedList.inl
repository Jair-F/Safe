#pragma once
#include <assert.h>
#include "DoublyListNode.inl"
#include "DoublyListNodeIterator.inl"

/**
 * \defgroup DoublyLinkedList
 * \ingroup LinkedList
 *
 * @note
 * works for every element which has implemented following methods:
 *     - standard constructor(T())
 *     - copy constructor
 *     - compare-operator - operator==()
 *     - bigger than operator - operator>()
 *
 * @brief a simple linked list which is linked forward and backward with basic functionality.
 *
 * @{
 */
template <typename T>
class DoublyLinkedList
{
    // documentating the template parameter
    /**
     * @tparam T type of which the Linked List should be.
     */

public:
    /**
     * @brief create an empty list
     */
    DoublyLinkedList();
    /**
     * @param _size size of the list
     * @param _standard_initializer initialize the whole list with this value
     */
    DoublyLinkedList(unsigned long _size, T _standard_initializer);
    ~DoublyLinkedList();

    void push_front(T _data);
    void push_back(T _data);

    /**
     * @brief reverse the whole list - first element will be last and last will be first
     */
    // void reverse();

    /**
     * @brief insert _data on the _position and push the data that is now on this positon one back
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

    /**
     * @return the position of the first occourence of _data if present.
     *         -1 if not present in the list
     */
    long long search(const T &_data) const;

    /**
     * @return the deleted element - undefined return value if there isn't any more elements in the list
     */
    T pop_back();
    /**
     * @return the deleted element - undefined return value if there isn't any more elements in the list
     */
    T pop_front();
    /**
     * @return the deleted element - undefined return value if there isn't any more elements in the list
     */
    T erase(unsigned long _position);
    /**
     * @brief delete the whole list
     */
    void clear();

    /**
     * @brief same as size()
     */
    unsigned long length() const;
    /**
     * @brief same as length()
     */
    unsigned long size() const;

    T &operator[](unsigned long _index) { return this->at(_index); }
    const T &operator[](unsigned long _index) const { return this->at(_index); }
    T &at(unsigned long _index);
    const T &at(unsigned long _index) const;

    /**
     * @return reference to the first element in the list
     */
    DoublyListNode<T> &front() const { return *(this->_begin->next); }
    /**
     * @return reference to the last element in the list
     */
    DoublyListNode<T> &last() const { return *(this->_end->prev); }

    /**
     * @return an iterator to the first element in the list
     */
    DoublyListNodeIterator<T> begin() const { return DoublyListNodeIterator<T>(*(this->_begin->next)); }
    /**
     * @return one element past the last element in the list
     */
    DoublyListNodeIterator<T> end() const { return DoublyListNodeIterator<T>(*(this->_end)); }

private:
    DoublyListNode<T> *_end;
    DoublyListNode<T> *_begin;
    unsigned long _size;
};

/** @} */

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
        if (this->size() / 2 > _position) // if the element is in the first half of the list - begin from begin_ptr
        {
            // default value of the iterator is the begin...
            unsigned long counter = 0;
            while (counter < _position - 1) // hold on one element before position and insert there the element so the element will be element _positoin
            {
                ++iterator;
                ++counter;
            }
        }
        else
        {
            iterator = this->last();
            unsigned long e_pos_to_insert = this->size() - 1 - _position; // element pos to delete counting from back
            unsigned long counter = 0;
            while (counter < e_pos_to_insert + 1) // hold on one element before position and insert there the element so the element will be element _position
            {
                --iterator;
                ++counter;
            }
        }

        // the iterator stops on the element before the insert pos
        DoublyListNode<T> *element_to_insert = new DoublyListNode<T>(_data, iterator.node(), iterator.next());
        DoublyListNode<T> *element_after = iterator.next(); // element after the element we want to insert

        iterator.node()->next = element_to_insert; // set the element before
        element_after->prev = element_to_insert;   // set the prev pointer of the element after

        ++this->_size;
    }
}

template <typename T>
T DoublyLinkedList<T>::pop_back()
{
    assert(this->size() > 0);
    if (this->size() > 0)
    {
        DoublyListNode<T> *element_to_delete = &this->last();
        T data_backup = *(element_to_delete->data);

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
    assert(this->size() > 0);
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
            if (this->size() / 2 > _position) // if the element is in the first half of the list - begin from begin_ptr
            {
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
            }
            else // element is in the second half of the list - begin from back
            {
                DoublyListNodeIterator<T> iterator(this->last());         // one element before the element_to_delete
                unsigned long e_to_delete = this->size() - 1 - _position; // element pos to delete counting from back
                unsigned long counter = 0;
                while (counter < e_to_delete + 1) // holding one element before the element to delete
                {
                    --iterator;
                    ++counter;
                }
                element_to_delete = iterator.next();
                iterator.node()->next = element_to_delete->next; // the element before element_to_delete
                element_to_delete->next->prev = iterator.node(); // element after element_to_delete
            }
        }
        data_backup = *(element_to_delete->data);
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

    if (this->size() / 2 > _index) // if the element is in the first half of the list - begin from begin_ptr
    {
        DoublyListNodeIterator<T> iterator(this->begin());
        unsigned long counter = 0;
        while (counter < _index) // after this while the iterator is on the requested position
        {
            ++iterator;
            ++counter;
        }
        return iterator.data();
    }
    else // element is in the second half of the list - begin from back
    {
        DoublyListNodeIterator<T> iterator(this->last());
        unsigned long wantent_element = this->size() - 1 - _index; // element pos to return counting from back
        unsigned long counter = 0;
        while (counter < wantent_element)
        {
            --iterator;
            ++counter;
        }
        return *(iterator.node()->data);
    }
}

template <typename T>
const T &DoublyLinkedList<T>::at(unsigned long _index) const
{
    assert(_index < this->size());

    if (this->size() / 2 > _index) // if the element is in the first half of the list - begin from begin_ptr
    {
        DoublyListNodeIterator<T> iterator(this->begin());
        unsigned long counter = 0;
        while (counter < _index) // after this while the iterator is on the requested position
        {
            ++iterator;
            ++counter;
        }
        return iterator.data();
    }
    else // element is in the second half of the list - begin from back
    {
        DoublyListNodeIterator<T> iterator(this->last());
        unsigned long wantent_element = this->size() - 1 - _index; // element pos to return counting from back
        unsigned long counter = 0;
        while (counter < wantent_element)
        {
            --iterator;
            ++counter;
        }
        return *(iterator.node()->data);
    }
}

template <typename T>
long long DoublyLinkedList<T>::search(const T &_data) const
{
    for (unsigned long i = 0; i < this->size(); ++i)
    {
        if (this->at(i) == _data)
            return i;
    }
    return -1;
}
