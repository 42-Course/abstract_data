#ifndef FT_LIST_HPP
#define FT_LIST_HPP

#include <memory>
#include <cstddef>
#include "iterators/reverse_iterator.hpp"
#include "iterators/list_iterator.hpp"
#include "utils/list_node.hpp"
#include "utils/enable_if.hpp"
#include <limits>

namespace ft {

template <typename T, typename Alloc = std::allocator<T> >
class list {
public:
  typedef T                                value_type;
  typedef Alloc                            allocator_type;
  typedef typename Alloc::reference        reference;
  typedef typename Alloc::const_reference  const_reference;
  typedef typename Alloc::pointer          pointer;
  typedef typename Alloc::const_pointer    const_pointer;
  typedef std::ptrdiff_t                   difference_type;
  typedef std::size_t                      size_type;

  typedef ft::list_iterator<T, T>                  iterator;
  typedef ft::list_iterator<const T, T>            const_iterator;
  typedef ft::reverse_iterator<iterator>        reverse_iterator;
  typedef ft::reverse_iterator<const_iterator>  const_reverse_iterator;

private:
  typedef list_node<T>              node_type;
  typedef std::allocator<node_type> node_allocator_type;

  node_type*       _head;
  node_type*       _tail;
  size_type        _size;
  allocator_type   _alloc;
  node_allocator_type _node_alloc;

  node_type* create_node(const value_type& val) {
    node_type* node = _node_alloc.allocate(1);
    _node_alloc.construct(node, node_type(val));
    return node;
  }

  void destroy_node(node_type* node) {
    _node_alloc.destroy(node);
    _node_alloc.deallocate(node, 1);
  }

  void init_empty() {
    _head = create_node(value_type());
    _tail = create_node(value_type());
    _head->next = _tail;
    _tail->prev = _head;
    _size = 0;
  }

public:
  explicit list(const allocator_type& alloc = allocator_type())
    : _alloc(alloc) {
    init_empty();
  }

  list(size_type n, const value_type& val = value_type(),
       const allocator_type& alloc = allocator_type())
    : _alloc(alloc) {
    init_empty();
    insert(begin(), n, val);
  }

  template <class InputIterator>
  list(InputIterator first, InputIterator last,
       const allocator_type& alloc = allocator_type(),
       typename ft::enable_if<!std::numeric_limits<InputIterator>::is_specialized>::type* = 0)
    : _alloc(alloc) {
    init_empty();
    insert(begin(), first, last);
  }

  list(const list& other) : _alloc(other._alloc) {
    init_empty();
    insert(begin(), other.begin(), other.end());
  }

  ~list() {
    clear();
    destroy_node(_head);
    destroy_node(_tail);
  }

  list& operator=(const list& other) {
    if (this != &other) {
      clear();
      insert(begin(), other.begin(), other.end());
    }
    return *this;
  }

  // Iterators
  iterator begin() { return iterator(_head->next); }
  iterator end() { return iterator(_tail); }
  const_iterator begin() const { return const_iterator(_head->next); }
  const_iterator end() const { return const_iterator(_tail); }
  reverse_iterator rbegin() { return reverse_iterator(end()); }
  reverse_iterator rend() { return reverse_iterator(begin()); }
  const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
  const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

  // Capacity
  bool empty() const { return _size == 0; }
  size_type size() const { return _size; }
  size_type max_size() const { return _node_alloc.max_size(); }

  // Element access
  reference front() { return *begin(); }
  const_reference front() const { return *begin(); }
  reference back() { return *(--end()); }
  const_reference back() const { return *(--end()); }

  allocator_type get_allocator() const { return _alloc; }

  // Modifiers
  void clear() {
    node_type* curr = _head->next;
    while (curr != _tail) {
      node_type* next = curr->next;
      destroy_node(curr);
      curr = next;
    }
    _head->next = _tail;
    _tail->prev = _head;
    _size = 0;
  }

  void push_back(const value_type& val) { insert(end(), val); }
  void push_front(const value_type& val) { insert(begin(), val); }
  void pop_back() { if (_size > 0) erase(--end()); }
  void pop_front() { if (_size > 0) erase(begin()); }

  iterator insert(iterator pos, const value_type& val) {
    node_type* node = create_node(val);
    node_type* next = pos.base();
    node_type* prev = next->prev;
    node->next = next;
    node->prev = prev;
    prev->next = node;
    next->prev = node;
    ++_size;
    return iterator(node);
  }

  void insert(iterator pos, size_type n, const value_type& val) {
    for (size_type i = 0; i < n; ++i)
      insert(pos, val);
  }

  template <class InputIterator>
  void insert(iterator pos, InputIterator first, InputIterator last,
              typename ft::enable_if<!std::numeric_limits<InputIterator>::is_specialized>::type* = 0) {
    list tmp;
    for (; first != last; ++first)
      tmp.push_back(*first);
    splice(pos, tmp);
  }

  iterator erase(iterator pos) {
    node_type* node = pos.base();
    node_type* prev = node->prev;
    node_type* next = node->next;
    prev->next = next;
    next->prev = prev;
    iterator ret(next);
    destroy_node(node);
    --_size;
    return ret;
  }

  iterator erase(iterator first, iterator last) {
    while (first != last)
      first = erase(first);
    return last;
  }

  void resize(size_type n, value_type val = value_type()) {
    while (_size > n)
      pop_back();
    while (_size < n)
      push_back(val);
  }

  void swap(list& other) {
    std::swap(_head, other._head);
    std::swap(_tail, other._tail);
    std::swap(_size, other._size);
    std::swap(_alloc, other._alloc);
  }

