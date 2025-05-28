#include "ContainerBenchmark.hpp"

template <template<typename, typename> class VecType, typename T>
void register_vector_tests(benchmark::ContainerBenchmark<VecType<T, std::allocator<T>>, T>& bench) {
  typedef VecType<T, std::allocator<T>> Vec;

  bench.add("default_ctor", [](Vec& v, const std::vector<T>&){ Vec tmp; });

  bench.add("fill_ctor", [](Vec& v, const std::vector<T>& data){
    Vec tmp(data.size(), T());
  });

  bench.add("range_ctor", [](Vec& v, const std::vector<T>& data){
    Vec tmp(data.begin(), data.end());
  });

  bench.add("copy_ctor", [](Vec& v, const std::vector<T>& data){
    Vec tmp(data.begin(), data.end());
    Vec copy(tmp);
  });

  bench.add("assign_fill", [](Vec& v, const std::vector<T>& data){
    v.assign(data.size(), T());
  });

  bench.add("assign_range", [](Vec& v, const std::vector<T>& data){
    v.assign(data.begin(), data.end());
  });

  bench.add("push_back", [](Vec& v, const std::vector<T>& data){
    for (size_t i = 0; i < data.size(); ++i)
      v.push_back(data[i]);
  });

  bench.add("pop_back", [](Vec& v, const std::vector<T>& data){
    v.assign(data.begin(), data.end());
    for (size_t i = 0; i < data.size(); ++i)
      v.pop_back();
  });

  bench.add("insert_begin", [](Vec& v, const std::vector<T>& data){
    for (size_t i = 0; i < data.size(); ++i)
      v.insert(v.begin(), data[i]);
  });

  bench.add("erase_begin", [](Vec& v, const std::vector<T>& data){
    v.assign(data.begin(), data.end());
    while (!v.empty())
      v.erase(v.begin());
  });

  bench.add("clear", [](Vec& v, const std::vector<T>& data){
    v.assign(data.begin(), data.end());
    v.clear();
  });

  bench.add("resize_smaller", [](Vec& v, const std::vector<T>& data){
    v.assign(data.begin(), data.end());
    v.resize(data.size() / 2);
  });

  bench.add("resize_larger", [](Vec& v, const std::vector<T>& data){
    v.assign(data.begin(), data.end());
    v.resize(data.size() * 2);
  });

  bench.add("swap", [](Vec& v, const std::vector<T>& data){
    Vec tmp(data.begin(), data.end());
    v.swap(tmp);
  });

  bench.add("operator_at", [](Vec& v, const std::vector<T>& data){
    v.assign(data.begin(), data.end());
    for (size_t i = 0; i < v.size(); ++i) {
      volatile T x = v[i]; // prevent optimizing out
    }
  });

  bench.add("front_back", [](Vec& v, const std::vector<T>& data){
    v.assign(data.begin(), data.end());
    volatile T f = v.front();
    volatile T b = v.back();
  });

  bench.add("iterator_traversal", [](Vec& v, const std::vector<T>& data){
    v.assign(data.begin(), data.end());
    for (typename Vec::iterator it = v.begin(); it != v.end(); ++it) {
      volatile T x = *it;
    }
  });
}
