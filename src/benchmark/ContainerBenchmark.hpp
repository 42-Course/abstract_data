#ifndef CONTAINER_BENCHMARK_HPP
#define CONTAINER_BENCHMARK_HPP

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include "Point.hpp"
#include "shared_utils.hpp"
#include "vector.hpp"
#include "list.hpp"


namespace benchmark {

template <typename Container, typename T>
struct TestCase {
  std::string label;
  void (*func)(Container&, const std::vector<T>&);
  TestCase(const std::string& l, void (*f)(Container&, const std::vector<T>&))
    : label(l), func(f) {}
};

template <typename Container, typename T>
class ContainerBenchmark {
  std::string container_name;
  std::string type_name;
  std::string ns;
  std::vector<TestCase<Container, T>> test_cases;

public:
  ContainerBenchmark(const std::string& ns_label,
                     const std::string& type,
                     const std::string& container)
    : container_name(container), type_name(type), ns(ns_label) {}

  void add(const std::string& label, void (*func)(Container&, const std::vector<T>&)) {
    test_cases.push_back(TestCase<Container, T>(label, func));
  }

  void run(std::size_t count, std::ofstream& out) {
    std::vector<T> data = benchmark::generate_data<T>(count);
    std::size_t total = test_cases.size();

    for (size_t i = 0; i < test_cases.size(); ++i) {
      std::ostringstream label;
      label <<  ns << " / " << type_name << " / " << count
            << " / " << test_cases[i].label << " [" << container_name << "]";
      print_progress(i, total, label.str());

      double time = benchmark::measure_time([&]() {
        Container c;
        test_cases[i].func(c, data);
      });

      out << type_name << "," << test_cases[i].label << "," << count << "," << ns << "," << time << "\n";
      std::cout << "\r" << std::string(80, ' ') << "\r";
    }
  }
};

} // namespace benchmark

#endif // CONTAINER_BENCHMARK_HPP