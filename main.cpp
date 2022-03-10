#include <iostream>
#include <algorithm>
#include "set.h"

int main() {
    set<int> s;
    s.insert(1);
    s.insert(-1);
    s.insert(-2);
    s.insert(3);
    s.insert(2);
    s.insert(6);
    s.insert(4);
    s.insert(5);
    //s.remove(2);
    s.print();
    s.print_by_levels();
    std::cout << std::endl;
    for (const auto &item : s)
        std::cout << item << " ";
    std::cout << std::endl;
    auto i = std::find(s.begin(), s.end(), 5);
    assert(*i == 5);

    auto c = std::count_if(s.begin(), s.end(), [](auto x) { return x % 2 == 0; });
    assert(c == 4);

    std::cout << *s.upper_bound(3) << std::endl;

    set<int> s1;
    s1.insert(2);
    s1.insert(4);
    s1.insert(3);
    s1.insert(14);
    s1.insert(6);
    //s1.insert(0);
    auto e = (s == s1);
    assert(!e);

    set<int> s2(s1);
    assert(s2 == s1);
    return 0;
}
