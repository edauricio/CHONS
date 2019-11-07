// Class to hold mesh connectivity and boundary information
// so that the solver/driver/whatever can promptly request this info
// and get an answer ASAP

#ifndef MESH_INFO_HOLDER_H
#define MESH_INFO_HOLDER_H

#include "ElementEnumInfo.h"
#include "ElementFactory.h" // To be removed after iteration 2
#include <unordered_map>
#include <iterator>

namespace CHONS {

class Element;
class GraphInfoProcessor;

class MeshInfoHolder {
    friend GraphInfoProcessor;
    class ElementIterator;

    public:
        using element_iterator = ElementIterator;


        MeshInfoHolder() : s_factory(ElementFactory::GetInstance()) {}
        static MeshInfoHolder* GetInstance();
        const std::array<Element*, 2>& GetSharingElements(const size_t&);
        element_iterator ElementBegin(const short&);
        element_iterator ElementEnd(const short&);

        

    private:

        // TODO: This class should be used only for the interface elements, that
        // are mapped to a 2-element array. For boundary and interior elements,
        // we should just return the unordered_map iterators begin and end (of
        // the corresponding bucket, of course)
        
        // Helper class to iterate over the interface elements
        class ElementIterator : public std::iterator<std::forward_iterator_tag, 
                                                        Element> {
            public:
                ElementIterator(Element*, const short&);
                ElementIterator(const ElementIterator&&);
                ElementIterator& operator=(const ElementIterator&&);
                ElementIterator(const ElementIterator&) = delete;
                ElementIterator& operator=(const ElementIterator&) = delete;

                ElementIterator& operator++();
                ElementIterator operator++(int);
                ElementIterator& operator--();
                ElementIterator operator--(int);
                bool operator==(ElementIterator);
                bool operator!=(ElementIterator);
                Element& operator*() { return *elem; }

            private:
                Element* elem;
                short int region;
                std::unordered_map<short int, Element*>::iterator bkt_beg;
                std::unordered_map<short int, Element*>::iterator bkt_end;
        };

        // MARKED TO BE REFACTORED IN ITERATION 2:
        // Since in this iteration (2) a native mesh file will be created instead
        // of reading directly from gmsh/cgns, we will know in advance how many elements
        // (nodes, lines, 2D/3D) will be created, so we can call for memory allocation
        // in the beginning of the section reading. Hence, no more "OrderElement" thing
        // will happen, and elements will be constructed in the previsouly allocated
        // memory right after reading it. Hence, the first parameter here could be
        // the Element* itself, passed by requesting it with ElementFactory::GetElement()
        // void AddInterfaceElement(const ElementInfo&, const int&);
        // void AddInteriorElement(const ElementInfo&, const int&);
        void AddBoundaryElement(const ElementInfo&, const int&);
        void AddInterfaceElement(const size_t&, 
                                const std::pair<ElementType, size_t>&, 
                                const std::pair<ElementType, size_t>&);
        void AddInteriorElement(const ElementInfo&, const int&);
        void Consolidate();


        static MeshInfoHolder* s_soleInstance;
        // TODO:
        // On writing our native mesh file, reorder the physical regions so that
        // they range from 1-numOfRegions; in this way, we can use a vector instead
        // of a map to retrive contained elements and thus accelerate the answer

        // type(region / tag)
        std::array<std::unordered_multimap<short int, size_t>,
                    MAX_NUMBER_OF_TYPES> s_tmpBndElements;
        std::array<std::unordered_multimap<short int, size_t>,
                    MAX_NUMBER_OF_TYPES> s_tmpIntElements;

        // same as above, but the definitive structure
        // (after iteration 2 only this structure will be needed)
        std::unordered_multimap<short int, Element*> s_boundaryElements;
        std::unordered_multimap<short int, Element*> s_interiorElements;

        // interface element to sharing elements
        // interface_tag, (element1_tag, element2_tag)
        std::unordered_map<size_t, 
                            std::vector<
                            std::pair<ElementType, size_t>
                            > > s_tmpBndToSharings;

        // same as above, but the definitive structure
        // (after iteration 2 only this structure will be needed)
        std::unordered_map<size_t, std::array<Element*,2>> s_interfaceElements;

        ElementFactory* s_factory;
};

} // end of namespace CHONS

#endif