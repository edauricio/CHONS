#include "MeshModule/ElementComposite.h"
#include "MeshModule/ElementEnumInfo.h"
#include "boost/assert.hpp"
#include <iostream>
#include <algorithm>

namespace CHONS {


//------------ Element Member Function Definitions -------------//
Element::Element(const ElementInfo& ein) {
    s_type = ein.type;
    s_tag = ein.tag;
    s_eleOrder = ein.eleOrder;
    s_polyOrder = ein.polyOrder;
}

void Element::AddNode(Element* ele) {
    std::cout << "Node elements can't add nodes.\n";
    exit(-1);
}

void Element::AddInterface(Element* ele) {
    std::cout << "Node elements have no interface.\n";
    exit(-1);
}

void Element::RemoveNode(Element* ele) {
    std::cout << "Node elements don't have nodes to remove.\n";
    exit(-1);
}

void Element::RemoveInterface(Element* ele) {
    std::cout << "Node elements don't have interfaces to remove.\n";
    exit(-1);
}

void Element::AddSharing(Element* ele) {
    std::cout << "This composite element is not shared.\n";
    exit(-1);
}

std::vector<double> Element::GetCoords() {
    std::cout << "Coordinates can only be requested on nodes.\n";
    exit(-1);
}

std::vector<Element*> Element::GetNodes() {
    std::cout << "Nodes don't have nodes of their own.\n";
    exit(-1);
}

std::vector<Element*> Element::GetInterfaces() {
    std::cout << "Nodes don't have an interface.\n";
    exit(-1);
}

std::vector<Element*> Element::GetSharing() {
    std::cout << "Such element can't be shared with others.\n";
    exit(-1);
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
    BOOST_ASSERT_MSG(!ein.coords.empty(), "Empty Node can't be created");

    s_eleOrder = 0;
    s_polyOrder = -1;
    s_dim = 0;
    s_incompleteElementPrimitivesNr = -1;    

    s_coords = ein.coords;
    
}

void Node::AddSharing(Element* ele) {
    BOOST_ASSERT_MSG(ele && (ele->GetType() != eNode), 
                    "Invalid element to share a Node");
    
    s_sharingElements.push_back(ele);
    
}

std::vector<Element*> Node::GetSharing() {
    return s_sharingElements;
}

std::vector<double> Node::GetCoords() {
    return s_coords;
}

//------------ End of Node Member Function Definitions -------------//

//------------ Line Member Function Definitions -------------//

Line::Line(const ElementInfo& ein) : Element(ein) {
    BOOST_ASSERT_MSG(!ein.nodes.empty() && (ein.nodes.size() >= 2), 
        "Invalid number of nodes for Line creation");

    s_incompleteElementPrimitivesNr = 2;
    s_dim = 1;
    for (auto nodes : ein.nodes)
        AddNode(nodes);

    for (auto ints : ein.interfaces)
        AddInterface(ints);
}

void Line::AddNode(Element* ele) {
    BOOST_ASSERT_MSG(ele && (ele->GetType() == eNode),
            "Invalid element for addition -- Line::AddNode");
        s_nodes.push_back(ele);
}

void Line::AddInterface(Element* ele) {
    BOOST_ASSERT_MSG(ele && (ele->GetType() == eNode), 
                "Invalid element for addition -- Line::AddInterface");

    s_interfaces.push_back(ele);
    ele->AddSharing(this);
    
}

void Line::RemoveNode(Element* ele) {
    BOOST_ASSERT_MSG(ele && (ele->GetType() == eNode), 
        "Invalid element for removal -- Line::RemoveNode");
        s_nodes.erase(
            std::remove_if(s_nodes.begin(),
                        s_nodes.end(),
                        [&](decltype(s_nodes)::value_type& v){ return v==ele;}),
            s_nodes.end());
}

void Line::RemoveInterface(Element* ele) {
    BOOST_ASSERT_MSG(ele && (ele->GetType() == eNode), 
        "Invalid element for removal -- Line::RemoveInterface");
    s_interfaces.erase(std::remove_if(s_interfaces.begin(), s_interfaces.end(),
                [&](decltype(s_interfaces)::value_type& v){return v==ele;}),
                s_interfaces.end());
}

void Line::AddSharing(Element* ele) {
    BOOST_ASSERT_MSG((ele->GetType() != eNode) && (ele->GetType() != eLine), 
        "Invalid element to share a Line.");

        s_sharingElements.push_back(ele);
}

std::vector<Element*> Line::GetNodes() {
    return s_nodes;
}

std::vector<Element*> Line::GetInterfaces() {
    return s_interfaces;
}

std::vector<Element*> Line::GetSharing() {
    return s_sharingElements;
}

//------------ End of Line Member Function Definitions -------------//


//------------ Quad Member Function Definitions -------------//

Quad::Quad(const ElementInfo& ein) : Element(ein) {
    BOOST_ASSERT_MSG(!ein.nodes.empty() && (ein.nodes.size() >= 4), 
        "Invalid number of nodes for Quad creation");
    
    s_incompleteElementPrimitivesNr = 4;
    s_dim = 2;
    for (auto& node : ein.nodes)
        AddNode(node);

    for (auto& ints : ein.interfaces)
        AddInterface(ints);
}

void Quad::AddNode(Element* ele) {
    BOOST_ASSERT_MSG(ele && (ele->GetType() == eNode),
                    "Invalid element for addition -- Quad::AddNode");

    s_nodes.push_back(ele);
}

void Quad::AddInterface(Element* ele) {
    BOOST_ASSERT_MSG(ele && (ele->GetType() == eLine), 
                "Invalid element for addition -- Quad::AddInterface");
    
        s_interfaces.push_back(ele);
        ele->AddSharing(this);
}

void Quad::RemoveNode(Element* ele) {
    BOOST_ASSERT_MSG(ele && (ele->GetType() == eNode), 
                "Invalid element for removal -- Quad::RemoveNode");

        s_nodes.erase(
            std::remove_if(s_nodes.begin(),
                        s_nodes.end(),
                        [&](decltype(s_nodes)::value_type& v){ return v==ele;}),
                s_nodes.end());

}

void Quad::RemoveInterface(Element* ele) {
    BOOST_ASSERT_MSG(ele && (ele->GetType() == eLine), 
                "Invalid element for removal -- Quad::RemoveInterface");

    s_interfaces.erase(std::remove_if(s_interfaces.begin(), s_interfaces.end(),
                    [&](decltype(s_interfaces)::value_type& v){return v==ele;})
                    , s_interfaces.end());
}

void Quad::AddSharing(Element* ele) {
    BOOST_ASSERT_MSG(ele && ((ele->GetType() == eHexa) 
                            || (ele->GetType() == ePrism)
                            || (ele->GetType() == ePyram)),
            "Invalid element to share a Quad");

        s_sharingElements.push_back(ele);
}

std::vector<Element*> Quad::GetNodes() {
    return s_nodes;
}

std::vector<Element*> Quad::GetInterfaces() {
    return s_interfaces;
}

std::vector<Element*> Quad::GetSharing() {
    return s_sharingElements;
}

//------------ End of Quad Member Function Definitions -------------//

//------------ Tri Member Function Definitions -------------//

Tri::Tri(const ElementInfo& ein) : Element(ein) {
    BOOST_ASSERT_MSG(!ein.nodes.empty() && (ein.nodes.size() >= 3), 
                    "Invalid number of nodes for Tri creation");

    s_incompleteElementPrimitivesNr = 3;
    s_dim = 2;

    for (auto& nodes : ein.nodes)
        AddNode(nodes);

    for (auto& ints : ein.interfaces)
        AddInterface(ints);
}

void Tri::AddNode(Element* ele) {
    BOOST_ASSERT_MSG(ele && (ele->GetType() == eNode), 
                "Invalid element for addition -- Tri::AddNode");

    s_nodes.push_back(ele);
}

void Tri::AddInterface(Element* ele) {
    BOOST_ASSERT_MSG(ele && (ele->GetType() == eLine), 
                "Invalid element for addition -- Tri::AddInterface");

        s_interfaces.push_back(ele);
        ele->AddSharing(this);
}

void Tri::RemoveNode(Element* ele) {
    BOOST_ASSERT_MSG(ele && (ele->GetType() == eNode), 
                "Invalid element for removal -- Tri::RemoveNode");
    
    s_nodes.erase(
        std::remove_if(s_nodes.begin(),
                    s_nodes.end(),
                    [&](decltype(s_nodes)::value_type& v){ return v==ele;}),
        s_nodes.end());
}

void Tri::RemoveInterface(Element* ele) {
    BOOST_ASSERT_MSG(ele && (ele->GetType() == eLine), 
                "Invalid element for removal -- Tri::RemoveInterface");

    s_interfaces.erase(
            std::remove_if(s_interfaces.begin(), s_interfaces.end(),
                    [&](decltype(s_interfaces)::value_type& v){return v==ele;}),
            s_interfaces.end());
}

void Tri::AddSharing(Element* ele) {
    BOOST_ASSERT_MSG(ele && ((ele->GetType() == eTetra) 
                            || (ele->GetType() == ePrism)
                            || (ele->GetType() == ePyram)), 
                "Invalid element to share a Tri");
    
    s_sharingElements.push_back(ele);
    
}

std::vector<Element*> Tri::GetNodes() {
    return s_nodes;
}

std::vector<Element*> Tri::GetInterfaces() {
    return s_interfaces;
}

std::vector<Element*> Tri::GetSharing() {
    return s_sharingElements;
}

//------------ End of Tri Member Function Definitions -------------//

//------------ Hexa Member Function Definitions -------------//

Hexa::Hexa(const ElementInfo& ein) : Element(ein) {
    BOOST_ASSERT_MSG(!ein.nodes.empty() && (ein.nodes.size() >= 6), 
                "Invalid number of nodes for Hexa creation");
    
    s_incompleteElementPrimitivesNr = 6;
    s_dim = 3;

    for (auto& nodes : ein.nodes)
        AddNode(nodes);

    for (auto& ints : ein.interfaces)
        AddInterface(ints);
}

void Hexa::AddNode(Element* ele) {
    BOOST_ASSERT_MSG(ele && (ele->GetType() == eNode), 
                "Invalid element for addition -- Hexa::AddNode");

    s_nodes.push_back(ele);
}

void Hexa::AddInterface(Element* ele) {
    BOOST_ASSERT_MSG(ele && (ele->GetType() == eQuad), 
                "Invalid element for addition -- Hexa::AddInterface");
    
        s_interfaces.push_back(ele);
        ele->AddSharing(this);

}

void Hexa::RemoveNode(Element* ele) {
    BOOST_ASSERT_MSG(ele && (ele->GetType() == eNode), 
                "Invalid element for removal -- Hexa::RemoveNode");

    
    s_nodes.erase(
            std::remove_if(s_nodes.begin(),
                    s_nodes.end(),
                    [&](decltype(s_nodes)::value_type& v){ return v==ele;}),
            s_nodes.end());
}

void Hexa::RemoveInterface(Element* ele) {
    BOOST_ASSERT_MSG(ele && (ele->GetType() == eQuad), 
                "Invalid element for removal -- Hexa:RemoveInterface");

    s_interfaces.erase(
        std::remove_if(s_interfaces.begin(), s_interfaces.end(),
            [&](decltype(s_interfaces)::value_type& v){return v==ele;}),
        s_interfaces.end());
}

std::vector<Element*> Hexa::GetNodes() {
    return s_nodes;
}

std::vector<Element*> Hexa::GetInterfaces() {
    return s_interfaces;
}

//------------ End of Hexa Member Function Definitions -------------//

//------------ Tetra Member Function Definitions -------------//

Tetra::Tetra(const ElementInfo& ein) : Element(ein) {
    BOOST_ASSERT_MSG(!ein.nodes.empty() && (ein.nodes.size() >= 4), 
                "Invalid number of nodes for Tetra creation");
    
    s_incompleteElementPrimitivesNr = 4;
    s_dim = 3;

    for (auto& nodes : ein.nodes)
        AddNode(nodes);

    for (auto& ints : ein.interfaces)
        AddInterface(ints);
}

void Tetra::AddNode(Element* ele) {
    BOOST_ASSERT_MSG(ele && (ele->GetType() == eNode), 
                "Invalid element for addition -- Tetra::AddNode");

    s_nodes.push_back(ele);
}

void Tetra::AddInterface(Element* ele) {
    BOOST_ASSERT_MSG(ele && (ele->GetType() == eTri), 
                "Invalid element for addition -- Tetra::AddInterface");
    
    s_interfaces.push_back(ele);
    ele->AddSharing(this);
}

void Tetra::RemoveNode(Element* ele) {
    BOOST_ASSERT_MSG(ele && (ele->GetType() == eNode), 
                "Invalid element for removal -- Tetra::RemoveNode");

    s_nodes.erase(
        std::remove_if(s_nodes.begin(),
                    s_nodes.end(),
                    [&](decltype(s_nodes)::value_type& v){ return v==ele;}),
        s_nodes.end());
}

void Tetra::RemoveInterface(Element* ele) {
    BOOST_ASSERT_MSG(ele && (ele->GetType() == eTri), 
                "Invalid element for removal -- Tetra::RemoveInterface");

    s_interfaces.erase(
        std::remove_if(s_interfaces.begin(), s_interfaces.end(),
            [&](decltype(s_interfaces)::value_type& v){return v==ele;}),
        s_interfaces.end());
}

std::vector<Element*> Tetra::GetNodes() {
    return s_nodes;
}

std::vector<Element*> Tetra::GetInterfaces() {
    return s_interfaces;
}

//------------ End of Tetra Member Function Definitions -------------//

//------------ Prism Member Function Definitions -------------//

Prism::Prism(const ElementInfo& ein) : Element(ein) {
    BOOST_ASSERT_MSG(!ein.nodes.empty() && (ein.nodes.size() >= 5), 
                "Invalid number of nodes for Prism creation");
    
    s_incompleteElementPrimitivesNr = 5;
    s_dim = 3;

    for (auto& nodes : ein.nodes)
        AddNode(nodes);

    for (auto& ints : ein.interfaces)
        AddInterface(ints);
}

void Prism::AddNode(Element* ele) {
    BOOST_ASSERT_MSG(ele && (ele->GetType() == eNode), 
                "Invalid element for addition -- Prism::AddNode");

    s_nodes.push_back(ele);
}

void Prism::AddInterface(Element* ele) {
    BOOST_ASSERT_MSG(ele && ((ele->GetType() == eQuad)
                            || (ele->GetType() == eTri)), 
            "Invalid element for addition -- Prism::AddInterface");
    
    s_interfaces.push_back(ele);
    ele->AddSharing(this);    
}

void Prism::RemoveNode(Element* ele) {
    BOOST_ASSERT_MSG(ele && (ele->GetType() == eNode), 
                "Invalid element for removal -- Prism::RemoveNode");
    
    s_nodes.erase(
        std::remove_if(s_nodes.begin(),
                    s_nodes.end(),
                    [&](decltype(s_nodes)::value_type& v){ return v==ele;}),
        s_nodes.end());

}

void Prism::RemoveInterface(Element* ele) {
    BOOST_ASSERT_MSG(ele && ((ele->GetType() == eQuad)
                                || (ele->GetType() == eTri)), 
            "Invalid element for removal -- Prism::RemoveInterface");

    s_interfaces.erase(
        std::remove_if(s_interfaces.begin(), s_interfaces.end(),
                [&](decltype(s_interfaces)::value_type& v){return v==ele;}),
        s_interfaces.end());
}

std::vector<Element*> Prism::GetNodes() {
    return s_nodes;
}

std::vector<Element*> Prism::GetInterfaces() {
    return s_interfaces;
}

//------------ End of Prism Member Function Definitions -------------//

//------------ Pyram Member Function Definitions -------------//

Pyram::Pyram(const ElementInfo& ein) : Element(ein) {
    BOOST_ASSERT_MSG(!ein.nodes.empty() && (ein.nodes.size() >= 5), 
                "Invalid number of nodes for Pyram creation");
    
    s_incompleteElementPrimitivesNr = 5;
    s_dim = 3;

    for (auto& nodes : ein.nodes)
        AddNode(nodes);

    for (auto& ints : ein.interfaces)
        AddInterface(ints);
}

void Pyram::AddNode(Element* ele) {
    BOOST_ASSERT_MSG(ele && (ele->GetType() == eNode), 
                "Invalid element for addition -- Pyram::AddNode");
    
    s_nodes.push_back(ele);
}

void Pyram::AddInterface(Element* ele) {
    BOOST_ASSERT_MSG(ele && ((ele->GetType() == eTri) 
                            || (ele->GetType() == eQuad)), 
                "Invalid element for addition -- Pyram::AddNode");

    s_interfaces.push_back(ele);
    ele->AddSharing(this);
}

void Pyram::RemoveNode(Element* ele) {
    BOOST_ASSERT_MSG(ele && (ele->GetType() == eNode), 
                "Invalid element for removal -- Pyram::RemoveNode");
    
    s_nodes.erase(
        std::remove_if(s_nodes.begin(), s_nodes.end(),
                    [&](decltype(s_nodes)::value_type& v){ return v==ele;}),
        s_nodes.end());
}

void Pyram::RemoveInterface(Element* ele) {
    BOOST_ASSERT_MSG(ele && ((ele->GetType() == eQuad)
                            || (ele->GetType() == eTri)), 
                "Invalid element for removal -- Pyram::RemoveInterface");

    s_interfaces.erase(
        std::remove_if(s_interfaces.begin(), s_interfaces.end(),
                [&](decltype(s_interfaces)::value_type& v){return v==ele;}),
        s_interfaces.end());
}

std::vector<Element*> Pyram::GetNodes() {
    return s_nodes;
}

std::vector<Element*> Pyram::GetInterfaces() {
    return s_interfaces;
}

//------------ End of Pyram Member Function Definitions -------------//


} // end of CHONS namespace