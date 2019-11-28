#include "MathModule/Mappings.h"
#include "MeshModule/ElementComposite.h"
#include "PolyModule/Polynomials.h"
#include "boost/assert.hpp"
#include <iostream> // remove

namespace CHONS {

using namespace Math;

// ---------- StdToPhysMapping Member Function Definitions --------- //

StdToPhysMapping::StdToPhysMapping(const unsigned& ord) : s_elementOrder(ord) {

}

// ---------- End of StdToPhysMapping Member Function Definitions --------- //


// ---------- LineMapping Member Function Definitions --------- //

LineMapping::LineMapping(Element *ele) : 
                                    StdToPhysMapping(ele->GetElementOrder()),
                                    s_basis(ele->GetElementOrder()+1),
                                    s_edgePoints(ele->GetElementOrder()+1) {
    std::vector<double> points{-1};
    points.push_back(1);
    double d = 2.0 / s_elementOrder;
    for (int i = 1; i != s_elementOrder; ++i)
        points.push_back(-1.0 + i*d);

    s_poly = new Lagrange{points};

    // If a straight line (well, it will be since problem is 1D in this case),
    // calculate Jacobian, which is constant and set the flag
}

Vector LineMapping::EvaluateFor(Element *ele, const double& r, const double& s, 
                                                            const double& t) {
    BOOST_ASSERT_MSG((s < -1) && (t < -1), "1D element only have one mapping"
                                " parameter");
    BOOST_ASSERT_MSG((r < 1) || (r > -1), "Standard line interval is [-1,1];"
                        " can't evaluate mapping beyond standard interval");


    std::vector<Element*> prims(ele->GetPrimitives());
    for (int i = 0; i != s_basis.size(); ++i) {
        s_basis[i] = dynamic_cast<Interpolant*>(s_poly)->EvaluateAt(r, i);
        s_edgePoints[i] = prims[i]->GetCoords()[0];
    }

    return Vector{dot_product(s_edgePoints, s_basis), 0, 0};
}

double LineMapping::Jacobian(Element* ele) {
    return 1.0;
}

// ---------- End of LineMapping Member Function Definitions --------- //


// ---------- End of QuadMapping Member Function Definitions --------- //

QuadMapping::QuadMapping(Element* ele) : 

// ---------- QuadMapping Member Function Definitions --------- //



} // end of CHONS namespace