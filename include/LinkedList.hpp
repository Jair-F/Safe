#pragma once

/*
    ListNode for simple singly linked list - one direction linked
*/
template <typename T>
class SinglyListNode
{
public:
    SinglyListNode(T &_data, SinglyListNode<T> *_next) : data(_data), next(_next) {}
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
    bool operator==(const SinglyListNodeIterator &_list_iterator) const { return this->_node->data() == _list_iterator.data(); }
    bool operator!=(const SinglyListNodeIterator &_list_iterator) const { return !(this->_node == _list_iterator); }

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
    SinglyLinkedList(unsigned long _size, T &_standard_initializer);
    ~SinglyLinkedList();

    void push_front(T &_data);
    void push_back(T &_data);

    /*
        insert _data on the _position and push the data that is now on this positon
        one back
    */
    void insert(unsigned long _position, T &_data);

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
