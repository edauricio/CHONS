// Classes to hold expansion/interpolation basis info, depending on the element

#ifndef BASIS_H
#define BASIS_H

#include "MeshModule/ElementEnumInfo.h"
#include "MeshModule/ElementComposite.h"
#include "PolyModule/Polynomials.h"
#include "MathModule/LinAlgEntities.h"

namespace CHONS {

using namespace Math;

class Basis {
    public:
        // Constructors
        Basis();

        // Destructor
        virtual ~Basis();

        // Member functions
        virtual Vector EvaluateAt(const double&) = 0;
        virtual Matrix EvaluateAt(const double&, const double&) = 0;

    protected:
        // Member functions
        

    private:

};

class TensorProductBasis : public Basis {
    public:
        TensorProductBasis(ElementType);

        

    protected:
        std::vector<Polynomial*> s_polys;
        unsigned s_dim;
};

class InterpolationTPBasis : public TensorProductBasis {
    public:
        InterpolationTPBasis(ElementType,
                        const std::initializer_list<std::vector<double>>&);

        virtual ~InterpolationTPBasis();

    virtual Vector EvaluateAt(const double&);
    virtual Matrix EvaluateAt(const double&, const double&);


    private:
        Tensor* s_oneDBasis;

};

class ExpansionTPBasis: public TensorProductBasis {

};

} // end of CHONS namespace

#endif