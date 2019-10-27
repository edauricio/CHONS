#ifndef GMSH_ELEMENTS_H
#define GMSH_ELEMENTS_H

#include <vector>
#include <utility>
#include "ElementEnumInfo.h"

namespace CHONS {

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
};

} // end of CHONS namespace

#endif