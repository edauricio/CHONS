// Class for Element creation and retrieval.

#ifndef ELEMENT_FACTORY_H
#define ELEMENT_FACTORY_H

#include "ElementComposite.h"
#include "ElementEnumInfo.h"
#include <cstdlib>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace CHONS {

class MeshReader;

// Each element is uniquely identified by its type and tag.
class ElementFactory {
    friend MeshReader; // Only MeshReader objects are able to place orders

    public:

        // Member function for adding/retrieving primitive vertex element
        Element* GetElement(const ElementInfo&);
        bool OrderElement(const ElementInfo&);
        void PlaceOrder();
        bool Created() { return s_created; };
        static ElementFactory* GetInstance();
        // DELETE THIS -- FOR DEBUGGING PURPOSES ONLY
        size_t HowMany(const ElementType& et) { 
            return (!(s_elements[et].empty())) ? s_elements[et].size() : 0; }
        // DELETE THIS -- FOR DEBUGGING PURPOSES ONLY
        void RunThrough();
        
    protected:
        ElementFactory() : s_elements(MAX_NUMBER_OF_TYPES),
                           s_orderedElements(MAX_NUMBER_OF_TYPES) {};
        
    private:
        // bool OrderElement(const ElementInfo&);
        // void PlaceOrder();


        static ElementFactory* s_singleInstance;
        Element* create(const ElementInfo&);
        bool s_created = false;
        std::vector<std::unordered_map<size_t, Element*> > s_elements;
        std::vector<std::unordered_map<size_t, ElementInfo>> s_orderedElements;

};

} // end of CHONS namespace

#endif