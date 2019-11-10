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
    template <typename T>
    class ElementIterator;

    public:
        using region_iterator = ElementIterator<
                    std::unordered_multimap<short, Element*>::local_iterator
                                                >;
        using all_iterator = ElementIterator<
                    std::unordered_multimap<short, Element*>::iterator
                                                >;


        MeshInfoHolder() : s_factory(ElementFactory::GetInstance()) {}
        static MeshInfoHolder* GetInstance();
        all_iterator ElementsBegin();
        all_iterator ElementsEnd();
        region_iterator ElementsBegin(const short&);
        region_iterator ElementsEnd(const short&);
        region_iterator BoundaryBegin(const short&);
        region_iterator BoundaryEnd(const short&);
        region_iterator InterfaceBegin();
        region_iterator InterfaceEnd();
        

    private:

        // Helper class to iterate through elements
        template <typename T>
        class ElementIterator {
            using iterator = T;
            public:
                // Iterator trais (since std::iterator<> is deprecated)
                using value_type = Element;
                using pointer = Element*;
                using reference = Element&;
                using difference_type = ptrdiff_t;
                using iterator_category = std::forward_iterator_tag;

                // Constructors
                ElementIterator<T>(T, T);
                ElementIterator<T>(const ElementIterator<T>&);

                ElementIterator<T>& operator=(const ElementIterator<T>&);

                Element& operator*() { return *elem; }
                Element* operator->() { return &**this; }
                ElementIterator<T>& operator++();
                ElementIterator<T> operator++(int);
                bool operator==(const ElementIterator<T>&);
                bool operator!=(const ElementIterator<T>&);


            private:
                T it;
                T end;
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

// Definition of ElementIterator helper class to iterate through the elements
template <typename T>
MeshInfoHolder::ElementIterator<T>::ElementIterator(iterator me,
                                            iterator e) : it(me),
                                                end(e) {
    if (it != end)
        elem = it->second;
}

template <typename T>
MeshInfoHolder::ElementIterator<T>::ElementIterator(
                                                const ElementIterator<T>& eit)
                                            : it(eit.it), end(eit.end) {
    if (it != end)
        elem = it->second;
}

template <typename T>
MeshInfoHolder::ElementIterator<T>& MeshInfoHolder::ElementIterator<T>::operator=(
                                                    const ElementIterator& eit){
    it = eit.it;
    end = eit.end;
    if (it != end)
        elem = eit.elem;
    else
        elem = nullptr;
    return *this;
}

template <typename T>
MeshInfoHolder::ElementIterator<T>& MeshInfoHolder::ElementIterator<T>::operator++() {
    it++;
    if (it != end)
        elem = it->second;
    else
        elem = nullptr;
    return *this;
}

template <typename T>
MeshInfoHolder::ElementIterator<T> MeshInfoHolder::ElementIterator<T>::operator++(int) {
    iterator it_old = it++;
    if (it != end)
        elem = it->second;
    else
        elem = nullptr;
    return ElementIterator<T>(it_old, end);
}

template <typename T>
bool MeshInfoHolder::ElementIterator<T>::operator==(const ElementIterator<T> & rhs) {
    return (it == rhs.it);
}

template <typename T>
bool MeshInfoHolder::ElementIterator<T>::operator!=(const ElementIterator<T>& rhs) {
    return !(*this == rhs);
}

} // end of namespace CHONS

#endif