  void assign(size_type n, const value_type& val) {
    clear();
    insert(begin(), n, val);
  }

  template <class InputIterator>
  void assign(InputIterator first, InputIterator last,
              typename ft::enable_if<!std::numeric_limits<InputIterator>::is_specialized>::type* = 0) {
    clear();
    insert(begin(), first, last);
  }

  // Operations
  void remove(const value_type& val) {
    iterator it = begin();
    while (it != end()) {
      if (*it == val)
        it = erase(it);
      else
        ++it;
    }
  }

  template <class Predicate>
  void remove_if(Predicate pred) {
    iterator it = begin();
    while (it != end()) {
      if (pred(*it))
        it = erase(it);
      else
        ++it;
    }
  }

  void unique() {
    if (_size < 2)
      return;
    iterator it = begin();
    iterator next = it;
    ++next;
    while (next != end()) {
      if (*it == *next)
        next = erase(next);
      else {
        it = next;
        ++next;
      }
    }
  }

  template <class BinaryPredicate>
  void unique(BinaryPredicate binary_pred) {
    if (_size < 2)
      return;

    iterator first = begin();
    iterator next = first;
    ++next;

    while (next != end()) {
      if (binary_pred(*first, *next)) {
        next = erase(next);
      } else {
        first = next;
        ++next;
      }
    }
  }

  void reverse() {
    if (_size < 2)
      return;
    node_type* curr = _head;
    while (curr) {
      std::swap(curr->next, curr->prev);
      curr = curr->prev;
    }
    std::swap(_head, _tail);
  }

  void merge(list& other) {
    iterator it1 = begin();
    iterator it2 = other.begin();
    while (it1 != end() && it2 != other.end()) {
      if (*it2 < *it1) {
        iterator next = it2;
        ++next;
        splice(it1, other, it2);
        it2 = next;
      } else {
        ++it1;
      }
    }
    if (it2 != other.end())
      splice(end(), other, it2, other.end());
  }

  template <class Compare>
  void merge(list& other, Compare comp) {
    if (this == &other)
      return;

    iterator it1 = begin();
    iterator it2 = other.begin();

    while (it1 != end() && it2 != other.end()) {
      if (comp(*it2, *it1)) {
        iterator next = it2;
        ++next;
        splice(it1, other, it2);
        it2 = next;
      } else {
        ++it1;
      }
    }
    if (it2 != other.end()) {
      splice(end(), other, it2, other.end());
    }
  }

  void sort() {
    if (_size < 2)
      return;
    for (iterator i = begin(); i != end(); ++i) {
      for (iterator j = i; j != begin(); --j) {
        iterator k = j;
        --k;
        if (*j < *k) {
          std::swap(*j, *k);
        } else {
          break;
        }
      }
    }
  }

  template <class Compare>
  void sort(Compare comp) {
    if (_size < 2)
      return;

    list carry;
    list counter[64];
    int fill = 0;

    while (!empty()) {
      carry.splice(carry.begin(), *this, begin());
      int i = 0;
      while (i < fill && !counter[i].empty()) {
        counter[i].merge(carry, comp);
        carry.swap(counter[i]);
        ++i;
      }
      carry.swap(counter[i]);
      if (i == fill)
        ++fill;
    }

    for (int i = 1; i < fill; ++i)
      counter[i].merge(counter[i - 1], comp);

    swap(counter[fill - 1]);
  }

  void splice(iterator pos, list& other) {
    if (this == &other || other.empty())
      return;
    splice(pos, other, other.begin(), other.end());
  }

  // Transfer single element from another list
  void splice(iterator pos, list& other, iterator it) {
    iterator next = it;
    ++next;
    if (pos == it || pos == next)
      return;
    splice(pos, other, it, next);
  }

  void splice(iterator pos, list& other, iterator first, iterator last) {
    if (first == last)
      return;

    size_type count = 0;
    for (iterator it = first; it != last; ++it)
      ++count;

    node_type* first_node = first.base();
    node_type* last_node = last.base();
    node_type* before = pos.base()->prev;
    node_type* after = pos.base();

    // Disconnect from 'other'
    first_node->prev->next = last_node;
    last_node->prev->next = after;
    after->prev->next = first_node;

    node_type* old_last = last_node->prev;
    last_node->prev = first_node->prev;
    first_node->prev = before;
    after->prev = old_last;

    _size += count;
    other._size -= count;
  }

};

// Non-member swap
template <class T, class Alloc>
void swap(list<T, Alloc>& x, list<T, Alloc>& y) {
  x.swap(y);
}

// Relational operators
template <class T, class Alloc>
bool operator==(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs) {
  if (lhs.size() != rhs.size()) return false;
  typename list<T, Alloc>::const_iterator lit = lhs.begin();
  typename list<T, Alloc>::const_iterator rit = rhs.begin();
  for (; lit != lhs.end(); ++lit, ++rit) {
    if (!(*lit == *rit))
      return false;
  }
  return true;
}

template <class T, class Alloc>
bool operator!=(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs) {
  return !(lhs == rhs);
}

// Relational
template <class T, class Alloc>
bool operator<(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs) {
  return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <class T, class Alloc>
bool operator<=(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs) {
  return !(rhs < lhs);
}

template <class T, class Alloc>
bool operator>(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs) {
  return rhs < lhs;
}

template <class T, class Alloc>
bool operator>=(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs) {
  return !(lhs < rhs);
}


} // namespace ft

#endif // FT_LIST_HPP
