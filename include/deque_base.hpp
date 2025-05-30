#ifndef FT_DEQUE_BASE_HPP
#define FT_DEQUE_BASE_HPP

#include <memory>
#include <cstddef>
#include <algorithm>

namespace ft {

template <typename T, typename Alloc>
class DequeBase {
protected:
  static const size_t BLOCK_SIZE;
  static const size_t INITIAL_MAP_SIZE;

  typedef typename Alloc::template rebind<T*>::other MapAllocator;
  typedef typename Alloc::template rebind<T>::other  NodeAllocator;

  T**              _map;
  size_t           _map_capacity;
  T**              _map_start;
  T**              _map_end;

  MapAllocator     _map_allocator;
  NodeAllocator    _node_allocator;
  Alloc            _allocator;

public:
  typedef Alloc allocator_type;

  DequeBase()
    : _map(0), _map_capacity(0),
      _map_start(0), _map_end(0),
      _map_allocator(), _node_allocator(), _allocator() {}

  explicit DequeBase(const allocator_type& alloc)
    : _map(0), _map_capacity(0),
      _map_start(0), _map_end(0),
      _map_allocator(alloc), _node_allocator(alloc), _allocator(alloc) {}

  DequeBase(const allocator_type& alloc, size_t num_elements)
    : _map(0), _map_capacity(0),
      _map_start(0), _map_end(0),
      _map_allocator(alloc), _node_allocator(alloc), _allocator(alloc) {
    initialize_map(num_elements);
  }

  ~DequeBase() {
    if (_map) {
      destroy_nodes(_map_start, _map_end);
      deallocate_map(_map, _map_capacity);
    }
  }

  allocator_type get_allocator() const { return _allocator; }

protected:
  T** allocate_map(size_t n) {
    return _map_allocator.allocate(n);
  }

  void deallocate_map(T** p, size_t n) {
    if (p)
      _map_allocator.deallocate(p, n);
  }

  T* allocate_node() {
    return _node_allocator.allocate(BLOCK_SIZE);
  }

  void deallocate_node(T* p) {
    if (p)
      _node_allocator.deallocate(p, BLOCK_SIZE);
  }

  void create_nodes(T** begin, T** end) {
    for (T** curr = begin; curr != end; ++curr)
      *curr = allocate_node();
  }

  void destroy_nodes(T** begin, T** end) {
    for (T** curr = begin; curr != end; ++curr)
      if (*curr)
        deallocate_node(*curr);
  }

  void initialize_map(size_t num_elements) {
    size_t num_blocks = num_elements / BLOCK_SIZE + 1;
    _map_capacity = std::max(INITIAL_MAP_SIZE, num_blocks + 2);
    _map = allocate_map(_map_capacity);
    T** start = _map + ((_map_capacity - num_blocks) / 2);
    T** finish = start + num_blocks;
    create_nodes(start, finish);

    _map_start = start;
    _map_end = finish;
  }
};

// Static member definitions (must be outside the class)
template <typename T, typename Alloc>
const size_t DequeBase<T, Alloc>::BLOCK_SIZE = 64;

template <typename T, typename Alloc>
const size_t DequeBase<T, Alloc>::INITIAL_MAP_SIZE = 8;

} // namespace ft

#endif // FT_DEQUE_BASE_HPP
