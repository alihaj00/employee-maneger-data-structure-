
#ifndef UNTITLED12_SORTEDLIST_H
#define UNTITLED12_SORTEDLIST_H

#include <iostream>
#include <cstring>
#include "library2.h"

#define ZERO 0

using std::string;
using std::ostream;

template<class T, class K>
class Node {
    Node *next;
    Node *prev;
    K key;
    bool last_node;
public:
    Node(T *data1, const K &key1);

    Node(const Node &new_node);

    ~Node();

    Node copy_node() {
        Node<T, K> *new_node = new Node<T, K>(this->get_data());
        return new_node;
    }

    const K &getKey() {
        return this->key;
    }

    void insert_next(Node *nextNode);

    void insert_prev(Node *prevNode);

    T *operator*();

    T *get_data();

    Node *get_next();

    Node *get_prev();

    bool get_last();

    void change_last(bool is_last) {
        this->last_node = is_last;
    }

    T *data;
};

template<class T, class K>
Node<T, K>::Node(const Node &new_node): data(new_node.data), last_node(new_node.last_node), key(new_node.key) {
    this->prev = new_node.prev;
    this->next = new_node.next;
}

template<class T, class K>
Node<T, K>::Node(T *data1, const K &key1) {
    this->next = nullptr;
    this->prev = nullptr;
    this->last_node = 0;
    this->data = data1;
    this->key = key1;
}

template<class T, class K>
Node<T, K>::~Node() = default;

template<class T, class K>
void Node<T, K>::insert_next(Node *nextNode) {
    this->next = nextNode;
}

template<class T, class K>
void Node<T, K>::insert_prev(Node *prevNode) {
    this->prev = prevNode;
}


template<class T, class K>
T *Node<T, K>::operator*() {
    return this->data;
}

template<class T, class K>
T *Node<T, K>::get_data() {
    return this->data;
}

template<class T, class K>
Node<T, K> *Node<T, K>::get_next() {
    return this->next;
}

template<class T, class K>
bool Node<T, K>::get_last() {
    return this->last_node;
}

template<class T, class K>
Node<T, K> *Node<T, K>::get_prev() {
    return this->prev;
}


template<class T, class K>
class List {

public:
    int list_size;
    Node<T, K> *first;
    Node<T, K> *last;

    int length();

    class const_iterator;

    List();

    List(const List &list);

    ~List();

    void clear();

    const List &copy();

    List &operator=(List list2);

    StatusType insert(T *new_data, const K &key);

    StatusType remove(const K &key);

    Node<T, K> *find(const K &key);

    const_iterator begin() {
        const_iterator begin(this->first, 0);
        return begin;
    }

    const_iterator end() {
        const_iterator end(this->last, this->list_size - 1);
        return end;
    }
};

template<class T, class K>
int List<T, K>::length() {
    return this->list_size;
}

template<class T, class K>
class List<T, K>::const_iterator {
    Node<T, K> *ptr;
    int index;
public:
    const_iterator(Node<T, K> *new_ptr, int index);


    const_iterator(const const_iterator &new_iter);

    const const_iterator copy_iter();

    ~const_iterator();

    Node<T, K> *get_ptr() {
        return this->ptr;
    }


    const_iterator &operator=(const_iterator new_iter);

    const_iterator &operator++();


    bool operator==(const const_iterator &iter) const;

    T *operator*();

    T *get_data();

    const K &get_key();
};

template<class T, class K>
List<T, K>::const_iterator::const_iterator(Node<T, K> *new_ptr, int index) {
    this->index = index;
    this->ptr = new_ptr;

}

template<class T, class K>
List<T, K>::const_iterator::const_iterator(const const_iterator &new_iter): index(new_iter.index) {
    this->ptr = new Node<T, K>(new_iter.ptr->get_data(), new_iter.ptr->getKey());

}

template<class T, class K>
const typename List<T, K>::const_iterator List<T, K>::const_iterator::copy_iter() {
    const_iterator new_iter = const_iterator(*this);
    return new_iter;
}

template<class T, class K>
List<T, K>::const_iterator::~const_iterator() = default;

template<class T, class K>
typename List<T, K>::const_iterator &List<T, K>::const_iterator::operator=(const_iterator new_iter) {
    this = new_iter.copy_iter();
    return *this;
}

