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

    for (auto& set_pt : pts) {
        s_polys.push_back(new Lagrange(set_pt));
    }

}

InterpolationTPBasis::~InterpolationTPBasis() {
    for (auto i = 0; i != s_polys.size(); ++i)
        delete s_polys[i];
}

Vector InterpolationTPBasis::EvaluateAt(const double& r, const double& s,
                                        const double& t) {
    switch(s_dim) {
        case 1:
            BOOST_ASSERT_MSG((s == -2) && (t == -2), 
                "number of points for 1D basis evaluation exceeded");
            BOOST_ASSERT_MSG((r >= -1) && (r <= 1), "point for basis evaluation"
                            " must be within the invertal [-1,1]");
            break;

        case 2:
            BOOST_ASSERT_MSG(t == -2, 
                "number of points for 2D basis evaluation exceeded");
            BOOST_ASSERT_MSG((r >= -1) && (r <= 1) && 
                            (s >= -1) && (s <= 1), "point for basis evaluation"
                            " must be within the invertal [-1,1]");
            break;

        case 3:
            BOOST_ASSERT_MSG((r >= -1) && (r <= 1) && 
                            (s >= -1) && (s <= 1) &&
                            (t >= -1) && (t <= 1), 
            "point for basis evaluation must be within the invertal [-1,1]");
            break;

        default:
            BOOST_ASSERT_MSG(false, "Dimension of basis is higher than 3?");
    }
    
    

    // Find the size of the basis vector (i.e., number of total points 
    // considering all dimensions)
    int vecSize = 1;
    for (auto& poly : s_polys)
        vecSize *= (poly->GetOrder()+1);

    // Mount the basis vector to be returned
    Vector basis(vecSize);

    // For each 1D polynomial basis, evaluate it for the corresponding "i" 
    // index (i.e. li(rj)), and multiply it with the corresponding basis in the 
    // right place in the basis vector (i.e. 'i' running faster than 'j' that 
    // runs faster than 'k')

    // To accomplish this task, a integer division is used (i/div), where 'div'
    // is an auxiliary variable; this integer division determines the "stepsize"
    // for each interpolant index in accordance with the basis global points.
    
    // for (int k = 0; k != s_polys.size(); ++k) {
    //     int div = (k == 0) ? 1 : div*s_polys[k-1]->GetOrder()+1;
    //     for (int i = 0; i != basis.size(); ++i)
    //         basis[i] *= dynamic_cast<Interpolant*>(s_polys[k])->EvaluateAt(
    //                             stdpts[k], (i/div)%(s_polys[k]->GetOrder()+1));
    // }

    // Even though the above algorithm is nice and clean, it is, unfortunately,
    // O(n^3) on the number of total points. Hence, a switch statement is more
    // appropriated in terms of performance

    switch(s_polys.size()) {
        case 1:
            for(int i = 0; i != basis.size(); ++i)
                basis[i] = dynamic_cast<Interpolant*>(s_polys[0])->EvaluateAt(
                            r, i);
            break;

        case 2:
            for (int j = 0; j != s_polys[1]->GetOrder()+1; ++j)
                for (int i = 0; i != s_polys[0]->GetOrder()+1; ++i)
                    basis[j*(s_polys[0]->GetOrder()+1) + i] =
                        dynamic_cast<Interpolant*>(s_polys[0])->EvaluateAt(r, i)
                    *   dynamic_cast<Interpolant*>(s_polys[1])->EvaluateAt(s, j);
            break;

        case 3:
            for (int k = 0; k != s_polys[2]->GetOrder()+1; ++k)
                for (int j = 0; j != s_polys[1]->GetOrder()+1; ++j)
                    for (int i = 0; i != s_polys[0]->GetOrder()+1; ++i)
                        basis[(k
                            * (s_polys[0]->GetOrder()+1)
                            * (s_polys[1]->GetOrder()+1) )
                            + j*(s_polys[0]->GetOrder()+1)
                            + i] =
                        dynamic_cast<Interpolant*>(s_polys[0])->EvaluateAt(r, i)
                    *   dynamic_cast<Interpolant*>(s_polys[1])->EvaluateAt(s, j)
                    *   dynamic_cast<Interpolant*>(s_polys[2])->EvaluateAt(t, k);
            break;

        default:
            BOOST_ASSERT_MSG(false, "Tensor product basis must have dimension"
                        " between 1 and 3");
            break;
    }

    return basis;
}

// ---------- End of InterpolationTPBasis Member Function Definitions --------- //

} // end of CHONS namespace