// Class to instantiate and keep a registry of live basis so it can
// appropriately bookeep the sharing of them across mesh objects

#ifndef BASIS_FACTORY_H
#define BASIS_FACTORY_H

#include <unordered_map>
#include <initializer_list>
#include <vector>
#include "Basis.h"
#include "MeshModule/ElementEnumInfo.h"

namespace CHONS {

class Element;

// TODO? Create hierarchy of this factory depending on the method
// e.g. Interp. (FR/Nodal DG) or Expansion (Modal DG)
// Is it necessary? This info should be available for the one creating
// the basis..... hence it can be passed on instantiation ("globally")
class BasisFactory {
    public:
        BasisFactory() = delete;
        virtual Basis* GetBasis(Element*);
        virtual Basis* GetBasis(Element*, 
                    const std::initializer_list<std::vector<double>>&);


    private:
        // New variable needed: not only ElementType is important, but
        // the order of the polynomial basis (in each dir.), since we would
        // like to support different orders throughout the mesh in a later
        // iteration

        std::unordered_map<ElementType, Basis*> s_basisMap;
};

class InterpolationBasisFactory : public BasisFactory {
    public:
        InterpolationBasisFactory() = default;
        virtual Basis* GetBasis(Element*, 
                    const std::initializer_list<std::vector<double>>&) override;

    private:

};

class ExpansionBasisFactory : public BasisFactory {
    public:
        ExpansionBasisFactory() = default;
        virtual Basis* GetBasis(Element*) override;

    private:
};

} // end of CHONS namespace

#endif