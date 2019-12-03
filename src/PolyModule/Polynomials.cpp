#include "PolyModule/Polynomials.h"
#include "boost/assert.hpp"
#include <cmath>
#include <vector>
#include <algorithm>
#include <iostream>

namespace CHONS {

// ---------- Polynomial Member Function Definitions --------- //


// ---------- End of Polynomial Member Function Definitions --------- //


// ---------- Expansion Member Function Definitions --------- //


// ---------- End of Expansion Member Function Definitions --------- //


// ---------- Interpolant Member Function Definitions --------- //


// ---------- End of Interpolant Member Function Definitions --------- //


// ---------- Jacobi Member Function Definitions --------- //


// ---------- End of Jacobi Member Function Definitions --------- //


Jacobi::Jacobi(const unsigned& al, const unsigned& be, const unsigned& ord)
                : Expansion(ord), s_alpha(al), s_beta(be) {

}

// Evaluate recurrence relations using dynamic programming (bottom-up) approach
double Jacobi::EvaluateAt(const double& x) {
    std::vector<double> low_evaluated{1.0};
    if (s_order > 0) {
        double A, B, C;
        A = 0.5*(s_alpha + s_beta) + 1.0;
        B = 0.5*(s_alpha - s_beta);
        low_evaluated.push_back(A*x + B);

        if (s_order > 1) {
            for (unsigned i = 1; i != s_order; ++i) {
                low_evaluated.push_back((coeffA(i)*x + coeffB(i))
                     * low_evaluated[i]
                     - coeffC(i)*low_evaluated[i-1]);
            }
        }
    }

    return low_evaluated.back();
}

std::vector<double> Jacobi::EvaluateAt(const std::vector<double>& pts) {
    std::vector<double> ev(pts.size());
    for (int i = 0; i != ev.size(); ++i)
        ev[i] = EvaluateAt(pts[i]);

    return ev;
}

double Jacobi::DerivativeAt(const double& x) {
    return 0.5*(s_alpha + s_beta + s_order + 1.0) *
                Jacobi(s_alpha+1,s_beta+1,s_order-1).EvaluateAt(x);
}

std::vector<double> Jacobi::DerivativeAt(const std::vector<double>& pts) {
    std::vector<double> dev(pts.size());
    for (int i = 0; i != dev.size(); ++i)
        dev[i] = DerivativeAt(pts[i]);

    return dev;
}

std::vector<double> Jacobi::GetZeros() {
    const double tol = 1.e-6, PI = 3.14159265359;
    double sum, delta;
    std::vector<double> zeros(s_order, 0);
    for (int i = 0; i != s_order; ++i) {
        zeros[i] = -1.0*std::cos((2.*i+1.)*PI/(2.*s_order));
        if (i != 0) zeros[i] = (zeros[i]+zeros[i-1]) / 2.;

        do {
            sum = 0.;
            for (int j = 0; j != i; ++j) sum += 1./(zeros[i] - zeros[j]);
            delta = - EvaluateAt(zeros[i]) / 
                            (DerivativeAt(zeros[i]) - EvaluateAt(zeros[i])*sum);
            zeros[i] += delta;
        } while (delta > tol);
    }

    return zeros;
}

// ---------- End of Jacobi Member Function Definitions --------- //

// ---------- Legendre Member Function Definitions --------- //

double Legendre::EvaluateAt(const double& x) {
    std::vector<double> low_evaluated{1.0};
    if (s_order > 0) {
        low_evaluated.push_back(x);
        if (s_order > 1) {
            for (int i = 1; i != s_order; ++i)
                low_evaluated.push_back(((2.*i + 1)/(i + 1.))*x*low_evaluated[i] 
                                - (i/(i+1.0))*low_evaluated[i-1]);
        }
    }
    return low_evaluated.back();
}

std::vector<double> Legendre::EvaluateAt(const std::vector<double>& pts) {
    std::vector<double> ev(pts.size());
    for (int i = 0; i != ev.size(); ++i)
        ev[i] = EvaluateAt(pts[i]);

    return ev;
}

// ---------- End of Legendre Member Function Definitions --------- //


// ---------- Lagrange Member Function Definitions --------- //

double Lagrange::EvaluateAt(const double& x, const unsigned& pt) {
    BOOST_ASSERT_MSG(pt < s_points.size(), "Maximum index 'j' of Lagrange"
                                " evaluation exceeded (pt > s_points.size()");
    double prod = 1.0;
    for (int i = (pt ? 0 : 1); i != s_points.size(); (i == pt-1) ? i+=2 : ++i) {
        prod *= ((x - s_points[i]) / (s_points[pt] - s_points[i]));
    }
    return prod;
}

std::vector<double> Lagrange::EvaluateAt(const std::vector<double>& pts,
                                            const unsigned& pt) { 
    std::vector<double> evs(pts.size(),0.0);
    for (int i = 0; i != evs.size(); ++i)
        evs[i] = EvaluateAt(pts[i], pt);

    return evs;
}

double Lagrange::DerivativeAt(const double& x, const unsigned& pt) {
    double deriv = 0.0, prod = 1.0;
    for (int i = (pt ? 0 : 1); i != s_points.size(); (i == pt-1) ? i+=2 : ++i) {
        prod = 1.0;
        for (int m = 0; m != s_points.size(); ++m) {
            if ((m != pt) && (m != i)) 
                prod *= ((x - s_points[m])/(s_points[pt] - s_points[m]));
        }
        deriv += (1./(s_points[pt] - s_points[i]))*prod;
    }

    return deriv;
}

std::vector<double> Lagrange::DerivativeAt(const std::vector<double>& pts,
                                            const unsigned& pt) { 
    std::vector<double> evs(pts.size(), 0.0);
    for (int i = 0; i != evs.size(); ++i)
        evs[i] = DerivativeAt(pts[i], pt);

    return evs;
}

std::vector<double> Lagrange::GetZeros(const unsigned& pt) {
    std::vector<double> zs{s_points.begin(), s_points.end()};
    auto end = std::remove(zs.begin(), zs.end(), s_points[pt]);
    return std::vector<double>(zs.begin(), end);
}

// ---------- End of Lagrange Member Function Definitions --------- //



} // end of CHONS namespace