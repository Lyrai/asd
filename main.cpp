#include <iostream>
#include <algorithm>
#include "set.h"

void tests();

int main() {
    setlocale(LC_ALL, "ru_RU.utf8");
    /*set<int> s;
    s.insert(1);
    std::cout << "inserted 1" << std::endl;
    s.insert(-1);
    std::cout << "inserted -1" << std::endl;
    s.insert(-2);
    std::cout << "inserted -2" << std::endl;
    s.insert(3);
    std::cout << "inserted 3" << std::endl;
    s.insert(2);
    std::cout << "inserted 2" << std::endl;
    s.insert(6);
    std::cout << "inserted 6" << std::endl;
    s.insert(4);
    std::cout << "inserted 4" << std::endl;
    s.insert(5);
    std::cout << "inserted 5" << std::endl;
    //s.remove(2);
    //s.print();
    //s.print_by_levels();
    for (const auto &item : s)
        std::cout << item << " ";
    std::cout << std::endl;
    /*for (const auto &item : s)
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
    assert(s2 == s1);*/
    tests();
    return 0;
}

template<typename T> using Myal = std::allocator<T>;
template<typename T> using Mypred = std::less<T>;
template<typename T> using ContainerTemplate = set<T, Mypred<T>>;

class Assert {
public:
    template<class T>
    static void AreEqual(T v1, T v2, std::string &&msg) {
        if(v1 != v2)
            std::cout << msg << std::endl;
    }

    template<class T>
    static void IsTrue(T v, std::string &&msg) {
        if(!v)
            std::cout << msg << std::endl;
    }
};

void tests() {
    {
        ContainerTemplate<int> T1;
        Assert::AreEqual(T1.size(), ContainerTemplate<int>::size_type(0), "Пустое дерево имеет ненулевой размер!");
        ContainerTemplate<int> Tree = {40, 50, 30, 35, 10, 75, 23, 87, 68};
        Assert::AreEqual(Tree.size(), ContainerTemplate<int>::size_type(9), "Неверно указывается размер дерева!");
        ContainerTemplate<int> Tree2(Tree);
        Assert::AreEqual(Tree.size(), Tree2.size(), "Неверно указывается размер после копирования!");
    }
    {
        ContainerTemplate<int> T1, T2;
        Assert::IsTrue(T1 == T2, "Неверно выполняется сравнение пустых деревьев!");
        ContainerTemplate<int> Tree = { 40,50,30,35,10,75,23,87,68 };
        ContainerTemplate<int> Tree2(Tree);
        Assert::IsTrue(Tree == Tree2, "Ошибка сравнения деревьев на равенство после копирования!");
    }
    {
        ContainerTemplate<int> T1 = { 40,50,30,35,10,75,23,87,68 };
        ContainerTemplate<int> T2;
        T2 = T1;
        Assert::IsTrue(T1 == T2, "Ошибка в операторе присваивания!");
        T2.clear();
        T1 = T2;
        Assert::IsTrue(T1 == T2, "Ошибка в операторе присваивания пустых деревьев!");
        ContainerTemplate<int> T3 = { 40,50,30,35,10,75,23,87,68 };
        T1 = T3;
        T2 = std::move(T3);
        Assert::IsTrue(T1 == T2, "Ошибка при перемещающем операторе присваивания!");
    }
}