#ifndef SJTU_LIST_HPP
#define SJTU_LIST_HPP

#include <cstddef>
#include <exception>

namespace sjtu {

class container_is_empty : public std::exception {
public:
    const char* what() const noexcept override {
        return "Container is empty";
    }
};

class index_out_of_bound : public std::exception {
public:
    const char* what() const noexcept override {
        return "Index out of bound";
    }
};

class invalid_iterator : public std::exception {
public:
    const char* what() const noexcept override {
        return "Invalid iterator";
    }
};

/**
 * a data container like std::list
 * allocate random memory addresses for data and they are doubly-linked in a
 * list.
 */
template <typename T> class list {
protected:
    struct node_base {
        node_base *prev, *next;
        node_base() : prev(this), next(this) {}
    };

    struct node : public node_base {
        T data;
        node(const T &val) : node_base(), data(val) {}
    };

    node_base head;
    size_t _size;

    /**
     * insert node cur before node pos
     * return the inserted node cur
     */
    node_base *insert(node_base *pos, node_base *cur) {
        cur->prev = pos->prev;
        cur->next = pos;
        pos->prev->next = cur;
        pos->prev = cur;
        _size++;
        return cur;
    }

    /**
     * remove node pos from list (no need to delete the node)
     * return the removed node pos
     */
    node_base *erase(node_base *pos) {
        pos->prev->next = pos->next;
        pos->next->prev = pos->prev;
        _size--;
        return pos;
    }

public:
    class const_iterator;
    class iterator {
        friend class list<T>;
    private:
        node_base *ptr;
        const list<T> *container;

    public:
        iterator(node_base *p = nullptr, const list<T> *c = nullptr) : ptr(p), container(c) {}
        iterator(const iterator &other) : ptr(other.ptr), container(other.container) {}

        iterator operator++(int) {
            if (ptr == &(container->head) && container->_size == 0) throw invalid_iterator(); // Should not happen if end()
            if (ptr == &(container->head)) throw invalid_iterator();
            iterator tmp = *this;
            ptr = ptr->next;
            return tmp;
        }
        iterator &operator++() {
            if (ptr == &(container->head)) throw invalid_iterator();
            ptr = ptr->next;
            return *this;
        }
        iterator operator--(int) {
            if (ptr->prev == &(container->head)) throw invalid_iterator();
            iterator tmp = *this;
            ptr = ptr->prev;
            return tmp;
        }
        iterator &operator--() {
            if (ptr->prev == &(container->head)) throw invalid_iterator();
            ptr = ptr->prev;
            return *this;
        }

        /**
         * TODO *it
         * throw std::exception if iterator is invalid
         */
        T &operator*() const {
            if (ptr == &(container->head) || ptr == nullptr) throw invalid_iterator();
            return static_cast<node*>(ptr)->data;
        }
        /**
         * TODO it->field
         * throw std::exception if iterator is invalid
         */
        T *operator->() const noexcept {
            if (ptr == &(container->head) || ptr == nullptr) return nullptr;
            return &(static_cast<node*>(ptr)->data);
        }

        /**
         * a operator to check whether two iterators are same (pointing to the same
         * memory).
         */
        bool operator==(const iterator &rhs) const {
            return ptr == rhs.ptr;
        }
        bool operator==(const const_iterator &rhs) const {
            return ptr == rhs.ptr;
        }
        
        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator &rhs) const {
            return ptr != rhs.ptr;
        }
        bool operator!=(const const_iterator &rhs) const {
            return ptr != rhs.ptr;
        }
    };

    /**
     * TODO
     * has same function as iterator, just for a const object.
     * should be able to construct from an iterator.
     */
    class const_iterator {
        friend class list<T>;
    private:
        const node_base *ptr;
        const list<T> *container;

    public:
        const_iterator(const node_base *p = nullptr, const list<T> *c = nullptr) : ptr(p), container(c) {}
        const_iterator(const const_iterator &other) : ptr(other.ptr), container(other.container) {}
        const_iterator(const iterator &other) : ptr(other.ptr), container(other.container) {}

        const_iterator operator++(int) {
            if (ptr == &(container->head)) throw invalid_iterator();
            const_iterator tmp = *this;
            ptr = ptr->next;
            return tmp;
        }
        const_iterator &operator++() {
            if (ptr == &(container->head)) throw invalid_iterator();
            ptr = ptr->next;
            return *this;
        }
        const_iterator operator--(int) {
            if (ptr->prev == &(container->head)) throw invalid_iterator();
            const_iterator tmp = *this;
            ptr = ptr->prev;
            return tmp;
        }
        const_iterator &operator--() {
            if (ptr->prev == &(container->head)) throw invalid_iterator();
            ptr = ptr->prev;
            return *this;
        }

        const T &operator*() const {
            if (ptr == &(container->head) || ptr == nullptr) throw invalid_iterator();
            return static_cast<const node*>(ptr)->data;
        }
        const T *operator->() const noexcept {
            if (ptr == &(container->head) || ptr == nullptr) return nullptr;
            return &(static_cast<const node*>(ptr)->data);
        }

