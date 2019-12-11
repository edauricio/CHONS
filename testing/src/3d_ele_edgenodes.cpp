#include <iostream>
#include <vector>

int main() {
    std::vector<int> nodes =
        {   1, 345, 371, 352, 361, 379, 423, 407, 
            443, 444, 445, 490, 489, 488, 539, 540, 
            541, 587, 588, 589, 803, 804, 805, 590, 
            591, 592, 1883, 1884, 1885, 1565, 1566, 
            1567, 808, 807, 806, 1580, 1581, 1582, 1886, 
            1887, 1888, 1889, 1890, 1891, 593, 596, 595, 
            594, 600, 599, 598, 597, 601, 809, 810, 811, 
            812, 813, 814, 815, 816, 817, 1584, 1585, 1586, 
            1583, 1588, 1589, 1590, 1587, 1591, 1892, 1893, 
            1894, 1895, 1896, 1897, 1898, 1899, 1900, 1901, 
            1902, 1903, 1904, 1905, 1906, 1907, 1908, 1909, 
            1910, 1911, 1912, 1913, 1914, 1915, 1916, 1917, 
            1918, 1919, 1920, 1921, 1922, 1923, 1924, 1925, 
            1926, 1927, 1928, 1929, 1930, 1931, 1932, 1933, 
            1934, 1935, 1936, 1937, 1938, 1939, 1940, 1941, 
            1942, 1943, 1944, 1945 
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

    std::vector<std::vector<int>> faces{
                        {0, 3, 2, 1},
                        {0, 1, 5, 4},
                        {0, 4, 7, 3},
                        {1, 2, 6, 5},
                        {2, 3, 7, 6},
                        {4, 5, 6, 7}
                    };

    int numNodes = 8;
    int numEdges = 12;
    int eleOrder = 4;
    int inc, ini;

    for (int i = 0; i != maps.size(); ++i) {
        std::cout << "Face: " << i << "\n";
        for (int j = 0; j != maps[i].size(); ++j) {
            std::cout << "Edge " << j << ": ";
            inc = ((faces[i][(j+1)%faces[i].size()] - faces[i][j]) > 0) ? 1 : -1;
            ini = (inc > 0) ? numNodes + (eleOrder-1)*maps[i][j]
                            : numNodes + (eleOrder-1)*maps[i][j] + (eleOrder-2);
            for (int  k = 0; k != eleOrder-1; ++k, ini += inc) {
                std::cout << nodes[ini] << " ";
            }
            std::cout << "\n";
        }
        std::cout << "Interior nodes: ";
        for (int k = 0; k != (eleOrder-1)*(eleOrder-1); ++k)
            std::cout << nodes[numNodes + (eleOrder-1)*numEdges + (eleOrder-1)*(eleOrder-1)*i + k] << " ";
        std::cout << "\n";
    }
}