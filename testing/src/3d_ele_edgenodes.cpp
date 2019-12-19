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

    // 3rd order
    // std::vector<double> nodes_o3{1, 9, 21, 12, 17, 22, 27, 25,
    //                             127, 128, 142, 141, 159, 160, 175, 176,
    //                             199, 200, 177, 178, 319, 320, 271, 272, 
    //                             202, 201, 285, 286, 321, 322, 323, 324,
    //                             179, 182, 181, 180, 203, 204, 205, 206,
    //                             288, 289, 290, 287, 325, 326, 327, 328,
    //                             329, 330, 331, 332, 333, 334, 335, 336,
    //                             337, 338, 339, 340, 341, 342, 343, 344
    //                         };

    // 4th order
    // std::vector<double> nodes_o3{1, 9, 21, 12, 17, 22, 27, 25, 
    //                             443, 444, 445, 466, 465, 464, 491, 492, 
    //                             493, 515, 516, 517, 563, 564, 565, 518, 
    //                             519, 520, 803, 804, 805, 707, 708, 709, 
    //                             568, 567, 566, 734, 735, 736, 806, 807, 
    //                             808, 809, 810, 811, 521, 524, 523, 522, 
    //                             528, 527, 526, 525, 529, 569, 570, 571, 
    //                             572, 573, 574, 575, 576, 577, 738, 739, 
    //                             740, 737, 742, 743, 744, 741, 745, 812, 
    //                             813, 814, 815, 816, 817, 818, 819, 820, 
    //                             821, 822, 823, 824, 825, 826, 827, 828, 
    //                             829, 830, 831, 832, 833, 834, 835, 836, 
    //                             837, 838, 839, 840, 841, 842, 843, 844, 
    //                             845, 846, 847, 848, 849, 850, 851, 852, 
    //                             853, 854, 855, 856, 857, 858, 859, 860, 
    //                             861, 862, 863, 864, 865
    //                         };

    // 6th order
    std::vector<double> nodes_o3{1, 9, 21, 12, 17, 22, 27, 25, 
                                1145, 1146, 1147, 1148, 1149, 1184, 1183, 1182, 
                                1181, 1180, 1225, 1226, 1227, 1228, 1229, 1265, 
                                1266, 1267, 1268, 1269, 1385, 1386, 1387, 1388, 
                                1389, 1270, 1271, 1272, 1273, 1274, 1985, 1986, 
                                1987, 1988, 1989, 1745, 1746, 1747, 1748, 1749, 
                                1394, 1393, 1392, 1391, 1390, 1810, 1811, 1812, 
                                1813, 1814, 1990, 1991, 1992, 1993, 1994, 1995, 
                                1996, 1997, 1998, 1999, 1275, 1278, 1277, 1276, 
                                1290, 1289, 1288, 1287, 1286, 1285, 1284, 1283, 
                                1282, 1281, 1280, 1279, 1291, 1294, 1293, 1292, 
                                1298, 1297, 1296, 1295, 1299, 1395, 1396, 1397, 
                                1398, 1399, 1400, 1401, 1402, 1403, 1404, 1405, 
                                1406, 1407, 1408, 1409, 1410, 1411, 1412, 1413, 
                                1414, 1415, 1416, 1417, 1418, 1419, 1816, 1817, 
                                1818, 1815, 1822, 1823, 1824, 1825, 1826, 1827, 
                                1828, 1829, 1830, 1819, 1820, 1821, 1832, 1833, 
                                1834, 1831, 1836, 1837, 1838, 1835, 1839, 2000, 
                                2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 
                                2009, 2010, 2011, 2012, 2013, 2014, 2015, 2016, 
                                2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024, 
                                2025, 2026, 2027, 2028, 2029, 2030, 2031, 2032, 
                                2033, 2034, 2035, 2036, 2037, 2038, 2039, 2040, 
                                2041, 2042, 2043, 2044, 2045, 2046, 2047, 2048, 
                                2049, 2050, 2051, 2052, 2053, 2054, 2055, 2056, 
                                2057, 2058, 2059, 2060, 2061, 2062, 2063, 2064, 
                                2065, 2066, 2067, 2068, 2069, 2070, 2071, 2072, 
                                2073, 2074, 2075, 2076, 2077, 2078, 2079, 2080, 
                                2081, 2082, 2083, 2084, 2085, 2086, 2087, 2088, 
                                2089, 2090, 2091, 2092, 2093, 2094, 2095, 2096, 
                                2097, 2098, 2099, 2100, 2101, 2102, 2103, 2104, 
                                2105, 2106, 2107, 2108, 2109, 2110, 2111, 2112, 
                                2113, 2114, 2115, 2116, 2117, 2118, 2119, 2120, 
                                2121, 2122, 2123, 2124, 2125, 2126, 2127, 2128, 
                                2129, 2130, 2131, 2132, 2133, 2134, 2135, 2136, 
                                2137, 2138, 2139, 2140, 2141, 2142, 2143, 2144, 
                                2145, 2146, 2147, 2148, 2149, 2150, 2151, 2152, 
                                2153, 2154, 2155, 2156, 2157, 2158, 2159, 2160, 
                                2161, 2162, 2163, 2164, 2165, 2166, 2167, 2168, 
                                2169, 2170, 2171, 2172, 2173, 2174, 2175, 2176, 
                                2177, 2178, 2179, 2180, 2181, 2182, 2183, 2184, 
                                2185, 2186, 2187, 2188, 2189, 2190, 2191, 2192, 
                                2193, 2194, 2195, 2196, 2197, 2198, 2199};

    // Algorithm to map the Hexahedral gmsh ordering nodes to a natural, "tensor
    // product" ordering so we can easily calculate our mapping function

    int cnt = 0;
    eleOrder = 6;
    int ini_pos[] = {0, // 0
                    ((eleOrder+1)*(eleOrder+1)-1), // 1
                    (eleOrder+1)*(eleOrder+1)*eleOrder, // 2
                    (eleOrder+1)*(eleOrder+1)*eleOrder + ((eleOrder+1)*(eleOrder+1)-1), // 3
                    1, // 4 (1st edge)
                    (eleOrder+1), // 5 (2nd edge)
                    (eleOrder+1)*(eleOrder+1), // 6 (3rd edge)
                    2*(eleOrder+1)-1, // 7 (4th edge)
                    (eleOrder+1)*(eleOrder+1) + eleOrder, // 8 (5th edge)
                    (eleOrder+1)*(eleOrder+1) - 1 - 1, // 9 (6th edge)
                    2*(eleOrder+1)*(eleOrder+1) - 1, // 10 (7th edge)
                    2*(eleOrder+1)*(eleOrder+1) - 1 - eleOrder, // 11 (8th edge)
                    eleOrder*(eleOrder+1)*(eleOrder+1) + 1, // 12 (9th edge)
                    eleOrder*(eleOrder+1)*(eleOrder+1) + (eleOrder+1), // 13 (10th edge)
                    eleOrder*(eleOrder+1)*(eleOrder+1) + 2*(eleOrder+1) - 1, // 14 (11th edge)
                    (eleOrder+1)*(eleOrder+1)*(eleOrder+1) - 1 - 1, // 15 (12th edge)
                    };
    // Bottom vertices of first face
    for (int i = 0; i != 2; ++i, ++cnt)
        std::cout << nodes_o3[cnt] << ": " << ini_pos[0] + (i * eleOrder) << "\n";

    // Top vertices of first face
    for (int i = 0; i != 2; ++i, ++cnt)
        std::cout << nodes_o3[cnt] << ": " << ini_pos[1] - (i * eleOrder) << "\n";

    // Bottom vertices of sixth face
    for (int i = 0; i != 2; ++i, ++cnt)
        std::cout << nodes_o3[cnt] << ": " << ini_pos[2] + (i*eleOrder) << "\n";

    // Top vertices of sixth face
    for (int i = 0; i != 2; ++i, ++cnt)
        std::cout << nodes_o3[cnt] << ": " << ini_pos[3] - (i*eleOrder) << "\n";

    // First hexa edge -- Fourth edge of first face
    for (int i = 0; i != eleOrder-1; ++i, ++cnt)
        std::cout << nodes_o3[cnt] << ": " << ini_pos[4] + i << "\n";

    // Second hexa edge -- First edge of first face
    for (int i = 0 ; i != eleOrder-1; ++i, ++cnt)
        std::cout << nodes_o3[cnt] << ": " << ini_pos[5] + i*(eleOrder+1) << "\n";

    // Third hexa edge -- Fourth edge of second face
    for (int i = 0; i != eleOrder-1; ++i, ++cnt)
        std::cout << nodes_o3[cnt] << ": " << ini_pos[6] + i*(eleOrder+1)*(eleOrder+1) << "\n";

    // Fourth hexa edge -- Third edge of first face
    for (int i = 0; i != eleOrder-1; ++i, ++cnt)
        std::cout << nodes_o3[cnt] << ": " << ini_pos[7] + i*(eleOrder+1) << "\n";

    // Fifth hexa edge --  Second edge of second face
    for (int i = 0; i != eleOrder-1; ++i, ++cnt)
        std::cout << nodes_o3[cnt] << ": " << ini_pos[8] + i*(eleOrder+1)*(eleOrder+1) << "\n";

    // Sixth hexa edge -- Second edge of first face
    for (int i = 0; i != eleOrder-1; ++i, ++cnt)
        std::cout << nodes_o3[cnt] << ": " << ini_pos[9] - i << "\n";

    // Seventh hexa edge -- Fourth edge of fifth face
    for (int i = 0; i != eleOrder-1; ++i, ++cnt)
        std::cout << nodes_o3[cnt] << ": " << ini_pos[10] + i*(eleOrder+1)*(eleOrder+1) << "\n";

    // Eigth hexa edge -- Second edge of fifth face
    for (int i = 0; i != eleOrder-1; ++i, ++cnt)
        std::cout << nodes_o3[cnt] << ": " << ini_pos[11] + i*(eleOrder+1)*(eleOrder+1) << "\n";

    // Ninth hexa edge -- First edge of sixth face
    for (int i = 0; i != eleOrder-1; ++i, ++cnt)
        std::cout << nodes_o3[cnt] << ": " << ini_pos[12] + i << "\n";

    // Tenth hexa edge --  Fourth edge of sixth face
    for (int i = 0; i != eleOrder-1; ++i, ++cnt)
        std::cout << nodes_o3[cnt] << ": " << ini_pos[13] + i*(eleOrder+1) << "\n";

    // Eleventh hexa edge -- Second edge of sixth face
    for (int i = 0; i != eleOrder-1; ++i, ++cnt)
        std::cout << nodes_o3[cnt] << ": " << ini_pos[14] + i*(eleOrder+1) << "\n";

    // Twelveth hexa edge -- Third edge of sixth face
    for (int i = 0; i != eleOrder-1; ++i, ++cnt)
        std::cout << nodes_o3[cnt] << ": " << ini_pos[15] - i << "\n";

    std::cout << "\n\n";

    // Now that all vertices and edges have been placed, and since gmsh doesn't
    // have any other extraneous node ordering for the face internal nodes, we
    // can simply use the algorithm developed previously for quad elements, for
    // each face

    // There should be one loop for each face, since the node ordering now
    // simply follows the face internal node common ordering (i.e. lagrangian
    // ordering of the internal nodes, which means embedded quads). In other words
    // since we are folloing the counter ("cnt"), we should now deal with each
    // face separately, because this counter fills each face in order from now on
    // This also means that the vector stride_quad should either be shrinked to
    // size 4 and then reused, or it could be completely removed altogether,
    // because now, since we will have 6 loops, we will know in advance which
    // face we're dealing with and thus what are the strides for each vertex/edge
    // of that face.

    // First face loop
    int pos_quad[] = {(eleOrder+1) + 1, // Bottom-left vertex
                    eleOrder*(eleOrder+1) - 1 - 1, // Top-right vertex
                    2*(eleOrder+1) + 1, // Bottom edge
                    (eleOrder-1)*(eleOrder+1) + 1 + 1, // Right edge
                    (eleOrder-1)*(eleOrder+1) - 1 - 1, // Top edge
                    2*(eleOrder+1) - 1 - 1 - 1  // Left edge
                    };
    for (int k = eleOrder-2; k > 0; k -= 2) {
        // Bottom vertices
        for (int i = 0; i != 2; ++cnt, ++i)
            std::cout << nodes_o3[cnt] << ": " << pos_quad[0] + i*(k*(eleOrder+1))  << "\n";
        // Top vertices
        for (int i = 0; i != 2; ++cnt, ++i)
            std::cout << nodes_o3[cnt] << ": " << pos_quad[1] - i*(k*(eleOrder+1))  << "\n";

        // Bottom edge interior nodes
        for (int i = 0; i != k-1; ++i, ++cnt)
            std::cout << nodes_o3[cnt] << ": " << pos_quad[2] + i*(eleOrder+1) << "\n"; // (cnt - k) + i ?

        // Right edge interior nodes
        for (int i = 0; i != k-1; ++i, ++cnt)
            std::cout << nodes_o3[cnt] << ": " << pos_quad[3] + i << "\n";

        // Top edge interior nodes
        for (int i = 0; i != k-1; ++i, ++cnt)
            std::cout << nodes_o3[cnt] << ": " << pos_quad[4] - i*(eleOrder+1) << "\n";

        // Left edge interior nodes
        for (int i = 0; i != k-1; ++i, ++cnt)
            std::cout << nodes_o3[cnt] << ": " << pos_quad[5] - i << "\n";

        // If the quad we're analyzing is of 2nd order, it'll have a single internal
        // node, so let's account for it
        if (k == 2)
            std::cout << nodes_o3[cnt++] << ": " 
                            << pos_quad[2] + 1 << "\n";

        // Recalculate initial positions for the next, inner quad
        pos_quad[0] += (eleOrder + 1 + 1);
        pos_quad[1] -= (eleOrder + 1 + 1);
        pos_quad[2] = pos_quad[0] + (eleOrder + 1);
        pos_quad[3] -= eleOrder;
        pos_quad[4] = pos_quad[1] - (eleOrder + 1);
        pos_quad[5] += eleOrder;
    }

        std::cout << "\n\n";


    // Second face loop
    pos_quad[0] = (eleOrder+1)*(eleOrder+1) + 1; // Bottom-left vertex
    pos_quad[1] = (eleOrder-1)*(eleOrder+1)*(eleOrder+1) + (eleOrder-1); // Top-right vertex
    pos_quad[2] = (eleOrder+1)*(eleOrder+1) + 1 + 1; // Bottom edge
    pos_quad[3] = 2*(eleOrder+1)*(eleOrder+1) + (eleOrder-1); // Right edge
    pos_quad[4] = (eleOrder-1)*(eleOrder+1)*(eleOrder+1) + (eleOrder-1) - 1; // Top edge
    pos_quad[5] = (eleOrder-2)*(eleOrder+1)*(eleOrder+1) + 1;  // Left edge
                    

    for (int k = eleOrder-2; k > 0; k -= 2) {
        // Bottom vertices
        for (int i = 0; i != 2; ++cnt, ++i)
            std::cout << nodes_o3[cnt] << ": " << pos_quad[0] + i*k  << "\n";
        // Top vertices
        for (int i = 0; i != 2; ++cnt, ++i)
            std::cout << nodes_o3[cnt] << ": " << pos_quad[1] - i*k  << "\n";

        // Bottom edge interior nodes
        for (int i = 0; i != k-1; ++i, ++cnt)
            std::cout << nodes_o3[cnt] << ": " << pos_quad[2] + i << "\n"; // (cnt - k) + i ?

        // Right edge interior nodes
        for (int i = 0; i != k-1; ++i, ++cnt)
            std::cout << nodes_o3[cnt] << ": " << pos_quad[3] + i*((eleOrder+1)*(eleOrder+1)) << "\n";

        // Top edge interior nodes
        for (int i = 0; i != k-1; ++i, ++cnt)
            std::cout << nodes_o3[cnt] << ": " << pos_quad[4] - i << "\n";

        // Left edge interior nodes
        for (int i = 0; i != k-1; ++i, ++cnt)
            std::cout << nodes_o3[cnt] << ": " << pos_quad[5] - i*((eleOrder+1)*(eleOrder+1)) << "\n";

        // If the quad we're analyzing is of 2nd order, it'll have a single internal
        // node, so let's account for it
        if (k == 2)
            std::cout << nodes_o3[cnt++] << ": " 
                            << pos_quad[5] + 1 << "\n";

        // Recalculate initial positions for the next, inner quad
        pos_quad[0] += ((eleOrder+1)*(eleOrder+1) + 1);
        pos_quad[1] -= ((eleOrder+1)*(eleOrder+1) + 1);
        pos_quad[2] = pos_quad[0] + 1;
        pos_quad[3] += (eleOrder*(eleOrder+1) + eleOrder);
        pos_quad[4] = pos_quad[1] - 1;
        pos_quad[5] -= (eleOrder*(eleOrder+1) + eleOrder);
    }

    std::cout << "\n\n";

    // Third face loop
    pos_quad[0] = (eleOrder+1)*(eleOrder+1) + (eleOrder+1); // Bottom-left vertex
    pos_quad[1] = eleOrder*(eleOrder+1)*(eleOrder+1) - 2*(eleOrder+1); // Top-right vertex
    pos_quad[2] = 2*(eleOrder+1)*(eleOrder+1) + (eleOrder+1); // Bottom edge
    pos_quad[3] = (eleOrder-1)*(eleOrder+1)*(eleOrder+1) + 2*(eleOrder+1); // Right edge
    pos_quad[4] = (eleOrder-1)*(eleOrder+1)*(eleOrder+1) - 2*(eleOrder+1); // Top edge
    pos_quad[5] = 2*(eleOrder+1)*(eleOrder+1) - 3*(eleOrder+1);  // Left edge
                    

    for (int k = eleOrder-2; k > 0; k -= 2) {
        // Bottom vertices
        for (int i = 0; i != 2; ++cnt, ++i)
            std::cout << nodes_o3[cnt] << ": " << pos_quad[0] + i*(k*(eleOrder+1)*(eleOrder+1))  << "\n";
        // Top vertices
        for (int i = 0; i != 2; ++cnt, ++i)
            std::cout << nodes_o3[cnt] << ": " << pos_quad[1] - i*(k*(eleOrder+1)*(eleOrder+1))  << "\n";

        // Bottom edge interior nodes
        for (int i = 0; i != k-1; ++i, ++cnt)
            std::cout << nodes_o3[cnt] << ": " << pos_quad[2] + i*((eleOrder+1)*(eleOrder+1)) << "\n"; // (cnt - k) + i ?

        // Right edge interior nodes
        for (int i = 0; i != k-1; ++i, ++cnt)
            std::cout << nodes_o3[cnt] << ": " << pos_quad[3] + i*(eleOrder+1) << "\n";

        // Top edge interior nodes
        for (int i = 0; i != k-1; ++i, ++cnt)
            std::cout << nodes_o3[cnt] << ": " << pos_quad[4] - i*((eleOrder+1)*(eleOrder+1)) << "\n";

        // Left edge interior nodes
        for (int i = 0; i != k-1; ++i, ++cnt)
            std::cout << nodes_o3[cnt] << ": " << pos_quad[5] - i*(eleOrder+1) << "\n";

        // If the quad we're analyzing is of 2nd order, it'll have a single internal
        // node, so let's account for it
        if (k == 2)
            std::cout << nodes_o3[cnt++] << ": " 
                            << pos_quad[2] + (eleOrder+1) << "\n";

        // Recalculate initial positions for the next, inner quad
        pos_quad[0] += ((eleOrder+1)*(eleOrder+1) + (eleOrder+1));
        pos_quad[1] -= ((eleOrder+1)*(eleOrder+1) + (eleOrder+1));
        pos_quad[2] = pos_quad[0] + (eleOrder+1)*(eleOrder+1);
        pos_quad[3] -= eleOrder*(eleOrder+1);
        pos_quad[4] = pos_quad[1] - (eleOrder+1)*(eleOrder+1);
        pos_quad[5] += eleOrder*(eleOrder+1);
    }

    std::cout << "\n\n";

    // Fourth face loop
    pos_quad[0] = (eleOrder+1)*(eleOrder+1) + (eleOrder+1) + eleOrder; // Bottom-left vertex
    pos_quad[1] = eleOrder*(eleOrder+1)*(eleOrder+1) - (eleOrder+1) - 1; // Top-right vertex
    pos_quad[2] = (eleOrder+1)*(eleOrder+1) + 2*(eleOrder+1) + eleOrder; // Bottom edge
    pos_quad[3] = 3*(eleOrder+1)*(eleOrder+1) - (eleOrder+1) - 1; // Right edge
    pos_quad[4] = eleOrder*(eleOrder+1)*(eleOrder+1) - 2*(eleOrder+1) - 1; // Top edge
    pos_quad[5] = (eleOrder-2)*(eleOrder+1)*(eleOrder+1) + (eleOrder+1) + eleOrder;  // Left edge
                    

    for (int k = eleOrder-2; k > 0; k -= 2) {
        // Bottom vertices
        for (int i = 0; i != 2; ++cnt, ++i)
            std::cout << nodes_o3[cnt] << ": " << pos_quad[0] + i*(k*(eleOrder+1))  << "\n";
        // Top vertices
        for (int i = 0; i != 2; ++cnt, ++i)
            std::cout << nodes_o3[cnt] << ": " << pos_quad[1] - i*(k*(eleOrder+1))  << "\n";

        // Bottom edge interior nodes
        for (int i = 0; i != k-1; ++i, ++cnt)
            std::cout << nodes_o3[cnt] << ": " << pos_quad[2] + i*(eleOrder+1) << "\n"; // (cnt - k) + i ?

        // Right edge interior nodes
        for (int i = 0; i != k-1; ++i, ++cnt)
            std::cout << nodes_o3[cnt] << ": " << pos_quad[3] + i*((eleOrder+1)*(eleOrder+1)) << "\n";

        // Top edge interior nodes
        for (int i = 0; i != k-1; ++i, ++cnt)
            std::cout << nodes_o3[cnt] << ": " << pos_quad[4] - i*(eleOrder+1) << "\n";

        // Left edge interior nodes
        for (int i = 0; i != k-1; ++i, ++cnt)
            std::cout << nodes_o3[cnt] << ": " << pos_quad[5] - i*((eleOrder+1)*(eleOrder+1)) << "\n";

        // If the quad we're analyzing is of 2nd order, it'll have a single internal
        // node, so let's account for it
        if (k == 2)
            std::cout << nodes_o3[cnt++] << ": " 
                            << pos_quad[5] + (eleOrder+1) << "\n";

        // Recalculate initial positions for the next, inner quad
        pos_quad[0] += ((eleOrder+1)*(eleOrder+1) + (eleOrder+1));
        pos_quad[1] -= ((eleOrder+1)*(eleOrder+1) + (eleOrder+1));
        pos_quad[2] = pos_quad[0] + (eleOrder+1);
        pos_quad[3] += eleOrder*(eleOrder+1);
        pos_quad[4] = pos_quad[1] - (eleOrder+1);
        pos_quad[5] -= eleOrder*(eleOrder+1);
    }

    std::cout << "\n\n";

    // Fifth face loop
    pos_quad[0] = 2*(eleOrder+1)*(eleOrder+1) - 1 - 1; // Bottom-left vertex
    pos_quad[1] = eleOrder*(eleOrder+1)*(eleOrder+1) - eleOrder; // Top-right vertex
    pos_quad[2] = 2*(eleOrder+1)*(eleOrder+1) - 1 - 1 - 1; // Bottom edge
    pos_quad[3] = 3*(eleOrder+1)*(eleOrder+1) - eleOrder; // Right edge
    pos_quad[4] = eleOrder*(eleOrder+1)*(eleOrder+1) - eleOrder + 1; // Top edge
    pos_quad[5] = (eleOrder-1)*(eleOrder+1)*(eleOrder+1) - 1 - 1;  // Left edge
                    

    for (int k = eleOrder-2; k > 0; k -= 2) {
        // Bottom vertices
        for (int i = 0; i != 2; ++cnt, ++i)
            std::cout << nodes_o3[cnt] << ": " << pos_quad[0] - i*k  << "\n";
        // Top vertices
        for (int i = 0; i != 2; ++cnt, ++i)
            std::cout << nodes_o3[cnt] << ": " << pos_quad[1] + i*k  << "\n";

        // Bottom edge interior nodes
        for (int i = 0; i != k-1; ++i, ++cnt)
            std::cout << nodes_o3[cnt] << ": " << pos_quad[2] - i << "\n"; // (cnt - k) + i ?

        // Right edge interior nodes
        for (int i = 0; i != k-1; ++i, ++cnt)
            std::cout << nodes_o3[cnt] << ": " << pos_quad[3] + i*((eleOrder+1)*(eleOrder+1)) << "\n";

        // Top edge interior nodes
        for (int i = 0; i != k-1; ++i, ++cnt)
            std::cout << nodes_o3[cnt] << ": " << pos_quad[4] + i << "\n";

        // Left edge interior nodes
        for (int i = 0; i != k-1; ++i, ++cnt)
            std::cout << nodes_o3[cnt] << ": " << pos_quad[5] - i*((eleOrder+1)*(eleOrder+1)) << "\n";

        // If the quad we're analyzing is of 2nd order, it'll have a single internal
        // node, so let's account for it
        if (k == 2)
            std::cout << nodes_o3[cnt++] << ": " 
                            << pos_quad[3] + 1 << "\n";

        // Recalculate initial positions for the next, inner quad
        pos_quad[0] += ((eleOrder+1)*(eleOrder+1) - 1);
        pos_quad[1] -= ((eleOrder+1)*(eleOrder+1) - 1);
        pos_quad[2] = pos_quad[0] - 1;
        pos_quad[3] += ((eleOrder+1)*(eleOrder+1) + 1);
        pos_quad[4] = pos_quad[1] + 1;
        pos_quad[5] -= ((eleOrder+1)*(eleOrder+1) + 1);
    }

    std::cout << "\n\n";

    // Sixth face loop
    pos_quad[0] = eleOrder*(eleOrder+1)*(eleOrder+1) + (eleOrder+1) + 1; // Bottom-left vertex
    pos_quad[1] = eleOrder*(eleOrder+1)*(eleOrder+1) + eleOrder*(eleOrder+1) - 1 - 1; // Top-right vertex
    pos_quad[2] = eleOrder*(eleOrder+1)*(eleOrder+1) + (eleOrder+1) + 1 + 1; // Bottom edge
    pos_quad[3] = eleOrder*(eleOrder+1)*(eleOrder+1) + 2*(eleOrder+1) + (eleOrder-1); // Right edge
    pos_quad[4] = (eleOrder+1)*(eleOrder+1)*(eleOrder+1)-1 - (eleOrder+1) - 1 - 1; // Top edge
    pos_quad[5] = eleOrder*(eleOrder+1)*(eleOrder+1) + (eleOrder-2)*(eleOrder+1) + 1;  // Left edge
                    

    for (int k = eleOrder-2; k > 0; k -= 2) {
        // Bottom vertices
        for (int i = 0; i != 2; ++cnt, ++i)
            std::cout << nodes_o3[cnt] << ": " << pos_quad[0] + i*k  << "\n";
        // Top vertices
        for (int i = 0; i != 2; ++cnt, ++i)
            std::cout << nodes_o3[cnt] << ": " << pos_quad[1] - i*k  << "\n";

        // Bottom edge interior nodes
        for (int i = 0; i != k-1; ++i, ++cnt)
            std::cout << nodes_o3[cnt] << ": " << pos_quad[2] + i << "\n"; // (cnt - k) + i ?

        // Right edge interior nodes
        for (int i = 0; i != k-1; ++i, ++cnt)
            std::cout << nodes_o3[cnt] << ": " << pos_quad[3] + i*(eleOrder+1) << "\n";

        // Top edge interior nodes
        for (int i = 0; i != k-1; ++i, ++cnt)
            std::cout << nodes_o3[cnt] << ": " << pos_quad[4] - i << "\n";

        // Left edge interior nodes
        for (int i = 0; i != k-1; ++i, ++cnt)
            std::cout << nodes_o3[cnt] << ": " << pos_quad[5] - i*(eleOrder+1) << "\n";

        // If the quad we're analyzing is of 2nd order, it'll have a single internal
        // node, so let's account for it
        if (k == 2)
            std::cout << nodes_o3[cnt++] << ": " 
                            << pos_quad[5] + 1 << "\n";

        // Recalculate initial positions for the next, inner quad
        pos_quad[0] += ((eleOrder+1) + 1);
        pos_quad[1] -= ((eleOrder+1) + 1);
        pos_quad[2] = pos_quad[0] + 1;
        pos_quad[3] += eleOrder;
        pos_quad[4] = pos_quad[1] - 1;
        pos_quad[5] -= eleOrder;
    }

    // TODO Check for the single internal hexa node
    // update the outer (hexa) loop control variable and the initial positions
    // of the inner hexa
}