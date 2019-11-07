#include <unordered_map>
#include <iostream>
#include <set>
#include <algorithm>
#include <vector>

int main() {
  std::unordered_multimap<std::string,std::string> myumm = {
    {"John","Middle East"},
    {"John","Africa"},
    {"John","South America"},
    {"John","North America"},
    {"Adam","Middle East"},
    {"Adam","Africa"},
    {"Adam","South America"},
    {"Adam","North America"},
    {"Bill","Middle East"},
    {"Bill","Africa"},
    {"Bill","South America"},
    {"Bill","North America"}
  };

  for (auto& x: myumm) {
    std::cout << "Element [" << x.first << ":" << x.second << "]";
    std::cout << " is in bucket #" << myumm.bucket (x.first) << std::endl;
  }

  auto bkt = myumm.equal_range("John");
  for (auto it = bkt.first; it != bkt.second; ++it)
    std::cout << it->first << " " << it->second << "\n";

  //    for (auto& x: myumm) {
  //    std::cout << "Element [" << x.first << ":" << x.second << "]";
  //    std::cout << " is in bucket #" << myumm.bucket (x.first) << std::endl;
  //    }

  return 0;
}