#pragma once
#include <assert.h>

#include "SinglyListNode.inl"
#include "SinglyListNodeIterator.inl"

/*
    Simple linked list - linked only in one direction

    works for every element which has implemented following methods:
        - standard constructor(T())
        - copy constructor
        - compare-operator - operator==()
        - bigger than operator - operator>()
*/

/**
 *  \defgroup SinglyLinkedList
 * \ingroup LinkedList
 */
template <typename T>
class SinglyLinkedList
{
    // documentating the template parameter
    /**
     * @tparam T type of which the Linked List should be.
     */

public:
    /**
     * @brief create an empty list
     */
    SinglyLinkedList();
    /**
     * @param _size size of the list
     * @param _standard_initializer initialize the whole list with this value
     */
    SinglyLinkedList(unsigned long _size, T _standard_initializer);
    ~SinglyLinkedList();

    void push_front(T _data);
    void push_back(T _data);

    /*
        reverse the whole list - first element will be last and last will be first
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
     * @return an iterator to the first element in the list
     */
    SinglyListNodeIterator<T> begin() { return SinglyListNodeIterator<T>(this->_begin->next); }
    /**
     * @return one element past the last element in the list
     */
    SinglyListNodeIterator<T> end() { return SinglyListNodeIterator<T>(this->_end); }

    /**
     * @return reference to the first element in the list
     */
    SinglyListNode<T> &front() const { return *(this->_begin->next); }
    /**
     * @return reference to the last element in the list
     */
    SinglyListNode<T> &last() const { return *(this->_last); }

private:
    // SinglyListNodeBase<T> _last; // the element which will be returned in a iterator at calling end(element past the last element of data)
    SinglyListNode<T> *_end;
    SinglyListNode<T> *_begin;
    SinglyListNode<T> *_last = nullptr; // last element in the list for the user. nullptr if no element in list
    unsigned long _size = 0;
};

// ---------------- Implementations ----------------

template <typename T>
SinglyLinkedList<T>::SinglyLinkedList() : _end(new SinglyListNode<T>()), _begin(new SinglyListNode<T>(_end)), _size(0)
{
}

template <typename T>
SinglyLinkedList<T>::SinglyLinkedList(unsigned long size, T _standard_initializer) : _end(new SinglyListNode<T>()), _begin(new SinglyListNode<T>(_end)), _size(0)
{
    for (unsigned long i = 0; i < size; i++)
    {
        this->push_front(_standard_initializer);
    }
}

template <typename T>
SinglyLinkedList<T>::~SinglyLinkedList()
{
    this->clear();
    delete this->_begin;
    delete this->_end;
}

template <typename T>
void SinglyLinkedList<T>::push_back(T _data)
{
    if (this->begin() == this->end()) // no element in the list
    {
        this->_begin->next = new SinglyListNode<T>(_data, this->_end);
        this->_last = this->_begin->next;
    }
    else // at least one element in the list
    {
        SinglyListNode<T> *tmp = new SinglyListNode<T>(_data, this->_end);
        this->_last->next = tmp;
        this->_last = tmp;
    }

    /*
    if (this->begin() == this->end()) // no element in the list...
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
    */

    ++_size;
}

template <typename T>
void SinglyLinkedList<T>::push_front(T _data)
{
    SinglyListNode<T> *tmp = new SinglyListNode<T>(_data, this->_begin->next);
    this->_begin->next = tmp;
    ++_size;
    if (this->_size == 1)
    {
        this->_last = &(this->front());
    }
}

template <typename T>
void SinglyLinkedList<T>::insert(unsigned long _position, T _data)
{
    assert(_position <= this->size());
    if (this->size() == 0 || _position == 0)
    {
        this->push_front(_data);
    }
    else if (_position == this->size()) // if the element should be inserted after the actual last element
    {
        // the push_back is more optimized than to iterate over every element to find the last element
        this->push_back(_data);
    }
    else // at least one element in the list
    {
        SinglyListNodeIterator<T> iterator(this->front());
        unsigned long counter = 0;
        while (counter < _position - 1) // hold on one element before position and insert there the element so the element will be element no _positoin
        {
            ++iterator;
            ++counter;
        }
        SinglyListNode<T> *tmp = new SinglyListNode<T>(_data, iterator.next());
        iterator.node()->next = tmp; // set the element before
        ++this->_size;
    }
}

template <typename T>
T SinglyLinkedList<T>::pop_back()
{
    if (this->size() > 0)
    {
        SinglyListNode<T> *element_to_delete = &(this->last());
        T data_backup = *element_to_delete->data;
        // finding one element before _end
        SinglyListNodeIterator<T> it(this->_begin);

        while (it.next() != element_to_delete)
            ++it;
        it.node()->next = this->_end; // setting next poniter from the element before _end
        this->_last = it.node();

        delete element_to_delete;
        --this->_size;
        return data_backup;
    }

    if (this->size() == 0)
    {
        this->_last = nullptr;
    }
    // return;
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
            element_to_delete = &(this->front());
            this->_begin->next = element_to_delete->next;
        }
        else
        {
            SinglyListNodeIterator<T> it(this->front()); // one element before the element_to_delete
            unsigned long counter = 0;
            while (counter < (_position - 1)) // holding one element before the element to delete
            {
                ++it;
                ++counter;
            }
            element_to_delete = it.next();
            it.node()->next = element_to_delete->next; // hoping over the element_to_delete
        }

        data_backup = *(element_to_delete->data);
        delete element_to_delete;
        --this->_size;
        return data_backup;
    }
    // return;
}

template <typename T>
void SinglyLinkedList<T>::clear()
{
    while (this->length() > 0)
    {
        this->pop_front();
    }
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
    SinglyListNodeIterator<T> iterator = this->front();
    unsigned long counter = 0;
    while (counter < _index) // after this while the iterator is on the requested position
    {
        ++iterator;
        ++counter;
    }
    return iterator.data();
}

template <typename T>
const T &SinglyLinkedList<T>::at(unsigned long _index) const
{
    assert(_index < this->size());
    SinglyListNodeIterator<T> iterator = this->front();
    unsigned long counter = 0;
    while (counter < _index) // after this while the iterator is on the requested position
    {
        ++iterator;
        ++counter;
    }
    return iterator.data();
}

template <typename T>
long long SinglyLinkedList<T>::search(const T &_data) const
{
    for (unsigned long i = 0; i < this->size(); ++i)
    {
        if (this->at(i) == _data)
            return i;
    }
    return -1;
}

/*
template <typename T>
void SinglyLinkedList<T>::reverse()
{
    for (uint16_t i = 0; i < this->size(); ++i)
    {
        SinglyListNode<T> &n1 = this->at(i);
        SinglyListNode<T> &n2 = this->at(this->size() - 1 - i); // the node which should be swapped with n1 - from the back

        SinglyListNode<T> *tmp = n1.next;
        n1.next = n2.next;
        n2.next = tmp->next;
    }
}
*/

/*
template <typename T>
void SinglyLinkedList<T>::sort()
{
    if (this->size() > 1)
    {
        bool list_sorted = false;
        while (list_sorted == false)
        {
            for (unsigned short i = 0; i < this->size(); ++i)
            {
                if (this->at(i) > this->at(i + 1))
                {
                    T tmp = this->at(i);
                    this->operator[](i) = this->at(i + 1);
                    this->operator[](i + 1) = tmp;
                }
            }
            list_sorted = true;
            for (unsigned short i = 0; i < this->size(); ++i)
            {
                if (this->at(i) > this->at(i + 1))
                {
                    list_sorted = false;
                    break;
                }
            }
        }
    }
}
*/