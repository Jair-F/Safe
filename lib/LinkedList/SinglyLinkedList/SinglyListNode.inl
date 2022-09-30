#pragma once

template <typename T>
T &&move(T &&_para)
{
    return static_cast<T &&>(_para);
}

/*
    ListNode for simple singly linked list - one direction linked
*/
template <typename T>
class SinglyListNode
{
public:
    SinglyListNode() : data(nullptr), next(nullptr) {}
    SinglyListNode(T &_data, SinglyListNode<T> *_next);
    /**
     * @brief _data will be moved inside the node - will be undefined after calling this constructor
     */
    // SinglyListNode(T &&_data, SinglyListNode<T> *_next);

    SinglyListNode(SinglyListNode<T> *_next) : data(nullptr), next(_next) {}

    ~SinglyListNode();
    T &operator*() const { return *this->data; }

    /*
        consider the next pointer and the data...
    */
    bool operator==(const SinglyListNode<T> &_node) const { return (this->data == _node.data && this->next == _node.next); }
    bool operator!=(const SinglyListNode<T> &_node) const { return !(*this == _node); }

    T *data;
    SinglyListNode<T> *next;
};

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
