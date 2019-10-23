// Class for Element creation and retrieval.

#ifndef ELEMENT_FACTORY_H
#define ELEMENT_FACTORY_H

#include "ElementComposite.h"
#include "ElementEnumInfo.h"
#include <cstdlib>
#include <unordered_map>

namespace CHONS {

// Each element is uniquely identified by its type and tag.
class ElementFactory {
    public:

        // Member function for adding/retrieving primitive vertex element
        Element* GetElement(const ElementInfo&);
        bool Created() { return s_created; };
        static ElementFactory* GetInstance();

    protected:
        ElementFactory() = default;
        
    private:
      static ElementFactory* s_singleInstance;
        Element* create(const ElementInfo&);
        bool s_created = false;
        std::map<ElementType, 
                std::unordered_map<size_t, Element*> > s_elements;
};

} // end of CHONS namespace

#endif