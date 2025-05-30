// https://www.youtube.com/watch?v=g9wTzeq0Y-g
// https://gcc.gnu.org/onlinedocs/libstdc++/libstdc++-html-USERS-4.3/a00704.html

#ifndef FT_DEQUE_HPP
#define FT_DEQUE_HPP

#include <memory>
#include <cstddef>
#include <limits>
#include <algorithm>
#include "deque_base.hpp"
#include "iterators/deque_iterator.hpp"
#include "utils/enable_if.hpp"

namespace ft {

template <typename T, typename Alloc = std::allocator<T> >
class deque : private DequeBase<T, Alloc> {
private:
  typedef DequeBase<T, Alloc> base;

public:
  typedef T                                     value_type;
  typedef Alloc                                 allocator_type;
  typedef typename allocator_type::reference         reference;
  typedef typename allocator_type::const_reference   const_reference;
  typedef typename allocator_type::pointer           pointer;
  typedef typename allocator_type::const_pointer     const_pointer;
  typedef std::size_t                          size_type;
  typedef std::ptrdiff_t                       difference_type;

  typedef ft::deque_iterator<T, T*, T&>              iterator;
  typedef ft::deque_iterator<T, const T*, const T&>  const_iterator;
  typedef std::reverse_iterator<iterator>            reverse_iterator;
  typedef std::reverse_iterator<const_iterator>      const_reverse_iterator;

protected:
  using base::BLOCK_SIZE;
  using base::_map;
  using base::_map_capacity;
  using base::_allocator;
  using base::allocate_node;
  using base::deallocate_node;
  using base::allocate_map;
  using base::deallocate_map;
  using base::create_nodes;
  using base::destroy_nodes;
  using base::initialize_map;
  using base::get_allocator;

  iterator begin_;
  iterator end_;

private:
  void initialize_storage(size_type count, const value_type& value) {
    for (size_type i = 0; i < count; ++i)
      push_back(value);
  }

  template <class InputIterator>
  void assign_range(InputIterator first, InputIterator last) {
    for (; first != last; ++first)
      push_back(*first);
  }

public:
  // Constructors
  deque()
    : base(), begin_(), end_() {}

  explicit deque(const allocator_type& alloc)
    : base(alloc), begin_(), end_() {}

  deque(size_type count, const value_type& value = value_type(),
        const allocator_type& alloc = allocator_type())
    : base(alloc, count) {
    size_t num_blocks = count / BLOCK_SIZE + 1;
    size_t start_block = (_map_capacity - num_blocks) / 2;
    begin_ = iterator(_map, BLOCK_SIZE, start_block, 0);
    end_ = begin_;
    initialize_storage(count, value);
  }


  template <class InputIterator>
  deque(InputIterator first, InputIterator last,
        const allocator_type& alloc = allocator_type(),
        typename ft::enable_if<!std::numeric_limits<InputIterator>::is_specialized>::type* = 0)
    : base(alloc) {
    assign_range(first, last);
  }

  deque(const deque& other)
    : base(other.get_allocator()) {
    assign_range(other.begin(), other.end());
  }

  ~deque() {
    clear();
  }

  deque& operator=(const deque& other) {
    if (this != &other) {
      clear();
      assign_range(other.begin(), other.end());
    }
    return *this;
  }

  // Basic iterator accessors
  iterator begin() { return begin_; }
  iterator end() { return end_; }
  const_iterator begin() const { return begin_; }
  const_iterator end() const { return end_; }
  reverse_iterator rbegin() { return reverse_iterator(end_); }
  reverse_iterator rend() { return reverse_iterator(begin_); }
  const_reverse_iterator rbegin() const { return const_reverse_iterator(end_); }
  const_reverse_iterator rend() const { return const_reverse_iterator(begin_); }

  // Capacity
  bool empty() const { return begin_ == end_; }
  size_type size() const { return end_ - begin_; }
  size_type max_size() const { return std::numeric_limits<difference_type>::max(); }


  // Element access
  reference operator[](size_type n) {
    return *(begin_ + n);
  }

  const_reference operator[](size_type n) const {
    return *(begin_ + n);
  }

  reference at(size_type n) {
    if (n >= size())
      throw std::out_of_range("ft::deque::at");
    return (*this)[n];
  }

  const_reference at(size_type n) const {
    if (n >= size())
      throw std::out_of_range("ft::deque::at");
    return (*this)[n];
  }

  reference front() {
    return *begin_;
  }

