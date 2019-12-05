// Classes responsible for holding affine mapping info

#ifndef MAPPINGS_H
#define MAPPINGS_H

#include "MeshModule/ElementEnumInfo.h"
#include "PolyModule/Polynomials.h"
#include "MathModule/LinAlgEntities.h"
#include "PolyModule/Basis.h"
#include "PolyModule/BasisFactory.h"


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
        unsigned s_order;
        // Math::Vector s_elementNodes; this will be our "extrinsic state"
        // double s_jacobian; calculated through an extrinsic state as well

        

};

class LagrangeElementMapping : public StdToPhysMapping {
    public:
        LagrangeElementMapping(Element*);


    protected:
        Basis* s_mappingBasis; // "intrinsic state" of the flyweight object

        // This factory is needed only while we don't have a ResourceDirector
        // class; once we do, every allocation request will be made through it,
        // which will be static and hence won't need a member object
        InterpolationBasisFactory s_basisFac;

};

class TensorProductLGMapping : public LagrangeElementMapping {
    public:
        TensorProductLGMapping(Element*);
        virtual Math::Vector EvaluateFor(Element*, const double&, 
                                    const double& = -2, const double& = -2) = 0;
        virtual double Jacobian(Element*, const double&, 
                                    const double& = -2, const double& = -2) = 0;


    protected:        

};

class LineLGMapping : public TensorProductLGMapping {
    public:
        LineLGMapping(Element*);
        virtual Math::Vector EvaluateFor(Element*, const double&, 
                                    const double& = -2, const double& = -2);
        virtual double Jacobian(Element*, const double&, 
                                    const double& = -2, const double& = -2);

    private:

};

class QuadLGMapping : public TensorProductLGMapping {
    public:
        QuadLGMapping(Element*);
        virtual Math::Vector EvaluateFor(Element*, const double&, 
                                    const double& = -2, const double& = -2);
        virtual double Jacobian(Element*, const double&, 
                                    const double& = -2, const double& = -2);


    private:
};

} // end of CHONS namespace

#endif