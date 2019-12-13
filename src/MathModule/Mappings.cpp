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
    nodes[0] = ele->GetNodes()[0]->GetCoords()[0];
    // Loop for higher order nodes
    for (int i = 1; i != s_order; ++i) {
        nodes[i] = ele->GetNodes()[i+1]->GetCoords()[0];
    }
    // Last node (enumerated second in Lagrangian element, but last in our
    // basis multiplication)
    nodes[s_order] = ele->GetNodes()[1]->GetCoords()[0];

    // Multiply to get the mapping
    mapped_x[0] = Math::dot_product(nodes, s_mappingBasis->EvaluateAt(r));

    return mapped_x;
}

double LineLGMapping::Jacobian(Element* ele, const double& r, 
                                            const double& s, 
                                            const double& t) {
    BOOST_ASSERT_MSG(ele->GetType() == eLine, "Invalid extrinsic state passed"
            " to Line mapping; an element other than a Line has been passed");
    
    // Calculate the element (constant) Jacobian
    // Jac = (x_r - x_l) / 2;
    std::vector<Element*> nodes = ele->GetNodes();
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

    // Allocate vector for the quad node elements
    std::vector<Element*> nodes( (s_order+1) * (s_order+1) );

    // Now organize Quad nodes into 'nodes' vector in a "tensor product" order
    // Rememeber that the Lagrangian element ordering is not the same as the
    // "natural tensor product" ordering that we conventioned in the code (in
    // Basis classes); thus this rearragement is needed
    
    // -=-=--=-=-=-=-=-=-   MAPPING ALGORITHM   -=-=-=-=-=-=-=-=-=-

    // The mapping algorithm below is based on the fact that for a Lagrangian 
    // high-order element, we simply have an "inception" of quads, one inside
    // another, such that we have multiple embedded quads; hence, we loop 
    // through all possible inner quads, according to element order. The array 
    // named ini_pos is the key auxiliary variable here to do such task.

    // cnt will be just a counter variable, incremented for every node we 
    // proccess; it will be used to subscript the Quad nodes vector, which means 
    // that we're just following Lagrangian node numbering and mapping the
    // "tensor product" ordering accordingly (and not the other way around)
    int cnt = 0; // reset counter

    // This vector is just a copy of Quad nodes vector used as the RHS of the
    // mapping below
    std::vector<Element*> nodes_aux = ele->GetNodes();

    // ini_pos is an array containing the initial position of the nodes to be
    // proccessed in every loop below, which account for bottom/top vertices of 
    // the quad, then bottom/right/top/left edges internal nodes.
    // Every outer loop iteration (k) is the processing of one quad; hence, 
    // after the loop is finished, we need to update ini_pos accordingly so that
    // we have the initial positions for the next inner quad; the "stride"
    // between nodes in this mapping is constant from an outer to an inner quad,
    // so we don't need a stride[] array for every vertex/edge processing and 
    // we can simply update the initial position for the nodes
    int ini_pos[] = {0, (ele->GetElementOrder()+1)*(ele->GetElementOrder()+1)-1, 
                    1, 2*(ele->GetElementOrder()+1)-1, 
                    (ele->GetElementOrder()+1)*(ele->GetElementOrder()+1)-1-1, 
                    ((ele->GetElementOrder()+1)*(ele->GetElementOrder()+1)-1) 
                        - (2*(ele->GetElementOrder()+1)-1)};

    // Every inner quad is a quad with an order reduced by two with respect to
    // its outer quad
    for (int k = ele->GetElementOrder(); k > 0; k -= 2) {

        // Now the "serendipity nodes" proccessing begins

        // Bottom vertices
        for (int i = 0; i != 2; ++i, cnt++)
            nodes[ini_pos[0] /* initial position */
                  + i*k /* stride */]  = nodes_aux[cnt];
        // Top vertices
        for (int i = 0; i != 2; ++i, cnt++)
            nodes[ini_pos[1] /* initial position */
                  - i*k /* stride */] = nodes_aux[cnt];

        // Bottom edge interior nodes
        for (int i = 0; i != k-1; ++i, ++cnt)
            nodes[ini_pos[2] /* initial position */
                  + i /* stride */] = nodes_aux[cnt];

        // Right edge interior nodes
        for (int i = 0; i != k-1; ++i, ++cnt)
            nodes[ini_pos[3] /* initial position */
                  + i*(ele->GetElementOrder()+1) /* stride */] = nodes_aux[cnt];

        // Top edge interior nodes
        for (int i = 0; i != k-1; ++i, ++cnt)
            nodes[ini_pos[4] /* initial position */
                  - i /* stride */] = nodes_aux[cnt];

        // Left edge interior nodes
        for (int i = 0; i != k-1; ++i, ++cnt)
            nodes[ini_pos[5] /* initial position */
                  - i*(ele->GetElementOrder()+1) /* stride */] = nodes_aux[cnt];

        // If the quad we're proccessing is of 2nd order, it'll have a single 
        // internal node instead of an internal quad, so let's account for it
        if (k == 2)
            nodes[ini_pos[5] + 1] = nodes_aux[cnt++];

        // Recalculate initial positions for the next, inner quad
        ini_pos[0] += (ele->GetElementOrder() + 1 + 1);
        ini_pos[1] -= (ele->GetElementOrder() + 1 + 1);
        ini_pos[2] = ini_pos[0] + 1;
        ini_pos[3] += ele->GetElementOrder();
        ini_pos[4] = ini_pos[1] - 1;
        ini_pos[5] -= ele->GetElementOrder();
    }

    // Populate the vectors containing the x- and y-nodes coordinates
    Math::Vector pts_x(nodes.size()), pts_y(nodes.size());
    for (int i = 0; i != nodes.size(); ++i) {
        pts_x[i] = nodes[i]->GetCoords()[0];
        pts_y[i] = nodes[i]->GetCoords()[1];
    }

    // Finally, calculate the mapping for each coordinate
    result[0] = dot_product(pts_x, s_mappingBasis->EvaluateAt(r, s));
    result[1] = dot_product(pts_y, s_mappingBasis->EvaluateAt(r, s));

    return result;
}

