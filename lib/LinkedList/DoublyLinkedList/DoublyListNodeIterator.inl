#pragma once

/**
 * \defgroup DoublyListNodeIterator
 * \ingroup DoublyLinkedList
 *
 * @{
 */
template <typename T>
class DoublyListNodeIterator
{
    // documentating the template parameter
    /**
     * @tparam T type of which the ListNodeIterator should be.
     */

public:
    DoublyListNodeIterator(const DoublyListNode<T> *_ln) : _node(const_cast<DoublyListNode<T> *>(_ln)) {}
    DoublyListNodeIterator(const DoublyListNode<T> &_ln) : _node(const_cast<DoublyListNode<T> *>(&_ln)) {}

    DoublyListNodeIterator(const DoublyListNodeIterator<T> &_it) : _node(_it.node()) {}
    DoublyListNodeIterator<T> operator=(const DoublyListNodeIterator<T> &_it) { this->_node = _it.node(); }

    /**
     * @brief post increment-operator(a++) - return the current data - before the switch
     */
    DoublyListNode<T> *operator++(int)
    {
        this->_node = this->_node->next;
        return this->_node->prev;
    }

    /**
     * @brief pre increment-operator(++a) - return the data after the switch
     */
    DoublyListNode<T> &operator++()
    {
        this->_node = this->_node->next;
        return *(this->_node);
    }

    /**
     * @brief post decrement-operator(a--) - return the current data - before the switch
     */
    DoublyListNode<T> *operator--(int)
    {
        this->_node = this->_node->prev;
        return this->_node->next;
    }

    /**
     * @brief pre decrement-operator(--a) - return the data after the switch
     */
    DoublyListNode<T> &operator--()
    {
        this->_node = this->_node->prev;
        return *(this->_node);
    }

    /**
     * @brief consider the data and the prev and next pointer
     */
    bool operator==(const DoublyListNodeIterator<T> &_list_iterator) const
    {
        return this->_node == _list_iterator.node() &&
               this->prev() == _list_iterator.prev() &&
               this->next() == _list_iterator.next();
    }
    /**
     * @brief consider the data and the prev and next pointer
     */
    bool operator!=(const DoublyListNodeIterator<T> &_list_iterator) const { return !(*this == _list_iterator); }

    T &data() const { return *(this->_node->data); }
    T &operator->() const { return this->_node->data; }
    T *operator*() const { return &this->_node->data; }
    DoublyListNode<T> *next() const { return this->_node->next; }
    DoublyListNode<T> *prev() const { return this->_node->prev; }
    DoublyListNode<T> *node() const { return this->_node; }

private:
    DoublyListNode<T> *_node;
};

/** @} */
