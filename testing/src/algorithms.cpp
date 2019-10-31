#include <algorithm>
#include <iostream>
#include <vector>
#include <list>

int main() {
    std::list<int> lt = {1, 5, 7, 2};
    std::vector<int> lt_cp(4);
    std::copy(lt.begin(), lt.end(), lt_cp.begin());
    for (auto e : lt_cp) std::cout << e << " ";
    std::cout << std::endl;
}