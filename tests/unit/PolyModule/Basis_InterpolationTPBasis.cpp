#include "catch2/include/catch.hpp"
#include "PolyModule/Basis.h"
#include "PolyModule/BasisFactory.h"
#include "MeshModule/ElementFactory.h"
#include "MeshModule/ElementComposite.h"
#include "MeshModule/ElementEnumInfo.h"
#include <iostream> // remove

using namespace CHONS;

TEST_CASE("Line interpolation basis test", "[lineinterp]") {
    InterpolationBasisFactory* basis_fac = new InterpolationBasisFactory;
    ElementFactory* ele_fac = ElementFactory::GetInstance();

    ElementInfo einfo, priminfo;
    einfo.tag = 1;
    einfo.type = eLine;
    einfo.eleOrder = 2;

    priminfo.type = eNode;
    std::vector<double> nodes_x{5.23, 5.89, 6.55};
    for (int i = 0; i != einfo.eleOrder; ++i) {
        priminfo.tag = i+1;
        priminfo.coords = std::vector<double>{nodes_x[i], 0., 0.};
        ele_fac->OrderElement(priminfo);
    }
    ele_fac->PlaceOrder();

    for (int i = 0; i != einfo.eleOrder; ++i) {
        priminfo.tag = i+1;
        einfo.prims.push_back(ele_fac->GetElement(priminfo));
    }

    ele_fac->OrderElement(einfo);
    ele_fac->PlaceOrder();

    Basis* basis;
    std::vector<double> pts;
    pts.push_back(-1.0);
    double dx = 2.0 / einfo.eleOrder;
    for (int i = 1; i != einfo.eleOrder+1; ++i) {
        pts.push_back(pts[i-1] + dx);
    }

    basis = basis_fac->GetBasis(ele_fac->GetElement(einfo), {pts});

    // Using std::vector so we don't need to include and compile LinAlgEntities
    // (also no Makefile modification needed --- hence, simpler)
    std::vector<double> basisvec = basis->EvaluateAt(0.25);

    std::vector<double> result{-0.09375, 0.9375, 0.15625};

    for (int i = 0; i != basisvec.size(); ++i)
        REQUIRE(basisvec[i] == Approx(result[i]));
}

TEST_CASE("Quad interpolation basis test", "[quadinterp]") {
    InterpolationBasisFactory* basis_fac = new InterpolationBasisFactory;
    ElementFactory* ele_fac = ElementFactory::GetInstance();

    ElementInfo einfo, priminfo;
    einfo.tag = 1;
    einfo.type = eQuad;
    einfo.eleOrder = 2;

    // Since in this Basis test we're not interested in correct mapping, the
    // nodes here are just for Quad creation, so their coords have no useful
    // meaning.
    priminfo.type = eNode;
    std::vector<double> nodes_x{5.23, 5.89, 6.55};
    std::vector<double> nodes_y{1.2, 1.8, 1.9};
    for (int j = 0; j != einfo.eleOrder+1; ++j) {
        for (int i = 0; i != einfo.eleOrder+1; ++i) {
            priminfo.tag = j*(einfo.eleOrder+1) + i + 1;
            priminfo.coords = std::vector<double>{nodes_x[i], nodes_y[j], 0.};
            ele_fac->OrderElement(priminfo);
        }
    }

    ele_fac->PlaceOrder();

    for (int i = 0; i != einfo.eleOrder+1; ++i) {
        for (int j = 0; j != einfo.eleOrder+1; ++j) {
            priminfo.tag = i*einfo.eleOrder + j + 1;
            einfo.prims.push_back(ele_fac->GetElement(priminfo));
        }
    }

    ele_fac->OrderElement(einfo);
    ele_fac->PlaceOrder();
    Basis* basis;
    std::vector<double> pts_x;
    pts_x.push_back(-1.0);
    double dx = 2.0 / einfo.eleOrder;
    for (int i = 1; i != einfo.eleOrder+1; ++i) {
        pts_x.push_back(pts_x[i-1] + dx);
    }
    std::vector<double> pts_y(pts_x);

    basis = basis_fac->GetBasis(ele_fac->GetElement(einfo), {pts_x, pts_y});

    // Using std::vector so we don't need to include and compile LinAlgEntities
    // (also no Makefile modification needed --- hence, simpler)
    std::vector<double> basisvec = basis->EvaluateAt(0.25, 0.25);

    std::vector<double> result{0.0087890625, -0.087890625, -0.01464843, 
                            -0.08789062, 0.87890625, 0.146484375,
                            -0.0146484375, 0.146484675, 0.024414062};


    for (int i = 0; i != basisvec.size(); ++i)
        REQUIRE(basisvec[i] == Approx(result[i]));
}

