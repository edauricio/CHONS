#ifndef GMSH_ELEMENTS_H
#define GMSH_ELEMENTS_H

#include <vector>
#include <utility>
#include <map>
#include "ElementEnumInfo.h"

namespace CHONS {

// Map for element type-element order, according to Gmsh numbering definition
// Currently, Gmsh defines/supports 140 types...
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
    {eQuad, 2},     // 16 Serendipity
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
    {eQuad, 3},     // 36
    {eQuad, 4},     // 37
    {eQuad, 5},     // 38
    {eQuad, 3},     // 39 Serendipity
    {eQuad, 4},     // 40 Serendipity
    {eQuad, 5},     // 41 Serendipity
    {},     // 42
    {},     // 43
    {},     // 44
    {},     // 45
    {},     // 46
    {},     // 47
    {},     // 48
    {},     // 49
    {},     // 50
    {},     // 51
    {},     // 52
    {},     // 53
    {},     // 54
    {},     // 55
    {},     // 56
    {},     // 57
    {},     // 58
    {},     // 59
    {},     // 60
    {},     // 61
    {},     // 62
    {},     // 63
    {},     // 64
    {},     // 65
    {},     // 66
    {},     // 67
    {},     // 68
    {},     // 69
    {},     // 70
    {},     // 71
    {},     // 72
    {},     // 73
    {},     // 74
    {},     // 75
    {},     // 76
    {},     // 77
    {},     // 78
    {},     // 79
    {},     // 80
    {},     // 81
    {},     // 82
    {},     // 83
    {},     // 84
    {},     // 85
    {},     // 86
    {},     // 87
    {},     // 88
    {},     // 89
    {},     // 90
    {},     // 91
    {eHexa, 3},     // 92
    {},     // 93
    {},     // 94
    {},     // 95
    {},     // 96
    {},     // 97
    {},     // 98
    {},     // 99
    {}     // 100
};

// A vector with the nodes defining each edge on a 3D element, according to its
// type
std::map<ElementType, std::vector<std::vector<int>>> GmshEdgesOn3DElements = {
    // Edge definition on eHexa element
    std::make_pair(eHexa, std::vector<std::vector<int>>{
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
    std::make_pair(eTetra, std::vector<std::vector<int>>{
                        {0, 1},
                        {1, 2},
                        {2, 0},
                        {0, 3},
                        {2, 3},
                        {1, 3}
                    }),
    // Edge definition on ePrism element
    std::make_pair(ePrism, std::vector<std::vector<int>>{
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
    std::make_pair(ePyram, std::vector<std::vector<int>>{
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
// type (see gmsh user guide 9.2.1 for more info)
std::map<ElementType, std::vector<std::vector<int>>> GmshFacesOn3DElements = {
    // Face definitions on eHexa element
    std::make_pair(eHexa, std::vector<std::vector<int>>{
                        {0, 3, 2, 1},
                        {0, 1, 5, 4},
                        {0, 4, 7, 3},
                        {1, 2, 6, 5},
                        {2, 3, 7, 6},
                        {4, 5, 6, 7}
                    }),
    // Face definitions on eTetra element
    std::make_pair(eTetra, std::vector<std::vector<int>>{
                        {0, 2, 1},
                        {0, 3, 2},
                        {1, 2, 3},
                        {0, 1, 3}
                    }),
    // Face definitions on ePrism element
    std::make_pair(ePrism, std::vector<std::vector<int>>{
                        {0, 1, 4, 3},
                        {0, 3, 5, 2},
                        {1, 2, 5, 4},
                        {0, 2, 1},
                        {3, 4, 5}                        
                    }),
    // Face definitions on ePyram element
    std::make_pair(ePyram, std::vector<std::vector<int>>{
                        {0, 3, 2, 1},
                        {0, 1, 4},
                        {0, 4, 3},
                        {1, 2, 4},
                        {2, 3, 4}
                    })
};

// A vector containing the initial position of the edge interior nodes, for each
// of the faces defined on GmshFacesOn3DElements, relative to the number of
// corner/defining nodes and the element order
// That is, the first edge interior node of the first edge of 1st face starts at
// numEdges + 1; then we loop through these interior nodes for eleOrder-1 times
// Then, the first edge interior node of the second edge of the 1st face starts
// at numEdges + 5; then we loop through it for eleOrder-1 times, and so on...
// Note that due to Gmsh node ordering (it accounts edges from a vertex-vertex
// perspective, from lesser to greater ones, see gmsh user guide), some
// loops will increment 1, while others starts at "the higher end" and
// decrements 1; for a full description, please check the algorithm that uses
// the mapping below in MeshReader.cpp
std::map<ElementType, std::vector<std::vector<int>> >
                                            GmshInteriorENodes3DElements = {
    std::make_pair(eHexa, std::vector<std::vector<int>>{
                        {1, 5, 3, 0},
                        {0, 4, 8, 2},
                        {2, 9, 7, 1},
                        {3, 6, 10, 4},
                        {5, 7, 11, 6},
                        {8, 10, 11, 9}
                    }),

    std::make_pair(eTetra, std::vector<std::vector<int>>{
                        {},
                        {},
                        {},
                        {}
                    }),

    std::make_pair(ePrism, std::vector<std::vector<int>>{
                        {},
                        {},
                        {},
                        {},
                        {}
                    }),

    std::make_pair(ePyram, std::vector<std::vector<int>>{
                        {},
                        {},
                        {},
                        {},
                        {}
                    })

};

} // end of CHONS namespace

#endif