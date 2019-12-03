#include "catch2/include/catch.hpp"
#include "PolyModule/Polynomials.h"
#include <algorithm>

using namespace CHONS;

TEST_CASE("Lagrange evaluation test", "[lgeval]") {
    // Setting up points
    Legendre pzs(5);
    std::vector<double> zs = pzs.GetZeros();

    Lagrange poly(zs);
    std::vector<double> interp_pts {3.99868964, 0.03698113, 2.76282799, 
                                                    2.19907537, 0.37861344};

    std::vector<double> eval_pts{-0.52, -0.23, 0., 0.66, 0.9};
    std::vector<double> result{0.0588232452, 1.472518811689, 2.7628279949,
                                1.4860715246, 0.3898535172};

    // Calculating the interpolation for each point eval_pts and comparing
    // to result, given by SciPy
    for (int j = 0; j != eval_pts.size(); ++j) {
        double sum = 0;
        for (int i = 0; i != interp_pts.size(); ++i)
            sum += (interp_pts[i]*poly.EvaluateAt(eval_pts[j], i));

        REQUIRE(sum == Approx(result[j]));
    }
}

TEST_CASE("Lagrange bulk evaluation test", "[lgbulkeval]") {
    // Setting up points
    Legendre pzs(6);
    std::vector<double> zs = pzs.GetZeros();

    Lagrange poly(zs);

    std::vector<double> interp_pts {0.26281814, 0.65413542, 0.19508567, 
                                    1.74067667, 2.10150842, 1.21751251};

    std::vector<double> eval_pts{-0.84, -0.33, 0., 0.41, 0.84};

    std::vector<double> result {0.69973856726, 0.135167123, 0.82828103,
                                        2.187788172, 1.540545590};

    std::vector<double> sum(eval_pts.size());
    for (int i = 0; i != poly.GetOrder()+1; ++i) {
        std::vector<double> bulk_eval = poly.EvaluateAt(eval_pts, i);
        for (int j = 0; j != sum.size(); ++j)
            sum[j] += interp_pts[i]*bulk_eval[j];
    }

    for (int i = 0; i != sum.size(); ++i)
        REQUIRE(sum[i] == Approx(result[i]));
}

TEST_CASE("Lagrange derivative test", "[lgderiv]") {
    // Setting up points
    Legendre pzs(6);
    std::vector<double> zs = pzs.GetZeros();

    Lagrange poly(zs);

    std::vector<double> interp_pts {0.26281814, 0.65413542, 0.19508567, 
                                    1.74067667, 2.10150842, 1.21751251};

    std::vector<double> eval_pts{-0.84, -0.33, 0., 0.41, 0.84};

    std::vector<double> result{2.407199889, 0.0250572884, 3.6911299716,
                                        1.568683093, -3.652148828};

    for (int j = 0; j != eval_pts.size(); ++j) {
        double sum = 0;
        for (int i = 0; i != interp_pts.size(); ++i)
            sum += interp_pts[i]*poly.DerivativeAt(eval_pts[j], i);

        REQUIRE(sum == Approx(result[j]));
    }
}

TEST_CASE("Lagrange bulk derivative test", "[lgbulkder]") {
    // Setting up points
    Legendre pzs(5);
    std::vector<double> zs = pzs.GetZeros();

    Lagrange poly(zs);

    std::vector<double> interp_pts {3.99868964, 0.03698113, 2.76282799, 
                                                    2.19907537, 0.37861344};

    std::vector<double> eval_pts{-0.52, -0.23, 0., 0.66, 0.9};

    std::vector<double> result{1.534754365, 6.3997172832, 4.19369633,
                                    -5.96347585593, -1.9233901611};

    std::vector<double> sum(eval_pts.size());
    for (int i = 0; i != poly.GetOrder()+1; ++i) {
        std::vector<double> bulk_eval = poly.DerivativeAt(eval_pts, i);
        for (int j = 0; j != sum.size(); ++j)
            sum[j] += interp_pts[i]*bulk_eval[j];
    }

    for (int i = 0; i != sum.size(); ++i)
        REQUIRE(sum[i] == Approx(result[i]));

}

TEST_CASE("Lagrange zeros test", "[lgzeros]") {
    // Setting up points
    Legendre pzs(5);
    std::vector<double> zs = pzs.GetZeros();

    Lagrange poly(zs);

    std::vector<double> exact_zs(poly.GetOrder());
    
    // Check zeros for every lagrange polynomial index "i"
    for (int i = 0; i != poly.GetOrder()+1; ++i) {
        std::vector<double> calc_zs = poly.GetZeros(i);
        std::remove_copy(zs.begin(), zs.end(), exact_zs.begin(), zs[i]);
        for (int j = 0; j != poly.GetOrder(); ++j)
            REQUIRE(calc_zs[j] == Approx(exact_zs[j]));
    }
}