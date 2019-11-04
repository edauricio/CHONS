#include "boost/pool/pool.hpp"
#include "boost/pool/poolfwd.hpp"
#include <unordered_set>
#include <iostream>
#include <vector>

int main() {
    std::vector<std::unordered_set<int>> ss;
    ss.push_back({1, 2, 5});
    for (auto a : ss[0])
        std::cout << "OK";

    size_t N = 10000000;
    int *a;

    // Normal alloc
    // for (int i = 0; i != N; ++i)
    //     a = new int;

    // Boost.Pool alloc
    // boost::pool<> pa(sizeof(int));
    // for (int i = 0; i != N; ++i)
    //     a = static_cast<int*>(pa.malloc());

    // CONCLUSIONS:
    // Using Boost allocator really makes memory allocation of lots of small
    // objects more efficient (more than twice compared to raw new usage).
    // However, this should be used when the number of objects to be allocated
    // is not known a priori or can't be determined. If this is not the case,
    // then a single new requesting memory for all the objects needed is way
    // better.

    // In other words: Boost.Pool helps when you really need to allocate many
    // small objects that you don't know the total number a priori (examples are
    // the STL containers -- set, list, map -- in a lot of situations).
}