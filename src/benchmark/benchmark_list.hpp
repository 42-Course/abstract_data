#include "ContainerBenchmark.hpp"

template <template<typename, typename> class ListType, typename T>
void register_list_tests(benchmark::ContainerBenchmark<ListType<T, std::allocator<T>>, T>& bench) {
  typedef ListType<T, std::allocator<T>> List;

  // Constructors
  bench.add("default_ctor", [](List& l, const std::vector<T>&) {
    List tmp;
  });

  bench.add("fill_ctor", [](List& l, const std::vector<T>& data) {
    List tmp(data.size(), T());
  });

  bench.add("range_ctor", [](List& l, const std::vector<T>& data) {
    List tmp(data.begin(), data.end());
  });

  bench.add("copy_ctor", [](List& l, const std::vector<T>& data) {
    List tmp(data.begin(), data.end());
    List copy(tmp);
  });

  // Assignment
  bench.add("assign_fill", [](List& l, const std::vector<T>& data) {
    l.assign(data.size(), T());
  });

  bench.add("assign_range", [](List& l, const std::vector<T>& data) {
    l.assign(data.begin(), data.end());
  });

  // Element Access
  bench.add("front_back", [](List& l, const std::vector<T>& data) {
    l.assign(data.begin(), data.end());
    volatile T f = l.front();
    volatile T b = l.back();
  });

  // Modifiers
  bench.add("push_back", [](List& l, const std::vector<T>& data) {
    for (std::size_t i = 0; i < data.size(); ++i)
      l.push_back(data[i]);
  });

  bench.add("push_front", [](List& l, const std::vector<T>& data) {
    for (std::size_t i = 0; i < data.size(); ++i)
      l.push_front(data[i]);
  });

  bench.add("pop_back", [](List& l, const std::vector<T>& data) {
    l.assign(data.begin(), data.end());
    while (!l.empty()) l.pop_back();
  });

  bench.add("pop_front", [](List& l, const std::vector<T>& data) {
    l.assign(data.begin(), data.end());
    while (!l.empty()) l.pop_front();
  });

  bench.add("insert_erase", [](List& l, const std::vector<T>& data) {
    l.assign(data.begin(), data.end());
    l.insert(l.begin(), T());
    l.erase(l.begin());
  });

  bench.add("clear", [](List& l, const std::vector<T>& data) {
    l.assign(data.begin(), data.end());
    l.clear();
  });

  bench.add("resize_up", [](List& l, const std::vector<T>& data) {
    l.assign(data.begin(), data.end());
    l.resize(data.size() * 2);
  });

  bench.add("resize_down", [](List& l, const std::vector<T>& data) {
    l.assign(data.begin(), data.end());
    l.resize(data.size() / 2);
  });

  bench.add("swap", [](List& l, const std::vector<T>& data) {
    List tmp(data.begin(), data.end());
    l.swap(tmp);
  });

  // Operations
  bench.add("remove", [](List& l, const std::vector<T>& data) {
    l.assign(data.begin(), data.end());
    if (!data.empty())
      l.remove(data[0]);
  });

  bench.add("remove_if", [](List& l, const std::vector<T>& data) {
    l.assign(data.begin(), data.end());
    l.remove_if([](const T& val) { return false; }); // No-op
  });

  bench.add("unique", [](List& l, const std::vector<T>& data) {
    l.assign(data.begin(), data.end());
    l.unique();
  });

  bench.add("sort", [](List& l, const std::vector<T>& data) {
    l.assign(data.begin(), data.end());
    l.sort();
  });

  bench.add("reverse", [](List& l, const std::vector<T>& data) {
    l.assign(data.begin(), data.end());
    l.reverse();
  });

  bench.add("merge", [](List& l, const std::vector<T>& data) {
    List tmp(data.begin(), data.end());
    l.assign(data.begin(), data.end());
    l.sort();
    tmp.sort();
    l.merge(tmp);
  });

  bench.add("splice", [](List& l, const std::vector<T>& data) {
    List other(data.begin(), data.end());
    l.assign(data.begin(), data.end());
    l.splice(l.begin(), other);
  });

  // Iteration
  bench.add("iterator_traversal", [](List& l, const std::vector<T>& data) {
    l.assign(data.begin(), data.end());
    for (typename List::iterator it = l.begin(); it != l.end(); ++it) {
      volatile T x = *it;
    }
  });

  bench.add("insert_erase", [](List& l, const std::vector<T>& data) {
    typename List::iterator it = l.begin();
    for (size_t i = 0; i < data.size(); ++i)
      l.insert(it, data[i]);

    it = l.begin();
    while (it != l.end())
      it = l.erase(it);
  });

  bench.add("resize_smaller", [](List& l, const std::vector<T>& data) {
    l.assign(data.begin(), data.end());
    l.resize(data.size() / 2);
  });

  bench.add("resize_larger", [](List& l, const std::vector<T>& data) {
    l.assign(data.begin(), data.end());
    l.resize(data.size() * 2);
  });

  bench.add("clear", [](List& l, const std::vector<T>& data) {
    l.assign(data.begin(), data.end());
    l.clear();
  });

  bench.add("swap", [](List& l, const std::vector<T>& data) {
    List other(data.begin(), data.end());
    l.swap(other);
  });

  // Operations
  bench.add("remove", [](List& l, const std::vector<T>& data) {
    l.assign(data.begin(), data.end());
    if (!data.empty())
      l.remove(data[0]);  // simulate remove by value
  });

  bench.add("unique", [](List& l, const std::vector<T>& data) {
    l.assign(data.begin(), data.end());
    l.sort();
    l.unique();
  });

  bench.add("sort", [](List& l, const std::vector<T>& data) {
    l.assign(data.begin(), data.end());
    l.sort();
  });

  bench.add("reverse", [](List& l, const std::vector<T>& data) {
    l.assign(data.begin(), data.end());
    l.reverse();
  });

  bench.add("merge", [](List& l, const std::vector<T>& data) {
    List other(data.begin(), data.end());
    l.assign(data.begin(), data.end());
    l.sort();
    other.sort();
    l.merge(other);
  });

  bench.add("splice_begin", [](List& l, const std::vector<T>& data) {
    List other(data.begin(), data.end());
    l.splice(l.begin(), other);
  });

  // Iterator traversal
  bench.add("iterator_traversal", [](List& l, const std::vector<T>& data) {
    l.assign(data.begin(), data.end());
    for (typename List::iterator it = l.begin(); it != l.end(); ++it) {
      volatile T x = *it;
    }
  });

  bench.add("reverse_iterator_traversal", [](List& l, const std::vector<T>& data) {
    l.assign(data.begin(), data.end());
    for (typename List::reverse_iterator rit = l.rbegin(); rit != l.rend(); ++rit) {
      volatile T x = *rit;
    }
  });

  // Capacity functions
  bench.add("size_empty", [](List& l, const std::vector<T>& data) {
    l.assign(data.begin(), data.end());
    volatile bool e = l.empty();
    volatile std::size_t s = l.size();
  });

  bench.add("max_size", [](List& l, const std::vector<T>&) {
    volatile std::size_t m = l.max_size();
  });
}
