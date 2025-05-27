#ifndef LIST_NODE_HPP
#define LIST_NODE_HPP

namespace ft {

template <typename T>
struct list_node {
  T value;
  list_node* prev;
  list_node* next;

  list_node(const T& val = T()) : value(val), prev(NULL), next(NULL) {}
};

} // namespace ft

#endif // LIST_NODE_HPP
