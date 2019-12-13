#include <iostream>
#include <vector>

int main() {

    /* Algorithm to map from the weird Gmsh edge internal nodes in an Hexahedral
    element to the normal, correct Lagrangian (gmsh) ordering FOR EACH FACE
    OF THE HEXA ELEMENT */

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

    std::cout << "\n\n-=-=-=-=--=-=-=-\n\n";

    // std::vector<double> nodes_o3{1, 9, 21, 12, 17, 22, 27, 25,
    //                             127, 128, 142, 141, 159, 160, 175, 176,
    //                             199, 200, 177, 178, 319, 320, 271, 272, 
    //                             202, 201, 285, 286, 321, 322, 323, 324,
    //                             179, 182, 181, 180, 203, 204, 205, 206,
    //                             288, 289, 290, 287, 325, 326, 327, 328,
    //                             329, 330, 331, 332, 333, 334, 335, 336,
    //                             337, 338, 339, 340, 341, 342, 343, 344
    //                         };

    std::vector<double> nodes_o3{1, 9, 21, 12, 17, 22, 27, 25, 
                                443, 444, 445, 466, 465, 464, 491, 492, 
                                493, 515, 516, 517, 563, 564, 565, 518, 
                                519, 520, 803, 804, 805, 707, 708, 709, 
                                568, 567, 566, 734, 735, 736, 806, 807, 
                                808, 809, 810, 811, 521, 524, 523, 522, 
                                528, 527, 526, 525, 529, 569, 570, 571, 
                                572, 573, 574, 575, 576, 577, 738, 739, 
                                740, 737, 742, 743, 744, 741, 745, 812, 
                                813, 814, 815, 816, 817, 818, 819, 820, 
                                821, 822, 823, 824, 825, 826, 827, 828, 
                                829, 830, 831, 832, 833, 834, 835, 836, 
                                837, 838, 839, 840, 841, 842, 843, 844, 
                                845, 846, 847, 848, 849, 850, 851, 852, 
                                853, 854, 855, 856, 857, 858, 859, 860, 
                                861, 862, 863, 864, 865
                            };

    // Algorithm to map the Hexahedral gmsh ordering nodes to a natural, "tensor
    // product" ordering so we can easily calculate our mapping function

    int cnt = 0;
    eleOrder = 4;
    int ini_pos[] = {0, ((eleOrder+1)*(eleOrder+1)-1), 1,
                    eleOrder+1, ((eleOrder+1)*(eleOrder+1)-1-1),
                    (2*(eleOrder+1)-1)
                    };
    // Bottom vertices of first face
    for (int i = 0; i != 2; ++i, ++cnt)
        std::cout << cnt << ": " << ini_pos[0] + (i * eleOrder) << "\n";

    // Top vertices of first face
    for (int i = 0; i != 2; ++i, ++cnt)
        std::cout << cnt << ": " << ini_pos[1] - (i * eleOrder) << "\n";

    // // Bottom vertices of sixth face
    // for (int i = 0; i != 2; ++i, ++cnt)
    //     std::cout << cnt << ": " << ini_pos[2] + (i*eleOrder) << "\n";

    // // Top vertices of sixth face
    // for (int i = 0; i != 2; ++i, ++cnt)
    //     std::cout << cnt << ": " << ini_pos[3] - (i*eleOrder) << "\n";

    // First face, fourth edge interior nodes
    for (int i = 0; i != eleOrder-1; ++i, ++cnt)
        std::cout << nodes_o3[8 + maps[0][3]*(eleOrder-1) + i] << ": " << ini_pos[2] + i << "\n";

    // First face, first edge interior nodes
    for (int i = 0; i != eleOrder-1; ++i, ++cnt)
        std::cout << nodes_o3[8 + maps[0][0]*(eleOrder-1) + i] << ": " << ini_pos[3] + i*(eleOrder+1) << "\n";

    // First face, third edge interior nodes
    for (int i = 0; i != eleOrder-1; ++i, ++cnt)
        std::cout << nodes_o3[8 + maps[0][2]*(eleOrder-1) + i] << ": " 
                << ini_pos[0]+(2*(eleOrder+1)-1) + i*(eleOrder+1) << "\n";

    // First face, second edge interior nodes
    for (int i = 0; i != eleOrder-1; ++i, ++cnt)
        std::cout << nodes_o3[8 + maps[0][1]*(eleOrder-1) + i] << ": " << ini_pos[4] - i << "\n";
}