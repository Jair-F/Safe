#pragma once

/**
 * \defgroup DoublyListNode
 * \ingroup DoublyLinkedList
 *
 * @{
 */
template <typename T>
class DoublyListNode
{
    // documentating the template parameter
    /**
     * @tparam T type of which the ListNode should be.
     */

public:
    DoublyListNode() : next(nullptr), prev(nullptr) {}
    /**
     * @param _prev pointer to the previous element.
     * @param _next pointer to the next element.
     */
    DoublyListNode(DoublyListNode<T> *_prev, DoublyListNode<T> *_next) : next(_next), prev(_prev) {}
    /**
     * @param _data data to initialize the node with.
     * @param _prev pointer to the previous element.
     * @param _next pointer to the next element.
     */
    DoublyListNode(T _data, DoublyListNode<T> *_prev, DoublyListNode<T> *_next) : data(_data), next(_next), prev(_prev) {}
    ~DoublyListNode() {}

    /**
     * @return a reference to the data which can be modified.
     */
    T &operator*() const { return data; }

    /**
     * @brief consider the next,prev pointer and the data
     */
    bool operator==(const DoublyListNode<T> &_node) const
    {
        return _node.next == this->next &&
               _node.prev == this->prev &&
               _node.data == this->data;
    }
    /**
     * @brief consider the next,prev pointer and the data
     */
    bool operator!=(const DoublyListNode<T> &_node) const { return !(_node == *this); }

    T data;
    DoublyListNode<T> *next, *prev;
};

/** @} */