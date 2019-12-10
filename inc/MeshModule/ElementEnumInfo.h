// Definitions of enum for Element Type identification and struct to 
// group Element Info to be created/retrieved.

#ifndef ELEMENT_NUM_INFO_H
#define ELEMENT_NUM_INFO_H

#include <cstdlib>
#include <vector>
#include <list>

namespace CHONS {

class Element;

enum ElementType {
    eNode,
    eLine,
    eQuad,
    eTri,
    eHexa,
    eTetra,
    ePrism,
    ePyram,
    MAX_NUMBER_OF_TYPES
};

struct ElementInfo {
    void clear() { coords.clear(); interfaces.clear(); nodes.clear(); }
    size_t tag;
    ElementType type;
    unsigned short int eleOrder;
    unsigned short int polyOrder;
    //int bndTag;
    std::vector<double> coords;
    std::vector<Element*> interfaces;
    std::vector<Element*> nodes;
};

} // end of CHONS namespace

#endif