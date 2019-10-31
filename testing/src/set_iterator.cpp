/*
std::set and std;:unordered_set iterators are iterators to const value_type;
the test here is to see wether dereferencing the iterator returns a reference
to a const value_type or to a value_type (the real type stored in the set).

Although the test showed that it is a value_type and hence can be passed to a
function with parameter value_type, the real code (in ElementComposite) gave
opposite results, i.e., it returned a reference to a const value_type hence
preventing it from being passed to a function expecting a value_type argument.
*/

#include <map>
#include <set>
#include <unordered_set>
#include <iostream>
#include <typeinfo>

class Base {
public:
    virtual void add(const Base*) = 0;
    int a;
};

class Derived1 : public Base {
public:
    virtual void add(const Base*) {};
    int b;
};

class Derived2 : public Base {
public:
    Derived2(std::unordered_set<Base*>);
    virtual void add(const Base*) {};
    int b;
};

void take_it(Base* a) {
    std::cout << "value received is: " << a->a << std::endl;
}

int main() {
    std::unordered_set<Base*> stest;
    Derived1 *a, *b, *c;
    a = new Derived1;
    b = new Derived1;
    c = new Derived1;
    stest.insert(a);
    stest.insert(b);
    stest.insert(c);

    Derived2 *a2 = new Derived2(stest);

    std::map<std::set<int>, size_t> mapset;
    std::set<int> sord {6, 1, 9, 10, 2, 0};
    for (auto e : sord)
        std::cout << e << " ";
    std::cout << std::endl;

    std::set<int> sord2 {0, 1, 9, 10, 2, 6};
    for (auto e : sord2)
        std::cout << e << " ";
    std::cout << std::endl;

    mapset.emplace(sord, 1);
    if (mapset.find(sord2) != mapset.end())
        std::cout << "YESS\n";



    

}

Derived2::Derived2(std::unordered_set<Base*> s) {
    for (auto it = s.begin(); it != s.end(); ++it) {
        std::cout << "Type: " << typeid(*it).name() << std::endl;
        add(*it);
    }
}