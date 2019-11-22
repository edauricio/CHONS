// Class to define polynomials of different types

#ifndef POLYNOMIALS_H
#define POLYNOMIALS_H

#include <cmath>
#include <vector>
#include <string>


namespace CHONS {

class Polynomial {
    public:
        Polynomial(const unsigned& ord) : s_order(ord) {}

        unsigned GetOrder() { return s_order; }
        virtual std::string GetType() = 0;

    protected:
        unsigned s_order;

};

class Expansion : public Polynomial {
    public:
        Expansion(const unsigned& ord) : Polynomial(ord) {}

        virtual double EvaluateAt(const double&) = 0;
        virtual std::vector<double> EvaluateAt(const std::vector<double>&) = 0;
        virtual double DerivativeAt(const double&) = 0;
        virtual std::vector<double> DerivativeAt(const std::vector<double>&) = 0;
        virtual std::vector<double> GetZeros() = 0;
        virtual std::string GetType() {return "Expansion";};

    private:

};

class Interpolant : public Polynomial {
    public:
        Interpolant(const std::vector<double>& pt) : Polynomial(pt.size()-1),
                                            s_points(pt) {}

        virtual double EvaluateAt(const double&, const unsigned&) = 0;
        virtual std::vector<double> EvaluateAt(const std::vector<double>&,
                                                const unsigned&) = 0;
        virtual double DerivativeAt(const double&, const unsigned&) = 0;
        virtual std::vector<double> DerivativeAt(const std::vector<double>&, 
                                                    const unsigned&) = 0;
        virtual std::vector<double> GetZeros(const unsigned&) = 0;
        virtual std::string GetType() {return "Interpolant";};

    protected:
        std::vector<double> s_points;

};

class Jacobi : public Expansion {
    public:
        // Alpha, Beta, Order
        Jacobi(const unsigned&, const unsigned&, const unsigned&);

        virtual double EvaluateAt(const double&);
        virtual std::vector<double> EvaluateAt(const std::vector<double>&);
        virtual double DerivativeAt(const double&);
        virtual std::vector<double> DerivativeAt(const std::vector<double>&);
        virtual std::vector<double> GetZeros();


    private:
        inline double coeffA(const unsigned& n) {
            return (2.0*n + s_alpha + s_beta + 1.0) *
                    (2.0*n + s_alpha + s_beta + 2.0) /
                    (2.*(n + 1.0)*(n + s_alpha + s_beta + 1.0)); }

        inline double coeffB(const unsigned& n) {
            return (std::pow(s_alpha,2) - std::pow(s_beta,2)) *
                    (2.0*n + s_alpha + s_beta + 1.0) /
                    
                    (2.*(n + 1.0)*(n + s_alpha + s_beta + 1.0) *
                    (2.*n + s_alpha + s_beta)); }

        inline double coeffC(const unsigned& n) {
            return (n + s_alpha)*(n + s_beta)*(2.0*n + s_alpha + s_beta + 2.0) /
                    
                    ((n + 1.0)*(n + s_alpha + s_beta + 1.0) *
                    (2*n + s_alpha + s_beta)); }

        unsigned s_alpha;
        unsigned s_beta;

};

class Legendre : public Jacobi {
    public:
        Legendre(const unsigned& ord) : Jacobi(0,0,ord) {}

        virtual double EvaluateAt(const double&);
        virtual std::vector<double> EvaluateAt(const std::vector<double>&);

    private:

};

class Lagrange : public Interpolant {
    public:
        Lagrange(const std::vector<double>& pts) : Interpolant(pts) {}

        virtual double EvaluateAt(const double&, const unsigned&);
        virtual std::vector<double> EvaluateAt(const std::vector<double>&,
                                                const unsigned&);
        virtual double DerivativeAt(const double&, const unsigned&);
        virtual std::vector<double> DerivativeAt(const std::vector<double>&,
                                                    const unsigned&);
        virtual std::vector<double> GetZeros(const unsigned&);

    private:

};

} // end of CHONS namespace



#endif