#include <iostream>
#include <vector>

int main() {
    std::vector<int> nodes =
        {   1, 345, 371, 352, 361, 379, 423, 407, 
            5231, 5232, 5262, 5261, 5295, 5296, 5327, 5328, 
            5439, 5440, 5329, 5330, 5999, 6000, 5835, 5836, 
            5442, 5441, 5843, 5844, 6001, 6002, 6003, 6004, 
            5331, 5334, 5333, 5332, 5443, 5444, 5445, 5446, 
            5846, 5847, 5848, 5845, 6005, 6006, 6007, 6008, 
            6009, 6010, 6011, 6012, 6013, 6014, 6015, 6016, 
            6017, 6018, 6019, 6020, 6021, 6022, 6023, 6024
        };

    std::vector<std::vector<size_t>> maps
                    {
                        {1, 5, 3, 0},
                        {0, 4, 8, 2},
                        {2, 9, 7, 1},
                        {3, 6, 10, 4},
                        {5, 7, 11, 6},
                        {8, 10, 11, 9}
                    };

    int numNodes = 8;
    int eleOrder = 2;

    for (int i = 0; i != maps.size(); ++i) {
        std::cout << "Face: " << i << "\n";
        for (int j = 0; j != maps[i].size(); ++j) {
            std::cout << "Edge " << j << ": " 
                    << numNodes + (eleOrder-1)*maps[i][j] << "\n";
        }
    }
}