#include "MeshModule/ElementFactory.h"
#include <iostream>

namespace CHONS {

bool ElementFactory::s_created = false;
std::map<ElementType, std::unordered_map<size_t, Element*> > 
    ElementFactory::s_elements = std::map<ElementType, 
                std::unordered_map<size_t, Element*> >();


Element* ElementFactory::GetElement(const ElementInfo& ein) {
    Element* ele;

    auto itm = s_elements.find(ein.type);
    if (itm != s_elements.end()) {
        auto its = itm->second.find(ein.tag);
        if (its != itm->second.end()) {
            ele = its->second;
            s_created = false;
        } else {
            ele = create(ein);
            itm->second.emplace(ein.tag, ele);
            s_created = true;
        }
    } else {
        s_elements.emplace(ein.type, std::unordered_map<size_t, Element*>());
        ele = create(ein);
        s_elements.find(ein.type)->second.emplace(ein.tag, ele);
        s_created = true;
    }

    return ele;
}

Element* ElementFactory::create(const ElementInfo& ein) {
    Element* ele;
    switch (ein.type) {
        case eNode:
          ele = new Node(ein);

        case eLine:
          ele = new Line(ein);

        case eQuad:
          ele = new Quad(ein);

        case eTri:
          ele = new Tri(ein);

        case eHexa:
          ele = new Hexa(ein);

        case eTetra:
          ele = new Tetra(ein);

        case ePrism:
          ele = new Prism(ein);

        case ePyram:
          ele = new Pyram(ein);

        default:
          std::cout << "Undefined element for creation.\n";
          exit(-1);
    }
    return ele;
}

} // end of CHONS namespace