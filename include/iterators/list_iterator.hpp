#ifndef LIST_ITERATOR_HPP
#define LIST_ITERATOR_HPP

#include <iterator>
#include <cstddef>
#include "utils/list_node.hpp"
#include "utils/is_convertible.hpp"

namespace ft {

template <typename T, typename NonConstT>
class list_iterator {
public:
  typedef T                                value_type;
  typedef value_type&                      reference;
  typedef value_type*                      pointer;
  typedef std::ptrdiff_t                   difference_type;
  typedef std::bidirectional_iterator_tag  iterator_category;

  typedef list_node<NonConstT>             node_type;
  typedef list_iterator<T, NonConstT>      self_type;

private:
  node_type* _node;

public:
  // Constructors
  list_iterator() : _node(NULL) {}
  explicit list_iterator(node_type* n) : _node(n) {}
  list_iterator(const self_type& other) : _node(other._node) {}

  // Conversion from iterator<U> to iterator<T>
  template <typename U>
  list_iterator(const list_iterator<U, NonConstT>& other,
                typename ft::enable_if<ft::is_convertible<U*, T*>::value>::type* = 0)
    : _node(other.base()) {}

  // Assignment
  self_type& operator=(const self_type& other) {
    if (this != &other)
      _node = other._node;
    return *this;
  }

  // Dereference
  reference operator*() const { return _node->value; }
  pointer operator->() const { return &_node->value; }

  // Navigation
  self_type& operator++() { _node = _node->next; return *this; }
  self_type operator++(int) { self_type tmp(*this); ++(*this); return tmp; }
  self_type& operator--() { _node = _node->prev; return *this; }
  self_type operator--(int) { self_type tmp(*this); --(*this); return tmp; }

  // Comparison
  template <typename U>
  bool operator==(const list_iterator<U, NonConstT>& rhs) const {
    return _node == rhs.base();
  }

  template <typename U>
  bool operator!=(const list_iterator<U, NonConstT>& rhs) const {
    return _node != rhs.base();
  }

  node_type* base() const { return _node; }
};

} // namespace ft

#endif // LIST_ITERATOR_HPP
