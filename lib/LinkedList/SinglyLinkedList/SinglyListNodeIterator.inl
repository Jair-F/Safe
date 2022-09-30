#pragma once

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

    // consider the data and the next pointer
    bool operator==(const SinglyListNodeIterator<T> &_list_iterator) const { return (this->_node == _list_iterator.node()); }
    bool operator!=(const SinglyListNodeIterator<T> &_list_iterator) const { return !(*this == _list_iterator); }

    T &data() const { return *(this->_node->data); }
    T &operator->() const { return *(this->_node->data); }
    T *operator*() const { return &(*(this->_node->data)); }
    SinglyListNode<T> *next() const { return this->_node->next; }
    SinglyListNode<T> *node() const { return _node; }

private:
    SinglyListNode<T> *_node;
};
