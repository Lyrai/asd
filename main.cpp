#include <iostream>
#include <algorithm>
#include "set.h"

int main() {
    set<int> s;
    s.insert(1);
    s.insert(-1);
    s.insert(-2);
    s.insert(3);
    //s.insert(2);
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
    std::cout << *i << "\n\n";
    ++i;

    auto c = std::count_if(s.begin(), s.end(), [](auto x) { return x % 2 == 0; });
    std::cout << c << "\n\n";

    set<int> s1;
    s1.insert(1);
    s1.insert(-1);
    s1.insert(-2);
    s1.insert(2);
    s1.insert(3);
    s1.insert(0);
    auto e = (s == s1);
    std::cout << "s == s1: " << e << std::endl;
    return 0;
}