        bool operator==(const iterator &rhs) const {
            return ptr == rhs.ptr;
        }
        bool operator==(const const_iterator &rhs) const {
            return ptr == rhs.ptr;
        }
        bool operator!=(const iterator &rhs) const {
            return ptr != rhs.ptr;
        }
        bool operator!=(const const_iterator &rhs) const {
            return ptr != rhs.ptr;
        }
    };

    /**
     * TODO Constructs
     * Atleast two: default constructor, copy constructor
     */
    list() : _size(0) {}
    list(const list &other) : _size(0) {
        for (const_iterator it = other.cbegin(); it != other.cend(); ++it) {
            push_back(*it);
        }
    }
    list(list &&other) noexcept : _size(other._size) {
        if (other._size > 0) {
            head.next = other.head.next;
            head.prev = other.head.prev;
            head.next->prev = &head;
            head.prev->next = &head;
            other.head.next = &other.head;
            other.head.prev = &other.head;
            other._size = 0;
        } else {
            head.next = &head;
            head.prev = &head;
        }
    }
    /**
     * TODO Destructor
     */
    ~list() {
        clear();
    }
    /**
     * TODO Assignment operator
     */
    list &operator=(const list &other) {
        if (this == &other) return *this;
        clear();
        for (const_iterator it = other.cbegin(); it != other.cend(); ++it) {
            push_back(*it);
        }
        return *this;
    }
    list &operator=(list &&other) noexcept {
        if (this == &other) return *this;
        clear();
        if (other._size > 0) {
            _size = other._size;
            head.next = other.head.next;
            head.prev = other.head.prev;
            head.next->prev = &head;
            head.prev->next = &head;
            other.head.next = &other.head;
            other.head.prev = &other.head;
            other._size = 0;
        }
        return *this;
    }
    /**
     * access the first / last element
     * throw container_is_empty when the container is empty.
     */
    const T &front() const {
        if (_size == 0) throw container_is_empty();
        return static_cast<node*>(head.next)->data;
    }
    const T &back() const {
        if (_size == 0) throw container_is_empty();
        return static_cast<node*>(head.prev)->data;
    }
    T &front() {
        if (_size == 0) throw container_is_empty();
        return static_cast<node*>(head.next)->data;
    }
    T &back() {
        if (_size == 0) throw container_is_empty();
        return static_cast<node*>(head.prev)->data;
    }
    /**
     * returns an iterator to the beginning.
     */
    iterator begin() {
        return iterator(head.next, this);
    }
    const_iterator begin() const {
        return const_iterator(head.next, this);
    }
    const_iterator cbegin() const {
        return const_iterator(head.next, this);
    }
    /**
     * returns an iterator to the end.
     */
    iterator end() {
        return iterator(&head, this);
    }
    const_iterator end() const {
        return const_iterator(&head, this);
    }
    const_iterator cend() const {
        return const_iterator(&head, this);
    }
    /**
     * checks whether the container is empty.
     */
    bool empty() const {
        return _size == 0;
    }
    /**
     * returns the number of elements
     */
    size_t size() const {
        return _size;
    }

    /**
     * clears the contents
     */
    void clear() {
        while (_size > 0) {
            pop_back();
        }
    }
    /**
     * insert value before pos (pos may be the end() iterator)
     * return an iterator pointing to the inserted value
     * throw if the iterator is invalid
     */
    iterator insert(iterator pos, const T &value) {
        if (pos.container != this) throw invalid_iterator();
        node *new_node = new node(value);
        insert(pos.ptr, new_node);
        return iterator(new_node, this);
    }
    /**
     * remove the element at pos (the end() iterator is invalid)
     * returns an iterator pointing to the following element, if pos pointing to
     * the last element, end() will be returned. throw if the container is empty,
     * the iterator is invalid
     */
    iterator erase(iterator pos) {
        if (pos.container != this || pos.ptr == &head) throw invalid_iterator();
        node_base *next_node = pos.ptr->next;
        erase(pos.ptr);
        delete static_cast<node*>(pos.ptr);
        return iterator(next_node, this);
    }
    /**
     * adds an element to the end
     */
    void push_back(const T &value) {
        insert(&head, new node(value));
    }
    /**
     * removes the last element
     * throw when the container is empty.
     */
    void pop_back() {
        if (_size == 0) throw container_is_empty();
        node_base *p = erase(head.prev);
        delete static_cast<node*>(p);
    }
    /**
     * adds an element to the beginning.
     */
    void push_front(const T &value) {
        insert(head.next, new node(value));
    }
    /**
     * removes the first element.
     * throw when the container is empty.
     */
    void pop_front() {
        if (_size == 0) throw container_is_empty();
        node_base *p = erase(head.next);
        delete static_cast<node*>(p);
    }

    /**
     * splice elements from another list
     */
    void splice(iterator pos, list &other) {
        if (this == &other) return;
        if (other.empty()) return;
        if (pos.container != this) throw invalid_iterator();

        node_base *first = other.head.next;
        node_base *last = other.head.prev;

        first->prev = pos.ptr->prev;
        last->next = pos.ptr;
        pos.ptr->prev->next = first;
        pos.ptr->prev = last;

        _size += other._size;
        other.head.next = &other.head;
        other.head.prev = &other.head;
        other._size = 0;
    }
};

} // namespace sjtu

#endif // SJTU_LIST_HPP