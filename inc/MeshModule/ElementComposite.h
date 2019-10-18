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
        virtual void AddPrimitive(Element*);
        virtual void RemovePrimitive(Element*);
        virtual void AddSharing(Element*);
        virtual std::map<ElementType, 
                    std::list<Element*> > GetPrimitives();
        virtual std::list<Element*> GetSharing();
        virtual Point* GetInteriorPoints();
        virtual Point* GetInterfacePoints();
        virtual Point* GetPoints();
        const ElementType& GetType() { return s_type; };
        const size_t& GetTag() { return s_tag; };
        const size_t& GetElementOrder() { return s_eleOrder; };
        const int& GetPolyOrder() { return s_polyOrder; };
        void SetPolyOrder(const int& o) { s_polyOrder = o; };

    protected:
        ElementType s_type;
        size_t s_tag;
        size_t s_eleOrder;
        int s_polyOrder;
        std::vector<Point*> s_interiorPoints;

};

class Node : public Element {
    public:
        Node(const ElementInfo&);
        virtual void AddSharing(Element*);
        virtual std::list<Element*> GetSharing();
        virtual ~Node() = default;

    private:
        std::vector<double> s_coords;
        std::list<Element*> s_sharingElements;

};

class Line : public Element {
    public:
        Line(const ElementInfo&);
        virtual void AddPrimitive(Element*);
        virtual void RemovePrimitive(Element*);
        virtual void AddSharing(Element*);
        virtual std::map<ElementType, 
                    std::list<Element*> > GetPrimitives();
        virtual std::list<Element*> GetSharing();
        virtual ~Line() = default;

    private:
        std::map<ElementType, std::list<Element*> > s_primitives;
        std::list<Element*> s_sharingElements;
};

class Quad : public Element {
    public:
        Quad(const ElementInfo&);
        virtual void AddPrimitive(Element*);
        virtual void RemovePrimitive(Element*);
        virtual void AddSharing(Element*);
        virtual std::map<ElementType, 
                    std::list<Element*> > GetPrimitives();
        virtual std::list<Element*> GetSharing();
        virtual ~Quad() = default;

    private:
        std::map<ElementType, std::list<Element*> > s_primitives;
        std::list<Element*> s_sharingElements;
};

class Tri : public Element {
    public:
        Tri(const ElementInfo&);
        virtual void AddPrimitive(Element*);
        virtual void RemovePrimitive(Element*);
        virtual void AddSharing(Element*);
        virtual std::map<ElementType, 
                    std::list<Element*> > GetPrimitives();
        virtual std::list<Element*> GetSharing();
        virtual ~Tri() = default;

    private:
        std::map<ElementType, std::list<Element*> > s_primitives;
        std::list<Element*> s_sharingElements;
};

class Hexa : public Element {
    public:
        Hexa(const ElementInfo&);
        virtual void AddPrimitive(Element*);
        virtual void RemovePrimitive(Element*);
        virtual std::map<ElementType, 
                    std::list<Element*> > GetPrimitives();
        virtual ~Hexa() = default;

    private:
        std::map<ElementType, std::list<Element*> > s_primitives;
};

class Tetra : public Element {
    public:
        Tetra(const ElementInfo&);
        virtual void AddPrimitive(Element*);
        virtual void RemovePrimitive(Element*);
        virtual std::map<ElementType, 
                    std::list<Element*> > GetPrimitives();
        virtual ~Tetra() = default;

    private:
        std::map<ElementType, std::list<Element*> > s_primitives;
};

class Prism : public Element {
    public:
        Prism(const ElementInfo&);
        virtual void AddPrimitive(Element*);
        virtual void RemovePrimitive(Element*);
        virtual std::map<ElementType, 
                    std::list<Element*> > GetPrimitives();
        virtual ~Prism() = default;

    private:
        std::map<ElementType, std::list<Element*> > s_primitives;
};

class Pyram : public Element {
    public:
        Pyram(const ElementInfo&);
        virtual void AddPrimitive(Element*);
        virtual void RemovePrimitive(Element*);
        virtual std::map<ElementType, 
                    std::list<Element*> > GetPrimitives();
        virtual ~Pyram() = default;

    private:
        std::map<ElementType, std::list<Element*> > s_primitives;
};

} // end of CHONS namespace

#endif