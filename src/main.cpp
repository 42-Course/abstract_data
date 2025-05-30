// main.cpp

#include <iostream>
#include <string>
#include <ctime>
#include "deque.hpp"

void run_vector_compliance_tests();
void run_list_compliance_tests();

void print_header(const std::string& container_name) {
    std::cout << "\n==========================" << std::endl;
    std::cout << "Testing: " << container_name << std::endl;
    std::cout << "==========================\n" << std::endl;
}

int main() {
    print_header("Vector");
    run_vector_compliance_tests();
    print_header("List");
    run_list_compliance_tests();

{
    ft::deque<int> dq;

    if (dq.empty())
        std::cout << "It's empty" << std::endl;
}

{
    ft::deque<int> dq(5, 42);
    std::cout << dq.front() << " " << dq.back() << "\n";
    dq[2] = 99;
    std::cout << dq.at(2) << "\n";
}
    return 0;
}