template<class T, class K>
typename List<T, K>::const_iterator &List<T, K>::const_iterator::operator++() {

    this->ptr = (this->ptr->get_next());
    this->index++;
    return *this;
}

template<class T, class K>
bool List<T, K>::const_iterator::operator==(const const_iterator &iter) const {
    Node<T, K> *node1 = this->ptr;
    Node<T, K> *node2 = iter.ptr;
    if ((node1->getKey() == node2->getKey())) {
        return true;

    }
    return false;
}

template<class T, class K>
T *List<T, K>::const_iterator::operator*() {
    return this->get_data();
}

template<class T, class K>
T *List<T, K>::const_iterator::get_data() {
    return this->ptr->get_data();
}

template<class T, class K>
const K &List<T, K>::const_iterator::get_key() {
    return this->ptr->getKey();
}


template<class T, class K>
List<T, K>::List():last(nullptr), first(nullptr), list_size(0) {}


template<class T, class K>
List<T, K>::List(const List &list) {
    this->list_size = 0;

    Node<T, K> *ptr = list.first;

    while (ptr) {
        this->insert(ptr->get_data());
        ptr = ptr->get_next();
    }
}

template<class T, class K>
List<T, K>::~List() {
    if (this)
        this->clear();

}

template<class T, class K>
void List<T, K>::clear() {
    if (list_size > 0) {
        Node<T, K> *ptr = first;
        Node<T, K> *to_delete;
        while (ptr) {
            to_delete = ptr;
            ptr = ptr->get_next();
            delete to_delete;
        }
        first = nullptr;

        last = nullptr;
        list_size = 0;
    }
}

template<class T, class K>
const List<T, K> &List<T, K>::copy() {
    if (this->list_size == 0) {
        return this;
    }
    List new_list = new List(*this);
    return new_list;
}

template<class T, class K>
List<T, K> &List<T, K>::operator=(List list2) {
    this->clear();

    Node<T, K> *ptr = list2.first;
    while (ptr) {
        this->insert(ptr->get_data());
        ptr = ptr->get_next();
    }
    return *this;
}

template<class T, class K>
Node<T, K> *(List<T, K>::find)(const K &key) {
    if (this->first && list_size > 0) {
        Node<T, K> *current = this->first;
        if (current->getKey() == key) return current;
        while (current) {
            if (current->getKey() == key) return current;
            current = current->get_next();

        }
    }
    return nullptr;
}

template<class T, class K>
StatusType List<T, K>::insert(T *new_data, const K &key) {


    if (this->list_size == 0) {
        Node<T, K> *new_node;
        try {
            new_node = new Node<T, K>(new_data, key);
        }
        catch (std::bad_alloc &) {
            delete new_node;
            return ALLOCATION_ERROR;
        }
        this->first = new_node;
        this->last = new_node;
        new_node->change_last(1);
        new_node->insert_prev(nullptr);
        new_node->insert_next(nullptr);
        last = new_node;

        this->list_size++;
        return SUCCESS;
    } else {
        if (this->find(key)) return FAILURE;
        Node<T, K> *new_node;
        try {
            new_node = new Node<T, K>(new_data, key);
        }
        catch (std::bad_alloc &) {
            delete new_node;
            return ALLOCATION_ERROR;
        }
        this->first->insert_prev(new_node);
        new_node->insert_next(first);
        new_node->insert_prev(nullptr);
        this->first = new_node;
        list_size++;
        return SUCCESS;
    }

}


template<class T, class K>
StatusType List<T, K>::remove(const K &key) {
    auto to_remove = find(key);
    if (!to_remove) return SUCCESS;
    if (this->list_size == 1) {
        delete this->first;
        this->first = this->last = nullptr;
        list_size--;
        return SUCCESS;
    }

    if (first == to_remove) {
        first = to_remove->get_next();
        first->insert_prev(nullptr);
    } else if (last == to_remove) {
        last = to_remove->get_prev();
        last->insert_next(nullptr);
        last->change_last(1);
    } else {
        auto next = to_remove->get_next();
        auto prev = to_remove->get_prev();
        if (next)
            next->insert_prev(prev);
        else
            next->insert_prev(nullptr);
        if (prev)
            prev->insert_next(next);
        else
            prev->insert_next(nullptr);
    }

    list_size--;
    if (list_size == 0) {
        first == nullptr;
        last = nullptr;
    }

    delete to_remove;
    return SUCCESS;
}


#endif //UNTITLED12_SORTEDLIST_H
