#ifndef GMSH_ELEMENTS_H
#define GMSH_ELEMENTS_H

#include <vector>
#include <utility>
#include <map>
#include "ElementEnumInfo.h"

namespace CHONS {

// Map for element type-element order, according to Gmsh numbering definition
std::vector<std::pair<ElementType, int>> GmshElementsMapping = {
    {},
    {eLine, 1},     // 1
    {eTri, 1},      // 2
    {eQuad, 1},     // 3
    {eTetra, 1},    // 4
    {eHexa, 1},     // 5
    {ePrism, 1},    // 6
    {ePyram, 1},    // 7
    {eLine, 2},     // 8
    {eTri, 2},      // 9
    {eQuad, 2},     // 10
    {eTetra, 2},    // 11
    {eHexa, 2},     // 12
    {ePrism, 2},    // 13
    {ePyram, 2},    // 14
    {eNode, 1},     // 15
    {eQuad, 2},     // 16
    {eHexa, 2},     // 17
    {ePrism, 2},     // 18
    {ePyram, 2},     // 19
    {eTri, 3},     // 20
    {eTri, 3},     // 21
    {eTri, 4},     // 22
    {eTri, 4},     // 23
    {eTri, 5},     // 24
    {eTri, 5},     // 25
    {eLine, 3},     // 26
    {eLine, 4},     // 27
    {eLine, 5},     // 28
    {eTetra, 3},     // 29
    {eTetra, 4},     // 30
    {eTetra, 5},     // 31
    {},     // 32
    {},     // 33
    {},     // 34
    {},     // 35
    {},     // 36
    {},     // 37
    {},     // 38
    {eQuad, 3},     // 39
};

// A vector with the nodes defining each edge on a 3D element, according to its
// type
std::map<ElementType, std::vector<std::vector<size_t>>> GmshEdgesOn3DElements = {
    // Edge definition on eHexa element
    std::make_pair(eHexa, std::vector<std::vector<size_t>>{
                        {0, 1},
                        {0, 3},
                        {0, 4},
                        {1, 2},
                        {1, 5},
                        {2, 3},
                        {2, 6},
                        {3, 7},
                        {4, 5},
                        {4, 7},
                        {5, 6},
                        {6, 7}
                    }),
    // Edge definition on eTetra element
    std::make_pair(eTetra, std::vector<std::vector<size_t>>{
                        {0, 1},
                        {1, 2},
                        {2, 0},
                        {0, 3},
                        {2, 3},
                        {1, 3}
                    }),
    // Edge definition on ePrism element
    std::make_pair(ePrism, std::vector<std::vector<size_t>>{
                        {0, 1},
                        {0, 2},
                        {0, 3},
                        {1, 2},
                        {1, 4},
                        {2, 5},
                        {3, 4},
                        {3, 5},
                        {4, 5}
                    }),
    // Edge definition on ePyram element
    std::make_pair(ePyram, std::vector<std::vector<size_t>>{
                        {0, 1},
                        {0, 3},
                        {0, 4},
                        {1, 2},
                        {1, 4},
                        {2, 3},
                        {2, 4},
                        {3, 4}
                    })
};

// A vector with the nodes defining each face on a 3D element, according to its
// type
std::map<ElementType, std::vector<std::vector<size_t>>> GmshFacesOn3DElements = {
    // Face definitions on eHexa element
    std::make_pair(eHexa, std::vector<std::vector<size_t>>{
                        {0, 3, 2, 1},
                        {0, 1, 5, 4},
                        {0, 4, 7, 3},
                        {1, 2, 6, 5},
                        {2, 3, 7, 6},
                        {4, 5, 6, 7}
                    }),
    // Face definitions on eTetra element
    std::make_pair(eTetra, std::vector<std::vector<size_t>>{
                        {0, 2, 1},
                        {0, 3, 2},
                        {1, 2, 3},
                        {0, 1, 3}
                    }),
    // Face definitions on ePrism element
    std::make_pair(ePrism, std::vector<std::vector<size_t>>{
                        {0, 1, 4, 3},
                        {0, 3, 5, 2},
                        {1, 2, 5, 4},
                        {0, 2, 1},
                        {3, 4, 5}                        
                    }),
    // Face definitions on ePyram element
    std::make_pair(ePyram, std::vector<std::vector<size_t>>{
                        {0, 3, 2, 1},
                        {0, 1, 4},
                        {0, 4, 3},
                        {1, 2, 4},
                        {2, 3, 4}
                    })
};

} // end of CHONS namespace

#endif