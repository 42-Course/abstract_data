#ifndef SHARED_UTILS_HPP
#define SHARED_UTILS_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <string>
#include <ctime>
#include <cstdlib>
#include <iterator>
#include "Point.hpp"

namespace benchmark {

template <typename T>
std::vector<T> generate_data(std::size_t count);

template <>
std::vector<int> generate_data<int>(std::size_t count) {
  std::vector<int> data;
  for (std::size_t i = 0; i < count; ++i)
    data.push_back(rand());
  return data;
}

template <>
std::vector<std::string> generate_data<std::string>(std::size_t count) {
  static const std::vector<std::string> words = {
    "alpha", "beta", "gamma", "delta", "epsilon", "zeta", "theta", "lambda", "omega",
    "quantum", "vector", "matrix", "scalar", "compute", "storage", "network", "thread",
    "function", "class", "object", "method", "template", "static", "dynamic", "virtual",
    "benchmark", "compile", "optimize", "debug", "release", "profile", "execute", "memory",
    "pointer", "iterator", "container", "allocator", "system", "kernel", "driver", "module",
    "parallel", "concurrent", "atomic", "synchronize", "mutex", "lock", "condition", "event",
    "queue", "stack", "map", "set", "graph", "tree", "node", "edge", "weight", "priority",
    "code", "syntax", "token", "parse", "analyze", "resolve", "link", "runtime", "build",
    "segment", "page", "heap", "buffer", "stream", "input", "output", "read", "write"
  };

  std::vector<std::string> data;
  data.reserve(count);

  for (std::size_t i = 0; i < count; ++i) {
    const std::string& word = words[rand() % words.size()] + "_" + words[rand() % words.size()];
    data.push_back(word);
  }

  return data;
}

template <>
std::vector<Point> generate_data<Point>(std::size_t count) {
  std::vector<Point> data;
  for (std::size_t i = 0; i < count; ++i)
    data.push_back(Point(rand() % 1000, rand() % 1000));
  return data;
}

template <typename Func>
double measure_time(Func f) {
  clock_t start = clock();
  f();
  clock_t end = clock();
  return static_cast<double>(end - start) / CLOCKS_PER_SEC;
}

} // namespace benchmark

#include <iomanip>
#include <chrono>

inline void print_progress(std::size_t current, std::size_t total, const std::string& label) {
  static std::size_t last_len = 0;

  const int bar_width = 50;
  float progress = static_cast<float>(current) / total;
  int pos = static_cast<int>(bar_width * progress);

  std::ostringstream oss;
  oss << "[";
  for (int i = 0; i < bar_width; ++i)
    oss << (i < pos ? "=" : (i == pos ? ">" : " "));
  oss << "] " << std::setw(3) << static_cast<int>(progress * 100.0) << "%  " << label;

  std::string line = oss.str();
  std::cout << "\r" << line;

  // Pad with spaces to fully clear the previous line
  if (line.length() < last_len)
    std::cout << std::string(last_len - line.length(), ' ');

  std::cout << std::flush;
  last_len = line.length();
}



#endif
