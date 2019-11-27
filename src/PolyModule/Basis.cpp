#include "PolyModule/Basis.h"
#include "boost/assert.hpp"
#include <memory>

namespace CHONS {

// ---------- Basis Member Function Definitions --------- //


// ---------- End of Basis Member Function Definitions --------- //


// ---------- TensorProductBasis Member Function Definitions --------- //

TensorProductBasis::TensorProductBasis(ElementType etype) : 
                                        s_dim(static_cast<unsigned>(etype)) {
    s_dim = (s_dim > 2) ? ((s_dim > 3) ? 3 : 2) : s_dim;
}

// ---------- End of TensorProductBasis Member Function Definitions --------- //



// ---------- InterpolationTPBasis Member Function Definitions --------- //

InterpolationTPBasis::InterpolationTPBasis(ElementType etype, 
                    const std::initializer_list<std::vector<double>> &pts) :
                    TensorProductBasis(etype) {

    BOOST_ASSERT_MSG(s_dim == pts.size(), "Invalid number of points coordinates"
                            " for Basis creation");

    // for (auto it = pts.begin(); ++it != pts.end(); )
    //     BOOST_ASSERT_MSG(it->size() == (it-1)->size(), "invalid points argument"
    //                 " for interpolation tensor basis construction");

    for (auto& set_pt : pts)
        s_polys.push_back(new Lagrange(set_pt));

}

InterpolationTPBasis::~InterpolationTPBasis() {
    for (auto i = 0; i != s_polys.size(); ++i)
        delete s_polys[i];
}

Vector InterpolationTPBasis::EvaluateAt(const double& x) {
    BOOST_ASSERT_MSG(s_dim == 1, "can't evaluate a multidimensional basis"
                        " on a single point coordinate");
    /*.......*/
}

// ---------- End of InterpolationTPBasis Member Function Definitions --------- //

} // end of CHONS namespace