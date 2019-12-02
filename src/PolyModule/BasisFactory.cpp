#include "PolyModule/BasisFactory.h"
#include "MeshModule/ElementComposite.h"
#include "boost/assert.hpp"
#include <vector>
#include <initializer_list>

namespace CHONS {

Basis* BasisFactory::GetBasis(Element* ele) {
    BOOST_ASSERT_MSG(false, "An initializer list of points is needed for"
                            " requesting an interpolation basis");
}
Basis* BasisFactory::GetBasis(Element* ele, 
                const std::initializer_list<std::vector<double>>& pts) {
    BOOST_ASSERT_MSG(false, "No points are needed for requesting an"
                            " expansion basis");
}

Basis* InterpolationBasisFactory::GetBasis(Element* ele, 
            const std::initializer_list<std::vector<double>>& pts) {
    Basis* retbasis;

    switch (ele->GetType()) {
        case eLine:
        case eQuad:
        case eHexa:
            // Tensor Product basis
            retbasis = new InterpolationTPBasis(ele->GetType(), pts);
            break;

        case eTri:
        case eTetra:
        case ePyram:
        case ePrism:
            // Simplex basis
            // retbasis = new InterpolationSimplexBasis(...);
            break;

        default:
        break;
    }

    return retbasis;
}

} // end of CHONS namespace