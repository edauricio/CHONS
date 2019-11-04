#include <unordered_map>
#include <iostream>
#include <set>
#include <algorithm>
#include <vector>

int main() {
    std::set<size_t>* st = new std::set<size_t>{5, 2, 7, 1};
    size_t ss = 2;
    //size_t sum = 
    std::unordered_map<std::set<size_t>*, size_t> uom;
    uom.emplace(new std::set<size_t>{5, 2, 7, 1}, ss);
    for (auto& em : uom)
        delete em.first;
    //delete st;

    std::vector<size_t> ok;
    std::set<size_t> oks;
    for (size_t i = 1; i != 201; ++i)
        ok.push_back(i);
    for (int i = 0; i != 200; ++i)
        for (int j = 0; j != 200; ++j) {
            auto it = oks.insert(ok[i]*ok[i]*ok[j] + ok[i]*ok[j]*ok[j]);
            if (!it.second)
                std::cout << i << " " << j << " = " << *(it.first) << "sh1t...\n";
        }

    
}