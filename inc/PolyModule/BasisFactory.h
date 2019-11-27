// Class to instantiate and keep a registry of live basis so it can
// appropriately bookeep the sharing of them across mesh objects

#ifndef BASIS_FACTORY_H
#define BASIS_FACTORY_H

#include <unordered_map>
#include "Basis.h"
#include "MeshModule/ElementEnumInfo.h"

namespace CHONS {

class Element;

// TODO? Create hierarchy of this factory depending on the method
// e.g. Interp. (FR/Nodal DG) or Expansion (Modal DG)
// Is it necessary? This info should be available for the one creating
// the basis..... hence it can be passed on instantiation
class BasisFactory {
    public:
        BasisFactory() = delete;
        static Basis* GetBasis(Element*);

    private:
        std::unordered_map<ElementType, Basis*> s_basisMap;

};

} // end of CHONS namespace

#endif