  const_reference front() const {
    return *begin_;
  }

  reference back() {
    return *(end_ - 1);
  }

  const_reference back() const {
    return *(end_ - 1);
  }


  // Modifiers
  void clear() {
    for (iterator it = begin_; it != end_; ++it)
      _allocator.destroy(&*it);

    size_t center = _map_capacity / 2;
    begin_ = end_ = iterator(_map, BLOCK_SIZE, center, 0);
  }

  void push_back(const value_type& value) {
    if (end_._element_index == BLOCK_SIZE) {
      // We need to move to a new block
      if (end_._block_index + 1 >= _map_capacity) {
        throw std::length_error("ft::deque push_back: map full");
        // Optionally: grow the map here in future
      }
      if (_map[end_._block_index + 1] == 0)
        _map[end_._block_index + 1] = allocate_node();

      ++end_._block_index;
      end_._element_index = 0;
    }

    _allocator.construct(&(*end_), value);
    ++end_;
  }

};

} // namespace ft

#endif // FT_DEQUE_HPP



// template <class T, class Allocator = std::allocator<T> >
// class deque {
// public:
//   // Member types
//   typedef T                                     value_type;
//   typedef Allocator                             allocator_type;
//   typedef typename Allocator::reference         reference;
//   typedef typename Allocator::const_reference   const_reference;
//   typedef typename Allocator::pointer           pointer;
//   typedef typename Allocator::const_pointer     const_pointer;
//   typedef implementation_defined                iterator;
//   typedef implementation_defined                const_iterator;
//   typedef implementation_defined                size_type;
//   typedef implementation_defined                difference_type;
//   typedef ft::reverse_iterator<iterator>       reverse_iterator;
//   typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;

//   // [23.2.1.1] Constructors, destructor, assignment
//   explicit deque(const Allocator& = Allocator());
//   explicit deque(size_type n, const T& value = T(), const Allocator& = Allocator());
//   template <class InputIterator>
//   deque(InputIterator first, InputIterator last, const Allocator& = Allocator());
//   deque(const deque& x);
//   ~deque();
//   deque& operator=(const deque& x);

//   // Assign
//   void assign(size_type n, const T& t);
//   template <class InputIterator>
//   void assign(InputIterator first, InputIterator last);

//   allocator_type get_allocator() const;

//   // [23.2.1.1] Iterators
//   iterator begin();
//   const_iterator begin() const;
//   iterator end();
//   const_iterator end() const;
//   reverse_iterator rbegin();
//   const_reverse_iterator rbegin() const;
//   reverse_iterator rend();
//   const_reverse_iterator rend() const;

//   // [23.2.1.2] Capacity
//   size_type size() const;
//   size_type max_size() const;
//   void resize(size_type sz, T c = T());
//   bool empty() const;

//   // [23.2.1.2] Element access
//   reference operator[](size_type n);
//   const_reference operator[](size_type n) const;
//   reference at(size_type n);
//   const_reference at(size_type n) const;
//   reference front();
//   const_reference front() const;
//   reference back();
//   const_reference back() const;

//   // [23.2.1.3] Modifiers
//   void push_front(const T& x);
//   void push_back(const T& x);
//   void pop_front();
//   void pop_back();
//   iterator insert(iterator position, const T& x);
//   void insert(iterator position, size_type n, const T& x);
//   template <class InputIterator>
//   void insert(iterator position, InputIterator first, InputIterator last);
//   iterator erase(iterator position);
//   iterator erase(iterator first, iterator last);
//   void clear();
//   void swap(deque& x);
// };

// // Relational Operators
// template <class T, class Alloc>
// bool operator==(const deque<T,Alloc>& x, const deque<T,Alloc>& y);
// template <class T, class Alloc>
// bool operator!=(const deque<T,Alloc>& x, const deque<T,Alloc>& y);
// template <class T, class Alloc>
// bool operator< (const deque<T,Alloc>& x, const deque<T,Alloc>& y);
// template <class T, class Alloc>
// bool operator> (const deque<T,Alloc>& x, const deque<T,Alloc>& y);
// template <class T, class Alloc>
// bool operator<=(const deque<T,Alloc>& x, const deque<T,Alloc>& y);
// template <class T, class Alloc>
// bool operator>=(const deque<T,Alloc>& x, const deque<T,Alloc>& y);

// // Specialized Algorithm
// template <class T, class Alloc>
// void swap(deque<T,Alloc>& x, deque<T,Alloc>& y);
