// Classes responsible for holding affine mapping info

#ifndef MAPPINGS_H
#define MAPPINGS_H

#include "MeshModule/ElementEnumInfo.h"
#include "PolyModule/Polynomials.h"
#include "MathModule/LinAlgEntities.h"


namespace CHONS {

using namespace Math;

// This base class is only necessary so that we can manage the flyweight 
// objects properly using the MappingFactory.
class StdToPhysMapping {
    public:
        StdToPhysMapping();

        // This operation is parameterized in one single variable so that the
        // interface is the same for all transforms. This parameter is the 
        // point NUMBER; This numbering system must be made consistent
        // throughout elements.
        // Remember that when we talk about node number, we're talking about
        // the node numbering system which is consistent and quite clear for
        // each element (e.g. see Gmsh manual); however, when we talk about
        // point number, we're talking about the "quadrature" or "nodal" points,
        // which not only vary according to element and polynomial approx. order,
        // but also vary in its own nature (e.g. GL, LGL, EvenlySpaced, etc)

        // The Vector return type returns a coordinate vector (x,y,z)
        virtual Vector operator()(const int&) = 0;

    protected:

};

class LineMapping : public StdToPhysMapping {
    public:
        LineMapping(Element*);

        virtual Vector operator()(const int&);

    private:
        Polynomial* s_poly;
        Vector s_edgePoints;
        Vector s_basis;
};

class QuadTransform : public StdToPhysMapping {

};

} // end of CHONS namespace


#endif