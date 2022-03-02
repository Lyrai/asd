#include <iostream>
#include "set.h"

int main() {
    set<int> s;
    s.insert(1);
    s.insert(-1);
    s.insert(-2);
    s.insert(2);
    s.insert(3);
    s.insert(0);
    s.print();
    s.print_by_levels();

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
