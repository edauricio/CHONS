// Classes responsible for holding affine mapping info

#ifndef MAPPINGS_H
#define MAPPINGS_H

#include "MeshModule/ElementEnumInfo.h"
#include "PolyModule/Polynomials.h"
#include "MathModule/LinAlgEntities.h"


namespace CHONS {

// This base class is only necessary so that we can manage the flyweight 
// objects properly using the MappingFactory.
class StdToPhysMapping {
    public:
        StdToPhysMapping(const unsigned&);

        virtual Math::Vector EvaluateFor(Element*, const double&, 
                                    const double& = -2, const double& = -2) = 0;
        virtual double Jacobian(Element*) = 0;

    protected:
        unsigned s_elementOrder;
        double s_jacobian;

};

class LineMapping : public StdToPhysMapping {
    public:
        LineMapping(Element*);

        virtual Math::Vector EvaluateFor(Element*, const double&, 
                                    const double& = -2, const double& = -2);
        virtual double Jacobian(Element*);

    private:
        Polynomial* s_poly;
        Math::Vector s_basis;
        Math::Vector s_edgePoints;
};

class QuadMapping : public StdToPhysMapping {
    public:
        QuadMapping(Element*);

    private:

};

} // end of CHONS namespace


#endif