double QuadLGMapping::Jacobian(Element* ele, const double& r, 
                                            const double& s, 
                                            const double& t) {
    // TODO
    // Implement jacobian for Quad Lagrangian Mapping
    return 0.0;
}

// ---------- End of QuadLGMapping Member Function Definitions --------- //



// ---------- HexaLGMapping Member Function Definitions --------- //

HexaLGMapping::HexaLGMapping(Element* ele) : TensorProductLGMapping(ele) {
    BOOST_ASSERT_MSG(ele->GetType() == eHexa, "Invalid extrinsic state passed"
            " to Hexa mapping; an element other than a Hexa has been passed");

    // Populate vector of evenly-spaced points for lagrange basis functions 
    // creation
    std::vector<double> pts{-1.0};
    double dx = 2.0 / s_order;
    for (int i = 1; i != s_order+1; ++i)
        pts.push_back(pts[i-1] + dx);

    s_mappingBasis = s_basisFac.GetBasis(ele, {pts, pts, pts});
}

Math::Vector HexaLGMapping::EvaluateFor(Element* ele, 
                                            const double& r,
                                            const double& s,
                                            const double& t) {
    // This mapping returns a vector containing the mapping for all three
    // coordinates (x,y,z); we instantiate the result now
    Math::Vector result(3);

    // TODO !!
    // DESIGN THE ALGORITHM FOR NODE ORDERING "REMAPPING" (i.e. from Lagrangian
    // or, better yet, Gmsh ordering, to the natural "Tensor Product ordering")

    return result;
}

// ---------- End of HexaLGMapping Member Function Definitions --------- //



} // end of CHONS namespace