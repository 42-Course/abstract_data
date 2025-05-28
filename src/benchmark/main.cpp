#include "ContainerBenchmark.hpp"
#include "benchmark_vector.hpp"
#include "benchmark_list.hpp"
#include "Point.hpp"

int main() {
  std::ofstream csv_vector("benchmark_vector.csv");
  std::ofstream csv_list("benchmark_list.csv");
  csv_vector << "Type,Function,Size,Namespace,Time\n";
  csv_list << "Type,Function,Size,Namespace,Time\n";

  std::size_t sizes[] = {1000, 10000, 100000};

  for (std::size_t size : sizes) {
    // -   VECTOR  -
    // ---- INT ----    
    {
      benchmark::ContainerBenchmark<ft::vector<int>, int> bench("ft", "int", "vector");
      register_vector_tests<ft::vector, int>(bench);
      bench.run(size, csv_vector);
    }

    {
      benchmark::ContainerBenchmark<std::vector<int>, int> bench("std", "int", "vector");
      register_vector_tests<std::vector, int>(bench);
      bench.run(size, csv_vector);
    }

    // ---- STRING ----
    {
      benchmark::ContainerBenchmark<ft::vector<std::string>, std::string> bench("ft", "string", "vector");
      register_vector_tests<ft::vector, std::string>(bench);
      bench.run(size, csv_vector);
    }

    {
      benchmark::ContainerBenchmark<std::vector<std::string>, std::string> bench("std", "string", "vector");
      register_vector_tests<std::vector, std::string>(bench);
      bench.run(size, csv_vector);
    }

    // ---- POINT ----
    {
      benchmark::ContainerBenchmark<ft::vector<Point>, Point> bench("ft", "point", "vector");
      register_vector_tests<ft::vector, Point>(bench);
      bench.run(size, csv_vector);
    }

    {
      benchmark::ContainerBenchmark<std::vector<Point>, Point> bench("std", "point", "vector");
      register_vector_tests<std::vector, Point>(bench);
      bench.run(size, csv_vector);
    }

    // -   LIST    -
    // ---- INT ----
    {
      benchmark::ContainerBenchmark<ft::list<int>, int> bench("ft", "int", "list");
      register_list_tests<ft::list, int>(bench);
      bench.run(size, csv_list);
    }

    {
      benchmark::ContainerBenchmark<std::list<int>, int> bench("std", "int", "list");
      register_list_tests<std::list, int>(bench);
      bench.run(size, csv_list);
    }

    // ---- STRING ----
    {
      benchmark::ContainerBenchmark<ft::list<std::string>, std::string> bench("ft", "string", "list");
      register_list_tests<ft::list, std::string>(bench);
      bench.run(size, csv_list);
    }

    {
      benchmark::ContainerBenchmark<std::list<std::string>, std::string> bench("std", "string", "list");
      register_list_tests<std::list, std::string>(bench);
      bench.run(size, csv_list);
    }

    // ---- POINT ----
    {
      benchmark::ContainerBenchmark<ft::list<Point>, Point> bench("ft", "point", "list");
      register_list_tests<ft::list, Point>(bench);
      bench.run(size, csv_list);
    }

    {
      benchmark::ContainerBenchmark<std::list<Point>, Point> bench("std", "point", "list");
      register_list_tests<std::list, Point>(bench);
      bench.run(size, csv_list);
    }
  }
}
