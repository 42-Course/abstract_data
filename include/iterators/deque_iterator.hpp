#ifndef FT_DEQUE_ITERATOR_HPP
#define FT_DEQUE_ITERATOR_HPP

#include <iterator>
#include <cstddef>

namespace ft {

template <typename T, typename Pointer, typename Reference>
class deque_iterator {
public:
  typedef T                               value_type;
  typedef Pointer                         pointer;
  typedef Reference                       reference;
  typedef std::ptrdiff_t                  difference_type;
  typedef std::random_access_iterator_tag iterator_category;

  typedef deque_iterator<T, T*, T&>             iterator;
  typedef deque_iterator<T, const T*, const T&> const_iterator;

  T**             _map;        // pointer to array of block pointers
  size_t          _block_size; // number of elements per block
  size_t          _block_index; // index of the current block in _map
  size_t          _element_index; // index within the current block

  // Constructors
  deque_iterator() : _map(NULL), _block_size(0), _block_index(0), _element_index(0) {}
  deque_iterator(T** map, size_t block_size, size_t block_idx, size_t elem_idx)
    : _map(map), _block_size(block_size), _block_index(block_idx), _element_index(elem_idx) {}

  // Convert to const_iterator
  operator const_iterator() const {
    return const_iterator(_map, _block_size, _block_index, _element_index);
  }

  reference operator*() const {
    return _map[_block_index][_element_index];
  }

  pointer operator->() const {
    return &_map[_block_index][_element_index];
  }

  // Index access
  reference operator[](difference_type n) const {
    deque_iterator tmp = *this + n;
    return *tmp;
  }

  // Increment
  deque_iterator& operator++() {
    if (++_element_index == _block_size) {
      ++_block_index;
      _element_index = 0;
    }
    return *this;
  }

  deque_iterator operator++(int) {
    deque_iterator tmp = *this;
    ++(*this);
    return tmp;
  }

  // Decrement
  deque_iterator& operator--() {
    if (_element_index == 0) {
      --_block_index;
      _element_index = _block_size - 1;
    } else {
      --_element_index;
    }
    return *this;
  }

  deque_iterator operator--(int) {
    deque_iterator tmp = *this;
    --(*this);
    return tmp;
  }

  // Arithmetic
  deque_iterator operator+(difference_type n) const {
    difference_type offset = static_cast<difference_type>(_block_index * _block_size + _element_index) + n;
    size_t new_block = offset / _block_size;
    size_t new_elem = offset % _block_size;
    return deque_iterator(_map, _block_size, new_block, new_elem);
  }

  deque_iterator operator-(difference_type n) const {
    return *this + (-n);
  }

  difference_type operator-(const deque_iterator& other) const {
    return static_cast<difference_type>(
      (_block_index * _block_size + _element_index) -
      (other._block_index * _block_size + other._element_index));
  }

  deque_iterator& operator+=(difference_type n) {
    *this = *this + n;
    return *this;
  }

  deque_iterator& operator-=(difference_type n) {
    *this = *this - n;
    return *this;
  }

  // Comparison
  bool operator==(const deque_iterator& rhs) const {
    return _map == rhs._map &&
           _block_index == rhs._block_index &&
           _element_index == rhs._element_index;
  }

  bool operator!=(const deque_iterator& rhs) const { return !(*this == rhs); }
  bool operator<(const deque_iterator& rhs) const { return (*this - rhs) < 0; }
  bool operator<=(const deque_iterator& rhs) const { return !(*this > rhs); }
  bool operator>(const deque_iterator& rhs) const { return (*this - rhs) > 0; }
  bool operator>=(const deque_iterator& rhs) const { return !(*this < rhs); }
};

} // namespace ft

#endif // FT_DEQUE_ITERATOR_HPP
