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

double LineLGMapping::Jacobian(Element* ele, const double& r, 
                                            const double& s, 
                                            const double& t) {
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

    // Allocate vector for the quad node elements
    std::vector<Element*> nodes( (s_order+1) * (s_order+1) );

    // Get all edges and interior nodes first
    // Remember that the primitives list is ordered first with the edges (in
    // gmsh order, or usual lagrange ordering of nodes) and then the interior
    // nodes (again in gmsh / usual lagrange element ordering)
    std::vector<Element*> allprim = ele->GetPrimitives();

    // Now organize Quad nodes into 'nodes' vector in a "tensor product" order
    // Rememeber that the Lagrangian element ordering is not the same as the
    // "natural tensor product" ordering that we conventioned in the code (in
    // Basis classes); thus this rearragement is needed

    // The first edge nodes are all in the correct order, and they're the first
    // so, easy looping
    for (int i = 0; i != s_order+1; ++i)
        nodes[i] = allprim[0]->GetPrimitives()[i];

    // Now, we get the primitive nodes of the last (4th) edge and put them
    // in order in vector 'nodes', noting that its last interior node element 
    // will be the next in the sequence above, and the second to last will be 
    // sequence positioned strided s_order elements from this last node, and
    // so on...
    // Note that only the interior edge nodes are taken into account here, since
    // the vertex one will be handled later by the 3rd edge
    // (Clearly, either approach could be taken, though)
    for (int i = 0; i != s_order-1; ++i)
        nodes[(s_order+1) /* initial position */
            + i*s_order /* stride */] = 
                                    allprim[3]->GetPrimitives()[(s_order-1)-i];

    // The 2nd edge now is handled; again, only the interior nodes will be
    // handled in this loop.
    // The sequence starts at 2*s_order+1 (due to C++ 0-indexing; otherwise it
    // would start at 2*(s_order+1)) and each consecutive interior node of
    // the edge is strided s_order elements from the previous one
    for (int i = 0; i != s_order-1; ++i)
        nodes[(2*s_order+1) /* initial position */
            + i*s_order /* stride */] = allprim[1]->GetPrimitives()[i+1];

    // Now the 3rd and last edge of the quad element.
    // This element holds the last nodes of the tensor product sequence, but
    // in reverse order. It's still a straightforward loop, though
    // Note that the top vertices nodes are handled in this case, not in
    // 2nd or 4th edges cases (those handle only interior edge nodes)
    // The sequence here clearly starts s_order elements before the last
    // position in 'nodes' vector.
    for (int i = 0; i != s_order+1; ++i)
        nodes[((s_order+1) * (s_order+1) - 1) - s_order/* initial position */
            + i] = allprim[2]->GetPrimitives()[s_order-i];

    // Note that these 4 loops could be merged into only two loops, but they'll
    // remain in 4 separate loops for the sake of clarity.
    // We now turn to the interior nodes

    // Remembering that the interior nodes are ordered just as a "smaller quad 
    // inside the bigger quad", we should first deal with the "interior vertex"
    // nodes.
    // First, the loop for bottom vertices. The initial position in the 'nodes'
    // vector is s_order+2 (i.e. s_order+1 for the bottom edge nodes, plus the
    // one in the 4th edge before the interior node), and the first and second
    // bottom interior vertex are strided s_order-3 elements from each other
    // (s_order-2 is the "order" of the interior quad)
    for (int i = 4; (i < nodes.size()) && (i != 6); ++i)
        nodes[s_order+2 /* initial position */
            + (i-4)*(s_order-2) /* stride */] = allprim[i];

    // The top vertices of the inside quad are the next on the allprim list,
    // but they're in reverse order. The initial position is now ( (nodes.size()
    // -1) - s_order - 1 - 1), i.e., we start at the last position,run backwards
    // all the nodes in the top edge, then count one less for the rightmost node
    // (the last interior node from edge 2).
    // The second top vertex of the inside quad is stride (s_order-2) elements
    // backwards relative to the first top vertex just added to the sequence
    // (again, note that (s_order-2) is the "order" of the inside quad)
    for (int i = 6; (i < nodes.size()) && (i != 8); ++i)
        nodes[((nodes.size()-1) - s_order - 1 - 1) /* initial position */
            - (i-6)*(s_order-2) /* stride */] = allprim[i];

    // Next in the primitives sequence are the interior nodes on the "bottom
    // interior edge". In the node ordering sequence the initial position of the
    // first interior node of the edge is (s_order+1+1), i.e., after all the 
    // nodes from the bottom edge (s_order), plus the leftmost node, comming
    // from the 4th (leftmost) edge, plus the bottom-left vertex of the
    // interior quad. Since in the primitive list they're in sequence, there's
    // no stride now
    for (int i = 8; (i < nodes.size()) && (i != 8+s_order-2-1); ++i)
        nodes[(s_order+1+1+1) /* initial position */
            + (i-8) /* increment */] = allprim[i];

    // Now for the "right interior edge" of the first interior quad.
    // The position in the tensor product sequence is now 
    // 2*(s_order+1)+1+s_order-2, i.e. the first multiplication accounts for the
    // for the two "bottom edges" (quad bottom edge and "first interior quad" 
    // bottom edge) and includes the first node of the leftmost quad (4th) edge 
    // (due to 0-indexing); then, plus one for the first leftmost interior node
    // of the "first interior quad (4th) edge" and finally plus the nodes on the 
    // bottom edge of the quad inner to this interior one we're dealing now.
    // The stride is now 1+1+1+s_order-2; the one's additions account for the
    // rightmost node (interior node of 2nd quad edge), plus the leftmost node
    // of the next (upper) horizontal line, plus the node of the interior
    // leftmost edge of the inner quad; the s_order-2 is again for the nodes
    // interior to this inner quad (thus pertaining to an edge of a still
    // inner quad, if the element order is that high)
    for (int i = 8+s_order-2-1; (i < nodes.size()) && 
                                (i != (8+s_order-2-1)+s_order-2-1); ++i)
        nodes[2*(s_order+1) + 1 + s_order-2 /* initial position */
            + (i-(8+s_order-2-1))*(1+1+1 + s_order-2) /* stride */] 
            = allprim[i];


    // For the "top interior edge", the nodes are again in sequence, but in
    // reverse order. The initial position may be calculated like the initial
    // position for the bottom edge, but running in the opposite direction, i.e.
    // we start at the end position of the 'nodes' vector and run backwards
    // all the nodes in the outermost, quad top edge, then subtract those
    // accounting for the rightmost node (the interior node to the 2nd quad
    // edge) and the top-right vertex node of the inner

    // Populate the vectors containing the x- and y-nodes coordinates
    // TODO: Challenge now is to populate these coordinate vectors into
    // the corresponding basis order, i.e. a "tensor product" order, not a
    // 'lagrangian element ordering'
    Math::Vector pts_x(nodes.size()), pts_y(nodes.size());





    return result;
}

// ---------- End of QuadLGMapping Member Function Definitions --------- //


} // end of CHONS namespace