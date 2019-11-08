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
        using iterator = ElementIterator;


        MeshInfoHolder() : s_factory(ElementFactory::GetInstance()) {}
        static MeshInfoHolder* GetInstance();
        iterator ElementsBegin(const short& reg = -1);
        iterator ElementsEnd(const short& reg = -1);
        iterator BoundaryBegin(const short&);
        iterator BoundaryEnd(const short&);
        iterator InterfaceBegin();
        iterator InterfaceEnd();

        

    private:

        // Helper class to iterate through elements
        class ElementIterator : public std::iterator<std::forward_iterator_tag,
                                                                Element*> {
            using bucket_iterator = 
                    std::unordered_multimap<short, Element*>::local_iterator;
            public:
                // Constructors
                ElementIterator(bucket_iterator, bucket_iterator);
                ElementIterator(const ElementIterator&);

                ElementIterator& operator=(const ElementIterator&);

                Element& operator*() { return *elem; }
                Element* operator->() { return &**this; }
                ElementIterator& operator++();
                ElementIterator operator++(int);
                bool operator==(const ElementIterator&);
                bool operator!=(const ElementIterator&);


            private:
                bucket_iterator it;
                bucket_iterator end;
                Element* elem;

        };

        // MARKED TO BE REFACTORED IN ITERATION 2:
        // Since in this iteration (2) a native mesh file will be created instead
        // of reading directly from gmsh/cgns, we will know in advance how many elements
        // (nodes, lines, 2D/3D) will be created, so we can call for memory allocation
        // in the beginning of the section reading. Hence, no more "OrderElement" thing
        // will happen, and elements will be constructed in the previsouly allocated
        // memory right after reading it. Thus, the first parameter here could be
        // the Element* itself, passed by requesting it with ElementFactory::GetElement()
        // void AddInterfaceElement(const ElementInfo&, const int&);
        // void AddInteriorElement(const ElementInfo&, const int&);
        void AddInterfaceElement(const ElementInfo&, const int&);
        void AddInteriorElement(const ElementInfo&, const int&);
        void Consolidate();


        static MeshInfoHolder* s_soleInstance;
        // TODO:
        // On writing our native mesh file, reorder the physical regions so that
        // they range from 1-numOfRegions; in this way, we can use a vector instead
        // of a map to retrive contained elements and thus accelerate the answer

        // type(region / tag)
        std::array<std::unordered_multimap<short int, size_t>,
                    MAX_NUMBER_OF_TYPES> s_tmpInterElements;
        std::array<std::unordered_multimap<short int, size_t>,
                    MAX_NUMBER_OF_TYPES> s_tmpIntElements;

        // same as above, but the definitive structure
        // (after iteration 2 only this structure will be needed)
        std::unordered_multimap<short int, Element*> s_interfaceElements;
        std::unordered_multimap<short int, Element*> s_interiorElements;

        ElementFactory* s_factory;
};

} // end of namespace CHONS

#endif