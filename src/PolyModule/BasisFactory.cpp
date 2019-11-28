#include "PolyModule/BasisFactory.h"
#include "MeshModule/ElementComposite.h"
#include <vector>
#include <initializer_list>

namespace CHONS {

Basis* BasisFactory::GetBasis(Element* ele) {
    // The decision on either Interpolation or Expansion basis will be decided
    // by ResourceDirector based on the chosen method (i.e. Expasion for Modal
    // DG and Interpolation for FR/Nodal DG)
    // For now, though, considerate only Interpolation basis
    switch (ele->GetType()) {
        case eLine:
            std::vector<std::vector<double>> pts{
                                            std::vector<double>{-1., 0., 0.}};
            pts.push_back({1., 0., 0.});
            double d = 2.0 / ele->GetElementOrder();
            for (int i = 1; i != ele->GetElementOrder(); ++i)
                pts.push_back({-1 + i*d, 0., 0.});
            
            return new InterpolationTPBasis(ele->GetType(), 
                                ele->);
    }
}

} // end of CHONS namespace