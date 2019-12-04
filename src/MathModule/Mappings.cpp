#include "MathModule/Mappings.h"
#include "MeshModule/ElementComposite.h"
#include "PolyModule/Polynomials.h"
#include "boost/assert.hpp"
#include <vector>

namespace CHONS {

using namespace Math;

// ---------- StdToPhysMapping Member Function Definitions --------- //

StdToPhysMapping::StdToPhysMapping(const unsigned& ord) : s_order(ord) {

}

// ---------- End of StdToPhysMapping Member Function Definitions --------- //



// ---------- LagrangeElementMapping Member Function Definitions --------- //

LagrangeElementMapping::LagrangeElementMapping(Element* ele)
                                    : StdToPhysMapping(ele->GetElementOrder()) {

}


// ------- End of LagrangeElementMapping Member Function Definitions ------- //



// ---------- TensorProductLGMapping Member Function Definitions --------- //

TensorProductLGMapping::TensorProductLGMapping(Element* ele)
                                    : LagrangeElementMapping(ele) {

}

// ------- End of TensorProductLGMapping Member Function Definitions ------- //



// ---------- LineLGMapping Member Function Definitions --------- //

LineLGMapping::LineLGMapping(Element* ele) : TensorProductLGMapping(ele) {
    BOOST_ASSERT_MSG(ele->GetType() == eLine, "Invalid extrinsic state passed"
            " to Line mapping; an element other than a Line has been passed");

    // Populate vector of evenly-spaced nodes in the standard region, so we
    // can create the mapping basis
    std::vector<double> std_pts{-1.0};
    double dx = 2.0 / s_order;
    for (int i = 1; i != s_order+1; ++i)
        std_pts.push_back(std_pts[i-1] + dx);

    s_mappingBasis = s_basisFac.GetBasis(ele, {std_pts});

}

Math::Vector LineLGMapping::EvaluateFor(Element* ele, const double& r,
                                        const double& s,
                                        const double& t) {
    BOOST_ASSERT_MSG(ele->GetType() == eLine, "Invalid extrinsic state passed"
            " to Line mapping; an element other than a Line has been passed");

    // The return is actually a single value, so a vector of 1 element will do
    // the job
    Math::Vector mapped_x(1);

    // Vector with nodes coordinates, arranged in the appropriate order
    Math::Vector nodes(s_order+1);
    // First node
    nodes[0] = ele->GetPrimitives()[0]->GetCoords()[0];
    // Loop for higher order nodes
    for (int i = 1; i != s_order; ++i) {
        nodes[i] = ele->GetPrimitives()[i+1]->GetCoords()[0];
    }
    // Last node (enumerated second in Lagrangian element, but last in our
    // basis multiplication)
    nodes[s_order] = ele->GetPrimitives()[1]->GetCoords()[0];

    // Multiply to get the mapping
    mapped_x[0] = Math::dot_product(nodes, s_mappingBasis->EvaluateAt(r));

    return mapped_x;
}

double LineLGMapping::Jacobian(Element* ele) {
    BOOST_ASSERT_MSG(ele->GetType() == eLine, "Invalid extrinsic state passed"
            " to Line mapping; an element other than a Line has been passed");
    
    // Calculate the element (constant) Jacobian
    // Jac = (x_r - x_l) / 2;
    std::vector<Element*> nodes = ele->GetPrimitives();
    return (nodes[1]->GetCoords()[0] - nodes[0]->GetCoords()[0]); 

}

// ---------- End of LineLGMapping Member Function Definitions --------- //

// ---------- QuadLGMapping Member Function Definitions --------- //

QuadLGMapping::QuadLGMapping(Element* ele) : TensorProductLGMapping(ele) {
    BOOST_ASSERT_MSG(ele->GetType() == eQuad, "Invalid extrinsic state passed"
            " to Quad mapping; an element other than a Quad has been passed");

    // Populate vector of evenly-spaced points for lagrange basis functions 
    // creation
    std::vector<double> pts{-1.0};
    double dx = 2.0 / s_order;
    for (int i = 1; i != s_order+1; ++i)
        pts.push_back(pts[i-1] + dx);

    s_mappingBasis = s_basisFac.GetBasis(ele, {pts, pts});

}

Math::Vector QuadLGMapping::EvaluateFor(Element* ele, const double& r,
                                                    const double& s,
                                                    const double& t) {
    // Instantiate returning vector which will contain both x and y coordinates
    Math::Vector result(2);

    // Get vector containing the node elements
    std::vector<Element*> nodes;
    // Get all edges and interior nodes first
    std::vector<Element*> allprim = ele->GetPrimitives();
    // Loop through edge primitives of Quad
    for (int i = 0; i != 4; ++i) { 
        // loop through each node of each edge and add it to nodes vector
        for (int j = 0; j != s_order+1; ++j)
            nodes.push_back(allprim[i]->GetPrimitives()[j]);
    }
    // Now loop through the remaining primitives, which are interior nodes,
    // and add them to the nodes vector
    for (int i = 4; i != allprim.size(); ++i)
        nodes.push_back(allprim[i]);

    // Populate the vectors containing the x- and y-nodes coordinates
    // TODO: Challenge now is to populate these coordinate vectors into
    // the corresponding basis order, i.e. a "tensor product" order, not a
    // 'lagrangian element ordering'
    Math::Vector pts_x(nodes.size()), pts_y(nodes.size());





    return result;
}

// ---------- End of QuadLGMapping Member Function Definitions --------- //


} // end of CHONS namespace