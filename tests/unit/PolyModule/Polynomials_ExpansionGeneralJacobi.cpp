#include "catch2/include/catch.hpp"
#include "PolyModule/Polynomials.h"
#include <iostream> // remove 

using namespace CHONS;

TEST_CASE("Jacobi evaluation test", "[jaceval]") {
    Jacobi poly(2, 1, 3);
    double value = poly.EvaluateAt(-0.9);

    REQUIRE(value == Approx(-2.1695));

    value = poly.EvaluateAt(-0.62);

    REQUIRE(value == Approx(0.512956));

    value = poly.EvaluateAt(-0.34);

    REQUIRE(value == Approx(0.681908));

    value = poly.EvaluateAt(0.);

    REQUIRE(value == Approx(-0.5));

    value = poly.EvaluateAt(0.44);

    REQUIRE(value == Approx(-0.467968));

    value = poly.EvaluateAt(0.88);

    REQUIRE(value == Approx(6.285856));
}

TEST_CASE("Jacobi bulk evaluation test", "[jacbulkeval]") {
    Jacobi poly(2, 2, 4);
    std::vector<double> pts = {-0.98, -0.6, -0.12, 0, 0.26, 0.6};
    std::vector<double> evals = poly.EvaluateAt(pts);
    std::vector<double> ans = {13.26651495, -1.128, 0.7009152, 0.9375, 
                                -0.06187305, -1.128};

    for (int i = 0; i != pts.size(); ++i)
        REQUIRE(evals[i] == Approx(ans[i]));
}

TEST_CASE("Jacobi derivative evaluation test", "[jacderiv]") {
    int alpha = 2, beta = 3, n = 4;
    Jacobi poly(alpha, beta, n);
    Jacobi polyder(alpha+1, beta+1, n-1);

    double vals[] = {-0.8, -0.53, -0.28, 0., 0.34, 0.67, 0.97};

// Here we rely on the formula for Jacobi polynomial derivatie, i.e.
// \frac{d}{dx}P_n^{(\alpha,\beta)} = 
//              \frac{1}{2}(n + \alpha + \beta + 1)P_{n-1}^{(\alpha+1,\beta+1)}
    for (int i = 0; i != 7; ++i)
        REQUIRE(poly.DerivativeAt(vals[i]) 
                == Approx(0.5
                    * (n + alpha + beta + 1)
                    * polyder.EvaluateAt(vals[i])));
}

TEST_CASE("Jacobi bulk derivative evaluation test", "[jacbulkder]") {
    int alpha = 2, beta = 3, n = 4;
    Jacobi poly(alpha, beta, n);
    Jacobi polyder(alpha+1, beta+1, n-1);

    std::vector<double> vals{-0.8, -0.53, -0.28, 0., 0.34, 0.67, 0.97};
    std::vector<double> dpdx_lhs = poly.DerivativeAt(vals);
    std::vector<double> dpdx_rhs = polyder.EvaluateAt(vals);
    
    for (auto& update : dpdx_rhs)
        update *= (0.5*(n + alpha + beta + 1));

    for (int i = 0; i != vals.size(); ++i)
        REQUIRE(dpdx_lhs[i] == Approx(dpdx_rhs[i]));
}

TEST_CASE("Jacobi zeros test", "[jaczeros]") {
    Jacobi poly(2, 3, 4);
    std::vector<double> calc_zeros = poly.GetZeros();
    std::vector<double> ex_zs {-0.59903471, -0.14761105, 0.32554377, 0.7287943};

    for (int i = 0; i != calc_zeros.size(); ++i)
        REQUIRE(calc_zeros[i] == Approx(ex_zs[i]));
}