#pragma once

template <typename T>
T &&move(T &&_para)
{
    return static_cast<T &&>(_para);
}

/**
 * \defgroup SinglyListNode
 * \ingroup SinglyLinkedList
 *
 * @brief ListNode for simple singly linked list - one direction linked
 *
 * @{
 */
template <typename T>
class SinglyListNode
{
    // documentating the template parameter
    /**
     * @tparam T type of which the ListNode should be.
     */

public:
    /**
     * @brief create a node which doesnt has a value assigned - value can not be changed afterwards!!
     * Its for the begin and end pointer in the linked list
     */
    SinglyListNode() : data(nullptr), next(nullptr) {}

    /**
     * @param data data to initialize the node with.
     * @param _next pointer to the next element.
     */
    SinglyListNode(T &_data, SinglyListNode<T> *_next);

    /**
     * @param _next pointer to the next element.
     */
    SinglyListNode(SinglyListNode<T> *_next) : data(nullptr), next(_next) {}

    /**
     * @brief _data will be moved inside the node - will be undefined after calling this constructor
     */
    // SinglyListNode(T &&_data, SinglyListNode<T> *_next);

    ~SinglyListNode();

    /**
     * @return a reference to the data which can be modified.
     */
    T &operator*() const { return *this->data; }

    /**
     * @brief consider the next pointer and the data
     */
    bool operator==(const SinglyListNode<T> &_node) const { return (this->data == _node.data && this->next == _node.next); }
    /**
     * @brief consider the next pointer and the data
     */
    bool operator!=(const SinglyListNode<T> &_node) const { return !(*this == _node); }

    T *data;
    SinglyListNode<T> *next;
};

/** @} */

/*
template <typename T>
SinglyListNode<T>::SinglyListNode(T &&_data, SinglyListNode<T> *_next) : next(_next)
{
    *data = move(_data);
}
*/

template <typename T>
SinglyListNode<T>::SinglyListNode(T &_data, SinglyListNode<T> *_next) : data(new T(_data)), next(_next) {}

template <typename T>
SinglyListNode<T>::~SinglyListNode()
{
    if (this->data != nullptr)
    {
        delete data;
    }
}