TEST_CASE("Hexa interpolation basis test", "[quadinterp]") {
    InterpolationBasisFactory* basis_fac = new InterpolationBasisFactory;
    ElementFactory* ele_fac = ElementFactory::GetInstance();

    ElementInfo einfo, priminfo;
    einfo.tag = 1;
    einfo.type = eQuad;
    einfo.eleOrder = 2;

    // Since in this Basis test we're not interested in correct mapping, the
    // nodes/elements here are just for Hexa creation, 
    // so their coords have no useful meaning. Similarly, Quad and Hexa creation
    // have no useful meaning (e.g. Hexa is created from the same 6 Quad faces)
    priminfo.type = eNode;
    std::vector<double> nodes_x{5.23, 5.89, 6.55};
    std::vector<double> nodes_y{1.2, 1.8, 1.9};
    for (int k = 0; k != einfo.eleOrder+1; ++k) {
        for (int j = 0; j != einfo.eleOrder+1; ++j) {
            for (int i = 0; i != einfo.eleOrder+1; ++i) {
                priminfo.tag = j*(einfo.eleOrder+1) + i + 1;
                priminfo.coords = std::vector<double>{nodes_x[i], nodes_y[j], 0.};
                ele_fac->OrderElement(priminfo);
            }
        }
        ele_fac->PlaceOrder();
    }

    for (int i = 0; i != einfo.eleOrder+1; ++i) {
        for (int j = 0; j != einfo.eleOrder+1; ++j) {
            priminfo.tag = i*einfo.eleOrder + j + 1;
            einfo.prims.push_back(ele_fac->GetElement(priminfo));
        }
    }

    ele_fac->OrderElement(einfo);
    ele_fac->PlaceOrder();

    priminfo.clear();
    einfo.clear();
    priminfo.type = eQuad;
    priminfo.tag = 1;
    einfo.type = eHexa;
    einfo.tag = 1;
    for (int i = 0; i != 6; ++i)
        einfo.prims.push_back(ele_fac->GetElement(priminfo));

    ele_fac->OrderElement(einfo);
    ele_fac->PlaceOrder();
    Basis* basis;
    std::vector<double> pts_x;
    pts_x.push_back(-1.0);
    double dx = 2.0 / einfo.eleOrder;
    for (int i = 1; i != einfo.eleOrder+1; ++i) {
        pts_x.push_back(pts_x[i-1] + dx);
    }
    std::vector<double> pts_y(pts_x);

    basis = basis_fac->GetBasis(ele_fac->GetElement(einfo), {pts_x, pts_y, pts_x});

    // Using std::vector so we don't need to include and compile LinAlgEntities
    // (also no Makefile modification needed --- hence, simpler)
    std::vector<double> basisvec = basis->EvaluateAt(0.25, 0.25, 0.25);

    std::vector<double> result{-0.000823974609375,
                                0.00823974609375,
                                0.001373291015625,
                                0.00823974609375,
                                -0.0823974609375,
                                -0.01373291015625,
                                0.001373291015625,
                                -0.01373291015625,
                                -0.002288818359375,
                                0.00823974609375,
                                -0.0823974609375,
                                -0.01373291015625,
                                -0.0823974609375,
                                0.823974609375,
                                0.1373291015625,
                                -0.01373291015625,
                                0.1373291015625,
                                0.02288818359375,
                                0.001373291015625,
                                -0.01373291015625,
                                -0.002288818359375,
                                -0.01373291015625,
                                0.1373291015625,
                                0.02288818359375,
                                -0.002288818359375,
                                0.02288818359375,
                                0.003814697265625};

    for (int i = 0; i != basisvec.size(); ++i)
        REQUIRE(basisvec[i] == Approx(result[i]));
}