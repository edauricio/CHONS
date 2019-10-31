#include "MeshModule/ElementComposite.h"
#include "MeshModule/ElementEnumInfo.h"
#include <iostream>

namespace CHONS {


//------------ Element Member Function Definitions -------------//
Element::Element(const ElementInfo& ein) {
    s_type = ein.type;
    s_tag = ein.tag;
    s_eleOrder = ein.eleOrder;
    s_polyOrder = ein.polyOrder;
}

void Element::AddPrimitive(Element* ele) {
    std::cout << "Primitive elements can't add primitives.\n";
    exit(-1);
}

void Element::RemovePrimitive(Element* ele) {
    std::cout << "Primitive elements don't have primitives to remove.\n";
    exit(-1);
}

void Element::AddSharing(Element* ele) {
    std::cout << "This composite element is not shared.\n";
    exit(-1);
}

std::map<ElementType, std::list<Element*> > Element::GetPrimitives() {
    return std::map<ElementType, std::list<Element*> >();
}

std::list<Element*> Element::GetSharing() {
    return std::list<Element*>();
}

Point* Element::GetInteriorPoints() {
    return nullptr;
}

Point* Element::GetInterfacePoints() {
    return nullptr;
}

Point* Element::GetPoints() {
    return nullptr;
}

//------------ End of Element Member Function Definitions -------------//


//------------ Node Member Function Definitions -------------//

Node::Node(const ElementInfo& ein) : Element(ein) {
    s_eleOrder = 0;
    s_polyOrder = -1;
    if (!ein.coords.empty())
        s_coords = ein.coords;
    else {
        std::cout << "Empty Node can't be created.\n";
        exit(-1);
    }
}

void Node::AddSharing(Element* ele) {
    if (ele && (ele->GetType() != eNode))
        s_sharingElements.push_back(ele);
    else {
        std::cout << "Invalid element to share a Node.\n";
        exit(-1);
    }
}

std::list<Element*> Node::GetSharing() {
    return s_sharingElements;
}

//------------ End of Node Member Function Definitions -------------//

//------------ Line Member Function Definitions -------------//

Line::Line(const ElementInfo& ein) : Element(ein) {
    if (ein.prims.empty() || (ein.prims.size() < 2)) { 
        std::cout << "Wrong number of primitives for Line creation\n"; 
        exit(-1); 
    }
    for (auto prim : ein.prims)
        AddPrimitive(prim);
}

void Line::AddPrimitive(Element* ele) {
    if (ele && (ele->GetType() == eNode)) {
        auto it = s_primitives.find(ele->GetType());
        if (it == s_primitives.end())
            it = (s_primitives.emplace(ele->GetType(), 
                                std::list<Element*>())).first;
        it->second.push_back(ele);
        ele->AddSharing(this);
    } else {
        std::cout << "Invalid primitive element for Line.\n";
        exit(-1);
    }
}

void Line::RemovePrimitive(Element* ele) {
    if (ele) {
        auto it = s_primitives.find(ele->GetType());
        if (it != s_primitives.end())
            it->second.remove(ele);
        else
            std::cout << "Element isn't a primitive of Line. Double check?\n";
    } else
        std::cout << "Invalid primitive element to remove from Line.\n";        
}

void Line::AddSharing(Element* ele) {
    if ((ele->GetType() == eNode) || (ele->GetType() == eLine)) {
        std::cout << "Invalid element to share a Line.\n";
        exit(-1);
    } else
        s_sharingElements.push_back(ele);
}

std::map<ElementType, std::list<Element*> > Line::GetPrimitives() {
    return s_primitives;
}

std::list<Element*> Line::GetSharing() {
    return s_sharingElements;
}

//------------ End of Line Member Function Definitions -------------//


//------------ Quad Member Function Definitions -------------//

Quad::Quad(const ElementInfo& ein) : Element(ein) {
    if (ein.prims.empty() || (ein.prims.size() < 4)) {
        std::cout << "Wrong number of primitives for Quad creation.\n";
        exit(-1);
    }        
    for (auto it = ein.prims.begin(); it != ein.prims.end(); it++) {
        AddPrimitive(*it);
    }
}

void Quad::AddPrimitive(Element* ele) {
    if (ele && ((ele->GetType() == eNode) || (ele->GetType() == eLine))) {
        auto it = s_primitives.find(ele->GetType());
        if (it == s_primitives.end()) {
            it = (s_primitives.emplace(ele->GetType(), 
                        std::list<Element*>())).first;
        }
        it->second.push_back(ele);
        ele->AddSharing(this);
    } else {
        std::cout << "Invalid primitive for Quad.\n";
        exit(-1);
    }
}

void Quad::RemovePrimitive(Element* ele) {
    if (ele) {
        auto it = s_primitives.find(ele->GetType());
        if (it != s_primitives.end())
            it->second.remove(ele);
        else
            std::cout << "Element isn't a primitive of Quad. Double check?\n";
    } else
        std::cout << "Invalid primitive element to remove from Quad.\n";
}

void Quad::AddSharing(Element* ele) {
    if (ele && ((ele->GetType() == eHexa) || (ele->GetType() == ePrism)
                || (ele->GetType() == ePyram)))
        s_sharingElements.push_back(ele);
    else {
        std::cout << "Invalid element to share a Quad.\n";
        exit(-1);
    }
}

std::map<ElementType, std::list<Element*> > Quad::GetPrimitives() {
    return s_primitives;
}

std::list<Element*> Quad::GetSharing() {
    return s_sharingElements;
}

//------------ End of Quad Member Function Definitions -------------//

//------------ Tri Member Function Definitions -------------//

Tri::Tri(const ElementInfo& ein) : Element(ein) {
    if (ein.prims.empty() || (ein.prims.size() < 3)) {
        std::cout << "Wrong number of primitives for Tri creation.\n";
        exit(-1);
    }
    for (auto prim : ein.prims)
        AddPrimitive(prim);
}

void Tri::AddPrimitive(Element* ele) {
    if (ele && ((ele->GetType() == eLine) || (ele->GetType() == eNode))) {
        auto it = s_primitives.find(ele->GetType());
        if (it == s_primitives.end())
            it = (s_primitives.emplace(ele->GetType(), 
                        std::list<Element*>())).first;
        it->second.push_back(ele);
        ele->AddSharing(this);
    } else {
        std::cout << "Invalid primitive element for Tri.\n";
        exit(-1);
    }
}

void Tri::RemovePrimitive(Element* ele) {
    if (ele) {
        auto it = s_primitives.find(ele->GetType());
        if (it != s_primitives.end())
            it->second.remove(ele);
        else
            std::cout << "Element isn't a primitive of Tri. Double check?\n";
    } else
        std::cout << "Invalid primitive element to remove from Tri.\n";
}

void Tri::AddSharing(Element* ele) {
    if (ele && ((ele->GetType() == eTetra)) || (ele->GetType() == ePrism)
                || (ele->GetType() == ePyram))
        s_sharingElements.push_back(ele);
    else {
        std::cout << "Invalid element to share a Tri.\n";
        exit(-1);
    }
}

std::map<ElementType, std::list<Element*> > Tri::GetPrimitives() {
    return s_primitives;
}

std::list<Element*> Tri::GetSharing() {
    return s_sharingElements;
}

//------------ End of Tri Member Function Definitions -------------//

//------------ Hexa Member Function Definitions -------------//

Hexa::Hexa(const ElementInfo& ein) : Element(ein) {
    if (ein.prims.empty() || (ein.prims.size() < 6)) {
        std::cout << "Wrong number of primitives for Hexa creation.\n";
        exit(-1);
    } else {
        for (auto prim : ein.prims)
            AddPrimitive(prim);
    }
}

void Hexa::AddPrimitive(Element* ele) {
    if (ele && ((ele->GetType() == eQuad) || (ele->GetType() == eNode))) {
        auto it = s_primitives.find(ele->GetType());
        if (it == s_primitives.end())
            it = (s_primitives.emplace(ele->GetType(),
                        std::list<Element*>())).first;
        it->second.push_back(ele);
        ele->AddSharing(this);
    } else {
        std::cout << "Invalid primitive element for Hexa.\n";
        exit(-1);
    }
}

void Hexa::RemovePrimitive(Element* ele) {
    if (ele) {
        auto it = s_primitives.find(ele->GetType());
        if (it != s_primitives.end())
            it->second.remove(ele);
        else
            std::cout << "Element is not a primitive of Hexa. Double check?\n";
    } else
        std::cout << "Invalid primitive element to remove from Hexa.\n";
}

std::map<ElementType, std::list<Element*> > Hexa::GetPrimitives() {
    return s_primitives;
}


//------------ End of Hexa Member Function Definitions -------------//

//------------ Tetra Member Function Definitions -------------//

Tetra::Tetra(const ElementInfo& ein) : Element(ein) {
    if (ein.prims.empty() || (ein.prims.size() < 4)) {
        std::cout << "Wrong number of primitive elements for Tetra creation.\n";
        exit(-1);
    }
    for (auto prim : ein.prims)
        AddPrimitive(prim);
}

void Tetra::AddPrimitive(Element* ele) {
    if (ele && ((ele->GetType() == eTri) || (ele->GetType() == eNode))) {
        auto it = s_primitives.find(ele->GetType());
        if (it == s_primitives.end())
            it = (s_primitives.emplace(ele->GetType(), 
                        std::list<Element*>())).first;
        it->second.push_back(ele);
        ele->AddSharing(this);
    } else {
        std::cout << "Invalid primitive element for Tetra.\n";
        exit(-1);
    }
}

void Tetra::RemovePrimitive(Element* ele) {
    if (ele) {
        auto it = s_primitives.find(ele->GetType());
        if (it != s_primitives.end())
            it->second.remove(ele);
        else
            std::cout << "Element is not a primitive of Tetra. Double check?\n";
    } else
        std::cout << "Invalid primitive element to remove from Tetra.\n";
}

std::map<ElementType, std::list<Element*> > Tetra::GetPrimitives() {
    return s_primitives;
}

//------------ End of Tetra Member Function Definitions -------------//

//------------ Prism Member Function Definitions -------------//

Prism::Prism(const ElementInfo& ein) : Element(ein) {
    if (ein.prims.empty() || (ein.prims.size() < 5)) {
        std::cout << "Wrong number of primitive elements for Tetra creation.\n";
        exit(-1);
    }
    for (auto prim : ein.prims)
        AddPrimitive(prim);
}

void Prism::AddPrimitive(Element* ele) {
    if (ele && ((ele->GetType() == eTri) || (ele->GetType() == eQuad)
                || (ele->GetType() == eNode))) {
        auto it = s_primitives.find(ele->GetType());
        if (it == s_primitives.end())
            it = (s_primitives.emplace(ele->GetType(), 
                        std::list<Element*>())).first;
        it->second.push_back(ele);
        ele->AddSharing(this);
    } else {
        std::cout << "Invalid primitive element for Tetra.\n";
        exit(-1);
    }
}

void Prism::RemovePrimitive(Element* ele) {
    if (ele) {
        auto it = s_primitives.find(ele->GetType());
        if (it != s_primitives.end())
            it->second.remove(ele);
        else
            std::cout << "Element is not a primitive of Tetra. Double check?\n";
    } else
        std::cout << "Invalid primitive element to remove from Tetra.\n";
}

std::map<ElementType, std::list<Element*> > Prism::GetPrimitives() {
    return s_primitives;
}

//------------ End of Prism Member Function Definitions -------------//

//------------ Pyram Member Function Definitions -------------//

Pyram::Pyram(const ElementInfo& ein) : Element(ein) {
    if (ein.prims.empty() || (ein.prims.size() < 5)) {
        std::cout << "Wrong number of primitive elements for Tetra creation.\n";
        exit(-1);
    }
    for (auto prim : ein.prims)
        AddPrimitive(prim);
}

void Pyram::AddPrimitive(Element* ele) {
    if (ele && ((ele->GetType() == eTri) || (ele->GetType() == eQuad)
                || (ele->GetType() == eNode))) {
        auto it = s_primitives.find(ele->GetType());
        if (it == s_primitives.end())
            it = (s_primitives.emplace(ele->GetType(), 
                        std::list<Element*>())).first;
        it->second.push_back(ele);
        ele->AddSharing(this);
    } else {
        std::cout << "Invalid primitive element for Tetra.\n";
        exit(-1);
    }
}

void Pyram::RemovePrimitive(Element* ele) {
    if (ele) {
        auto it = s_primitives.find(ele->GetType());
        if (it != s_primitives.end())
            it->second.remove(ele);
        else
            std::cout << "Element is not a primitive of Tetra. Double check?\n";
    } else
        std::cout << "Invalid primitive element to remove from Tetra.\n";
}

std::map<ElementType, std::list<Element*> > Pyram::GetPrimitives() {
    return s_primitives;
}

//------------ End of Pyram Member Function Definitions -------------//


} // end of CHONS namespace