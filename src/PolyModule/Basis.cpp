#include "PolyModule/Basis.h"
#include "boost/assert.hpp"
#include <memory>
#include <algorithm>

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

    for (auto& set_pt : pts)
        s_polys.push_back(new Lagrange(set_pt));

}

InterpolationTPBasis::~InterpolationTPBasis() {
    for (auto i = 0; i != s_polys.size(); ++i)
        delete s_polys[i];
}

Vector InterpolationTPBasis::EvaluateAt(const double& r, const double& s,
                                        const double& t) {
    BOOST_ASSERT_MSG((s_dim == 1) && (s == -2) && (t == -2), 
            "number of points for 1D basis evaluation exceeded");
    BOOST_ASSERT_MSG((s_dim == 2) && (t == -2), 
            "number of points for 2D basis evaluation exceeded");

    BOOST_ASSERT_MSG((r >= -1) && (r <= 1) && 
                     (s <= -1) && (s >= 1) &&
                     (t >= -1) && (t <= 1), 
            "points for basis evaluation can't exceed the interval [-1,1]");

    // Find the size of the basis vector (i.e., number of total points 
    // considering all dimensions)
    int vecSize = 1;
    for (auto& poly : s_polys)
        vecSize *= (poly->GetOrder()+1);

    // Mount the basis vector to be returned
    Vector basis(vecSize);
    std::vector<double> stdpts{r, s, t};
    std::fill(basis.begin(), basis.end(), 1.0);

    // For each 1D polynomial basis, evaluate it for the corresponding "i" 
    // index (i.e. li(rj)), and multiply it with the corresponding basis in the 
    // right place in the basis vector (i.e. 'i' running faster than 'j' that 
    // runs faster than 'k')
    for (int k = 0; k != s_polys.size(); ++k) {
        for (int i = 0; i != basis.size(); ++i)
            basis[i] *= dynamic_cast<Interpolant*>(s_polys[k])->EvaluateAt(
                                stdpts[k], i%(s_polys[k]->GetOrder()+1));
        }

    return basis;
}

// ---------- End of InterpolationTPBasis Member Function Definitions --------- //

} // end of CHONS namespace