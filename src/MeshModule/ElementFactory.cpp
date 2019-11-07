#include "MeshModule/ElementFactory.h"
#include "boost/assert.hpp"
#include <iostream>
#include <string>
#include <memory>

namespace CHONS {

// bool ElementFactory::s_created = false;
// std::map<ElementType, std::unordered_map<size_t, Element*> > 
//     ElementFactory::s_elements = std::map<ElementType, 
//                 std::unordered_map<size_t, Element*> >();
ElementFactory* ElementFactory::s_singleInstance = nullptr;

// DELETE THIS -- FOR DEBUGGING PURPOSES ONLY
// void ElementFactory::RunThrough() {
//     std::vector<std::string> EnumToString{"eNode", "eLine", "eQuad", "eTri"};
//     for (int i = 0; i != s_elements.size(); ++i) {
//         if (s_elements[i].empty()) continue;
//         std::cout << "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n";
//         for (auto& tag_ele : s_elements[i]) {            
//             std::cout << "Element type: " << EnumToString[i] << "\n";
//                 std::cout << "Tag: " << tag_ele.first << "\n";
//                 std::cout << "Primitives:\n";
//                 for (auto& prims : tag_ele.second->GetPrimitives()) {
//                     std::cout << "\ttype: " << EnumToString[prims.first] << "\n";;
//                     std::cout << "\ttag: ";
//                     for (auto& primtag : prims.second)
//                         std::cout << primtag->GetTag() << " ";
//                     std::cout << "\n";
//                 }
//         }
//     }
// }

ElementFactory::ElementFactory() : s_elements(MAX_NUMBER_OF_TYPES),
                           s_orderedElements(MAX_NUMBER_OF_TYPES) {

}


ElementFactory* ElementFactory::GetInstance() {
    if (!s_singleInstance)
        s_singleInstance = new ElementFactory();

    return s_singleInstance;
}

Element* ElementFactory::GetElement(const ElementInfo& ein) {
    try {
        return s_elements[ein.type].at(ein.tag);
    } catch (...) {
        std::cerr << __func__ << ": Couldn't find element of type " 
                    << ein.type << ", tag " << ein.tag << "\n";
        std::abort();
    }
}

bool ElementFactory::OrderElement(const ElementInfo& ein) {
    // If element already exists, do nothing and return false status
    if (s_elements[ein.type].count(ein.tag))
        return false; // should we throw and exception in this case?

    // Otherwise, try to add it to the order
    BOOST_ASSERT_MSG(s_orderedElements[ein.type].emplace(ein.tag, ein).second,
                    "Element has already been added to the order");
    return true;
}

template <typename T>
void ElementFactory::PlaceOrder(std::unordered_map<size_t, ElementInfo>& tag_einfo) {
    std::allocator<T> alloc;
    auto ele = alloc.allocate(tag_einfo.size());
    for (auto it = tag_einfo.begin(); it != tag_einfo.end(); ) {
        alloc.construct(ele, it->second);
        s_elements[it->second.type].emplace(it->first, ele++);
        tag_einfo.erase(it++);
    }
}

void ElementFactory::PlaceOrder() {
    for (int i = 0; i != s_orderedElements.size(); ++i) {
        if (!s_orderedElements[i].empty()) {
            // allocate given number of elements of given type
            switch (i) {
                case 0: // Node
                    PlaceOrder<Node>(s_orderedElements[i]);
                    s_orderedElements[i].clear();
                    break;

                case 1: // Line
                    PlaceOrder<Line>(s_orderedElements[i]);
                    s_orderedElements[i].clear();
                  break;

                case 2: // Quad
                    PlaceOrder<Quad>(s_orderedElements[i]);
                    s_orderedElements[i].clear();
                  break;

                case 3: // Tri
                    PlaceOrder<Tri>(s_orderedElements[i]);
                    s_orderedElements[i].clear();
                  break;

                case 4: // Hexa
                    PlaceOrder<Hexa>(s_orderedElements[i]);
                    s_orderedElements[i].clear();
                  break;

                case 5: // Tetra
                    PlaceOrder<Tetra>(s_orderedElements[i]);
                    s_orderedElements[i].clear();
                  break;

                case 6: // Prism
                    PlaceOrder<Prism>(s_orderedElements[i]);
                    s_orderedElements[i].clear();
                  break;

                case 7: // Pyram
                    PlaceOrder<Pyram>(s_orderedElements[i]);
                    s_orderedElements[i].clear();
                  break;
            }
        }
    }
}


} // end of CHONS namespace