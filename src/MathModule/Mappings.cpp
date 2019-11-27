#include "MathModule/Mappings.h"
#include "MeshModule/ElementComposite.h"

namespace CHONS {

LineMapping::LineMapping(Element* ele) : s_basis(ele->GetElementOrder()+1),
                                        s_edgePoints(ele->GetElementOrder()+1) {
    // WARNING
    // This mapping should ONLY be created when the problem is 1D.
    // Should we insert an assert here to guarantee it?
    int i = 0;
    for (auto& prim : ele->GetPrimitives())
        s_edgePoints[i] = prim->GetCoords().front();
    // The type of polynomial will depend on the type of solver (Nodal/Modal)
    // Further iterations should cover such information (probably global?)
    std::vector<double> points(s_edgePoints.begin(), s_edgePoints.end());
    
    s_poly = new Lagrange{points};
}

// REFACTOR THIS OVERLOADING
// IT IS WRONG BECAUSE IT IS NOT IMPLEMENTED IN TERMS OF POINTS NUMBER, BUT COORD
Vector LineMapping::operator()(const int& n) {
    for (int i = 0; i != s_basis.size(); ++i)
        s_basis[i] = dynamic_cast<Interpolant*>(s_poly)->EvaluateAt(n, i);

    return Vector{dot_product(s_edgePoints, s_basis), 0, 0};
}

} // end of CHONS namespace