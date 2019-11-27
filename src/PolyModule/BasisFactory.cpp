#include "PolyModule/BasisFactory.h"
#include "MeshModule/ElementComposite.h"

namespace CHONS {

Basis* BasisFactory::GetBasis(Element* ele) {
    switch (ele->GetType()) {
        case eLine:
            return new TensorProductBasis(ele->GetType(), ele->GetPolyOrder());
    }
}

} // end of CHONS namespace