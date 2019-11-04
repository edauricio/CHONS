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
        // DELETE THIS -- FOR DEBUGGING PURPOSES ONLY
        size_t HowMany(const ElementType& et) { 
            return ((s_elements.find(et) != s_elements.end()) ? 
              s_elements.find(et)->second.size()
            : 0); }
        // DELETE THIS -- FOR DEBUGGING PURPOSES ONLY
        void RunThrough();
        
    protected:
        ElementFactory() = default;
        
    private:
      static ElementFactory* s_singleInstance;
        Element* create(const ElementInfo&);
        bool s_created = false;
        std::unordered_map<ElementType, 
                std::unordered_map<size_t, Element*> > s_elements;
};

} // end of CHONS namespace

#endif