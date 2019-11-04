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
void ElementFactory::RunThrough() {
    std::vector<std::string> EnumToString{"eNode", "eLine", "eQuad", "eTri"};
    for (int i = 0; i != s_elements.size(); ++i) {
        if (s_elements[i].empty()) continue;
        std::cout << "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n";
        for (auto& tag_ele : s_elements[i]) {            
            std::cout << "Element type: " << EnumToString[i] << "\n";
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

// REFACTOR THIS METHOD (only retrieval is allowed now)
Element* ElementFactory::GetElement(const ElementInfo& ein) {
    Element* ele;
    try {
        ele = s_elements[ein.type].at(ein.tag);
    } catch (...) {
        std::cerr << "Couldn't find element of type " << ein.type << ", "
                    "tag " << ein.tag << "\n";
        std::abort();
    }
    // if (!s_elements[ein.type].count(ein.tag))
    //     ele = nullptr;
    // else
    //     ele = s_elements[ein.type].at(ein.tag);

    return ele;


    // auto itm = s_elements.find(ein.type);
    // if (itm != s_elements.end()) {
    //     auto its = itm->second.find(ein.tag);
    //     if (its != itm->second.end()) {
    //         ele = its->second;
    //         s_created = false;
    //     } else {
    //         ele = create(ein);
    //         itm->second.emplace(ein.tag, ele);
    //         s_created = true;
    //     }
    // } else {
    //     s_elements.emplace(ein.type, std::unordered_map<size_t, Element*>());
    //     ele = create(ein);
    //     s_elements.find(ein.type)->second.emplace(ein.tag, ele);
    //     s_created = true;
    // }

    // return ele;
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

void ElementFactory::PlaceOrder() {
    for (int i = 0; i != s_orderedElements.size(); ++i) {
        if (!s_orderedElements[i].empty()) {
            // allocate given number of elements of given type
            switch (i) {
                case 0: // Node
                    { // block to hide einfo variable definition from other cases
                    std::allocator<Node> node_alloc;
                    auto nele = node_alloc.allocate(
                                                s_orderedElements[i].size());

                    // for (auto einfo = s_orderedElements[i].begin();
                    //         einfo != s_orderedElements[i].end();
                    //         ++einfo, ++ele) {
                    for (auto& einfo : (s_orderedElements[i])) {
                        node_alloc.construct(nele, einfo.second);
                        s_elements[i].emplace(einfo.first, nele++);
                    }
                    s_orderedElements[i].clear();
                    }
                    break;

                case 1: // Line
                    {
                    std::allocator<Line> line_alloc;
                    auto lele = line_alloc.allocate(
                                                s_orderedElements[i].size());

                    for (auto& einfo : (s_orderedElements[i])) {
                        line_alloc.construct(lele, einfo.second);
                        s_elements[i].emplace(einfo.first, lele++);
                    }
                    s_orderedElements[i].clear();
                    }
                  break;

                case 2: // Quad
                    {
                    std::allocator<Quad> quad_alloc;
                    auto qele = quad_alloc.allocate(
                                                s_orderedElements[i].size());

                    for (auto& einfo : (s_orderedElements[i])) {
                        quad_alloc.construct(qele, einfo.second);
                        s_elements[i].emplace(einfo.first, qele++);
                    }
                    s_orderedElements[i].clear();
                    }
                  break;

                case 3: // Tri
                    {
                    std::allocator<Tri> tri_alloc;
                    auto tele = tri_alloc.allocate(
                                                s_orderedElements[i].size());

                    for (auto& einfo : (s_orderedElements[i])) {
                        tri_alloc.construct(tele, einfo.second);
                        s_elements[i].emplace(einfo.first, tele++);
                    }
                    s_orderedElements[i].clear();
                    }
                  break;

                case 4: // Hexa
                    {
                    std::allocator<Hexa> hexa_alloc;
                    auto hele = hexa_alloc.allocate(
                                                s_orderedElements[i].size());

                    for (auto& einfo : (s_orderedElements[i])) {
                        hexa_alloc.construct(hele, einfo.second);
                        s_elements[i].emplace(einfo.first, hele++);
                    }
                    s_orderedElements[i].clear();
                    }
                  break;

                case 5: // Tetra
                    {
                    std::allocator<Tetra> tetra_alloc;
                    auto tetele = tetra_alloc.allocate(
                                                s_orderedElements[i].size());

                    for (auto& einfo : (s_orderedElements[i])) {
                        tetra_alloc.construct(tetele, einfo.second);
                        s_elements[i].emplace(einfo.first, tetele++);
                    }
                    s_orderedElements[i].clear();
                    }
                  break;

                case 6: // Prism
                    {
                    std::allocator<Prism> prism_alloc;
                    auto pele = prism_alloc.allocate(
                                                s_orderedElements[i].size());

                    for (auto& einfo : (s_orderedElements[i])) {
                        prism_alloc.construct(pele, einfo.second);
                        s_elements[i].emplace(einfo.first, pele++);
                    }
                    s_orderedElements[i].clear();
                    }
                  break;

                case 7: // Pyram
                    {
                    std::allocator<Pyram> pyram_alloc;
                    auto pyele = pyram_alloc.allocate(
                                                s_orderedElements[i].size());

                    for (auto& einfo : (s_orderedElements[i])) {
                        pyram_alloc.construct(pyele, einfo.second);
                        s_elements[i].emplace(einfo.first, pyele++);
                    }
                    s_orderedElements[i].clear();
                    }
                  break;
            }
        }
    }
}


// Element* ElementFactory::create(const ElementInfo& ein) {
//     Element* ele;
//     switch (ein.type) {
//         case eNode:
//           ele = new Node(ein);
//           break;

//         case eLine:
//           ele = new Line(ein);
//           break;

//         case eQuad:
//           ele = new Quad(ein);
//           break;

//         case eTri:
//           ele = new Tri(ein);
//           break;

//         case eHexa:
//           ele = new Hexa(ein);
//           break;

//         case eTetra:
//           ele = new Tetra(ein);
//           break;

//         case ePrism:
//           ele = new Prism(ein);
//           break;

//         case ePyram:
//           ele = new Pyram(ein);
//           break;

//         default:
//           std::cout << "Undefined element for creation.\n";
//           exit(-1);
//     }
//     return ele;
// }

} // end of CHONS namespace