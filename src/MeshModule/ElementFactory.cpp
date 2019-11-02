#include "MeshModule/ElementFactory.h"
#include <iostream>
#include <string>

namespace CHONS {

// bool ElementFactory::s_created = false;
// std::map<ElementType, std::unordered_map<size_t, Element*> > 
//     ElementFactory::s_elements = std::map<ElementType, 
//                 std::unordered_map<size_t, Element*> >();
ElementFactory* ElementFactory::s_singleInstance = nullptr;

// DELETE THIS -- FOR DEBUGGING PURPOSES ONLY
void ElementFactory::RunThrough() {
    std::vector<std::string> EnumToString{"eNode", "eLine", "eQuad", "eTri"};
    for (auto& type_map : s_elements) {
        std::cout << "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n";
        std::cout << "Element type: " << EnumToString[type_map.first] << "\n";
        for (auto& tag_ele : type_map.second) {
            std::cout << "Tag: " << tag_ele.first << "\n";
            std::cout << "Primitives:\n";
            for (auto& prims : tag_ele.second->GetPrimitives()) {
                std::cout << "\ttype: " << EnumToString[prims.first] << "\n";;
                std::cout << "\ttag: ";
                for (auto& primtag : prims.second)
                    std::cout << primtag->GetTag() << " ";
                std::cout << "\n";
            }
        }
    }
}


ElementFactory* ElementFactory::GetInstance() {
    if (!s_singleInstance)
        s_singleInstance = new ElementFactory();

    return s_singleInstance;
}


Element* ElementFactory::GetElement(const ElementInfo& ein) {
    Element* ele;
    // TODO: Too many find()'s in this function...
    // Is there a way to improve it?
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
          break;

        case eLine:
          ele = new Line(ein);
          break;

        case eQuad:
          ele = new Quad(ein);
          break;

        case eTri:
          ele = new Tri(ein);
          break;

        case eHexa:
          ele = new Hexa(ein);
          break;

        case eTetra:
          ele = new Tetra(ein);
          break;

        case ePrism:
          ele = new Prism(ein);
          break;

        case ePyram:
          ele = new Pyram(ein);
          break;

        default:
          std::cout << "Undefined element for creation.\n";
          exit(-1);
    }
    return ele;
}

} // end of CHONS namespace