#include "catch2/include/catch.hpp"
#include "PolyModule/Polynomials.h"
#include <vector>

using namespace CHONS;

TEST_CASE("Legendre evaluation test", "[legeval]") {
    Legendre poly(5);
    double value = poly.EvaluateAt(-0.98);

    REQUIRE(value == Approx(-0.7204462748));

    value = poly.EvaluateAt(-0.6);

    REQUIRE(value == Approx(0.15264));

    value = poly.EvaluateAt(-0.12);

    REQUIRE(value == Approx(-0.2100759552));

    value = poly.EvaluateAt(0);

    REQUIRE(value == Approx(0.));

    value = poly.EvaluateAt(0.26);

    REQUIRE(value == Approx(0.3430665836));

    value = poly.EvaluateAt(0.6);

    REQUIRE(value == Approx(-0.15264));


    poly = Legendre(4);
    std::vector<double> pts = {-0.98, -0.6, -0.12, 0, 0.26, 0.6};
    std::vector<double> evals = poly.EvaluateAt(pts);
    std::vector<double> ans = {0.8088607, -0.408, 0.3219072, 0.375, 
                                    0.1414927, -0.408};

    for (int i = 0; i != pts.size(); ++i)
        REQUIRE(evals[i] == Approx(ans[i]));
}