#include <iostream>
#include <string>
#include <cassert>
#ifdef MODE_FT
    #include "list.hpp"
    namespace ns = ft;
#else
    #include <list>
    namespace ns = std;
#endif

void run_list_compliance_tests() {
    std::cout << "\n[ns::list] Starting API compliance tests..." << std::endl;

    ns::list<int> list_default;
    ns::list<int> list_fill(5, 42);
    int arr[] = {1, 2, 3, 4, 5};
    ns::list<int> list_range(arr, arr + 5);
    ns::list<int> list_copy(list_range);
    list_default = list_fill;

    const ns::list<int> const_list(list_range);
    assert(!const_list.empty());
    assert(const_list.front() == 1);
    assert(const_list.back() == 5);

    // Iterators
    for (ns::list<int>::iterator it = list_range.begin(); it != list_range.end(); ++it) *it;
    for (ns::list<int>::const_iterator it = const_list.begin(); it != const_list.end(); ++it) *it;
    for (ns::list<int>::reverse_iterator it = list_range.rbegin(); it != list_range.rend(); ++it) *it;
    for (ns::list<int>::const_reverse_iterator it = const_list.rbegin(); it != const_list.rend(); ++it) *it;

    // Modifiers
    list_default.assign(arr, arr + 3);
    list_default.assign(4, 7);
    list_default.push_back(99);
    list_default.pop_back();
    list_default.push_front(88);
    list_default.pop_front();

    list_default.insert(list_default.begin(), 33);
    list_default.insert(++list_default.begin(), 2, 77);
    list_default.insert(list_default.end(), arr, arr + 2);

    list_default.erase(list_default.begin());
    if (list_default.size() > 2)
        list_default.erase(list_default.begin(), ++ ++list_default.begin());

    list_default.resize(10, 100);
    list_default.resize(5);

    // Access
    if (!list_default.empty()) {
        list_default.front();
        list_default.back();
    }

    // Swap and compare
    ns::list<int> list_temp(list_default);
    list_temp.swap(list_default);
    ns::swap(list_temp, list_default);

    assert((list_temp == list_default) || (list_temp != list_default));
    (void)(list_temp < list_default);
    (void)(list_temp <= list_default);
    (void)(list_temp > list_default);
    (void)(list_temp >= list_default);

    std::cout << "[ns::list] All API compliance tests passed.\n" << std::endl;
}
