// Classes for element definition (in terms of Composites of primitive
// structures)

#ifndef ELEMENT_COMPOSITE_H
#define ELEMENT_COMPOSITE_H

#include <vector>
#include <map>
#include <list>
#include <cstdlib>
#include "ElementEnumInfo.h"

namespace CHONS {

class Point; // not yet defined (analytically)

class Element {
    public:
        Element(const ElementInfo&);
        virtual ~Element() = default;
        virtual void AddNode(Element*);
        virtual void AddInterface(Element*);
        virtual void RemoveNode(Element*);
        virtual void RemoveInterface(Element*);
        virtual void AddSharing(Element*);
        virtual std::vector<double> GetCoords();
        virtual std::vector<Element*> GetNodes();
        virtual std::vector<Element*> GetInterfaces();
        virtual std::vector<Element*> GetSharing();
        virtual Point* GetInteriorPoints();
        virtual Point* GetInterfacePoints();
        virtual Point* GetPoints();
        const ElementType& GetType() { return s_type; };
        const size_t& GetTag() { return s_tag; };
        const unsigned short int& GetElementOrder() { return s_eleOrder; };
        const short int& GetPolyOrder() { return s_polyOrder; };
        const short int& GetIncompletePrimNr() { 
                                    return s_incompleteElementPrimitivesNr; }
        void SetPolyOrder(const int& o) { s_polyOrder = o; };

    protected:
        ElementType s_type;
        size_t s_tag;
        unsigned short int s_eleOrder;
        unsigned short int s_dim;
        short int s_incompleteElementPrimitivesNr;
        short int s_polyOrder;
        std::vector<Point*> s_interiorPoints;

};

class Node : public Element {
    public:
        Node(const ElementInfo&);
        virtual void AddSharing(Element*);
        virtual std::vector<Element*> GetSharing();
        virtual std::vector<double> GetCoords();
        virtual ~Node() = default;

    private:
        std::vector<double> s_coords;
        std::vector<Element*> s_sharingElements;

};

class Line : public Element {
    public:
        Line(const ElementInfo&);
        virtual void AddNode(Element*);
        virtual void AddInterface(Element*);
        virtual void RemoveNode(Element*);
        virtual void RemoveInterface(Element*);
        virtual void AddSharing(Element*);
        virtual std::vector<Element*> GetNodes();
        virtual std::vector<Element*> GetInterfaces();
        virtual std::vector<Element*> GetSharing();
        virtual ~Line() = default;

    private:
        std::vector<Element*> s_nodes;
        std::vector<Element*> s_interfaces;
        std::vector<Element*> s_sharingElements;
};

class Quad : public Element {
    public:
        Quad(const ElementInfo&);
        virtual void AddNode(Element*);
        virtual void AddInterface(Element*);
        virtual void RemoveNode(Element*);
        virtual void RemoveInterface(Element*);
        virtual void AddSharing(Element*);
        virtual std::vector<Element*> GetNodes();
        virtual std::vector<Element*> GetInterfaces();
        virtual std::vector<Element*> GetSharing();
        virtual ~Quad() = default;

    private:
        std::vector<Element*> s_nodes;
        std::vector<Element*> s_interfaces;
        std::vector<Element*> s_sharingElements;
};

class Tri : public Element {
    public:
        Tri(const ElementInfo&);
        virtual void AddNode(Element*);
        virtual void AddInterface(Element*);
        virtual void RemoveNode(Element*);
        virtual void RemoveInterface(Element*);
        virtual void AddSharing(Element*);
        virtual std::vector<Element*> GetNodes();
        virtual std::vector<Element*> GetInterfaces();
        virtual std::vector<Element*> GetSharing();
        virtual ~Tri() = default;

    private:
        std::vector<Element*> s_nodes;
        std::vector<Element*> s_interfaces;
        std::vector<Element*> s_sharingElements;
};

class Hexa : public Element {
    public:
        Hexa(const ElementInfo&);
        virtual void AddNode(Element*);
        virtual void AddInterface(Element*);
        virtual void RemoveNode(Element*);
        virtual void RemoveInterface(Element*);
        virtual std::vector<Element*> GetNodes();
        virtual std::vector<Element*> GetInterfaces();
        virtual ~Hexa() = default;

    private:
        std::vector<Element*> s_nodes;
        std::vector<Element*> s_interfaces;
};

class Tetra : public Element {
    public:
        Tetra(const ElementInfo&);
        virtual void AddNode(Element*);
        virtual void AddInterface(Element*);
        virtual void RemoveNode(Element*);
        virtual void RemoveInterface(Element*);
        virtual std::vector<Element*> GetNodes();
        virtual std::vector<Element*> GetInterfaces();
        virtual ~Tetra() = default;

    private:
        std::vector<Element*> s_nodes;
        std::vector<Element*> s_interfaces;
};

class Prism : public Element {
    public:
        Prism(const ElementInfo&);
        virtual void AddNode(Element*);
        virtual void AddInterface(Element*);
        virtual void RemoveNode(Element*);
        virtual void RemoveInterface(Element*);
        virtual std::vector<Element*> GetNodes();
        virtual std::vector<Element*> GetInterfaces();
        virtual ~Prism() = default;

    private:
        std::vector<Element*> s_nodes;
        std::vector<Element*> s_interfaces;
};

class Pyram : public Element {
    public:
        Pyram(const ElementInfo&);
        virtual void AddNode(Element*);
        virtual void AddInterface(Element*);
        virtual void RemoveNode(Element*);
        virtual void RemoveInterface(Element*);
        virtual std::vector<Element*> GetNodes();
        virtual std::vector<Element*> GetInterfaces();
        virtual ~Pyram() = default;

    private:
        std::vector<Element*> s_nodes;
        std::vector<Element*> s_interfaces;
};

} // end of CHONS namespace

#endif