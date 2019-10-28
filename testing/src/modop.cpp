#include <iostream>
#include <vector>

int main() {
  int numEdges = 3;
  std::vector<int> vi{1,5,7};

  for (int i = 0; i != numEdges; ++i) {
    std::cout << i << "\t" << i%numEdges << "\n";
    std::cout << "First: " << vi[i%numEdges] << "\n";
    std::cout << "Second: " << vi[(i+1)%numEdges] << "\n";
  }

}