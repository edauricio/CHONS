#include "catch2/include/catch.hpp"
#include "MathModule/Mappings.h"
#include "MeshModule/ElementFactory.h"
#include "MeshModule/ElementEnumInfo.h"

using namespace CHONS;

TEST_CASE("Line mapping test", "[linemap]") {
    ElementFactory *fac = ElementFactory::GetInstance();
    ElementInfo einfo;
    einfo.type = eNode;

    double xinf = -10.0, xsup = 10.0;
    einfo.tag = 1;
    std::vector<double> coords{xinf, 0., 0.};
    einfo.coords = coords;
    fac->OrderElement(einfo);

    einfo.tag = 2;
    coords = {xsup, 0., 0.};
    einfo.coords = coords;
    fac->OrderElement(einfo);

    int order = 2;
    double d = (xsup - xinf) / order;
    // Create each node
    for (int i = 1; i != order; ++i) {
        einfo.tag = i+2;
        coords = {xinf + i*d, 0., 0.};
        einfo.coords = coords;
        fac->OrderElement(einfo);
    }

    fac->PlaceOrder();

    // Create edge element
    einfo.type = eLine;
    einfo.tag = 1;
    einfo.eleOrder = order;
    ElementInfo priminfo;
    priminfo.type = eNode;
    for (int i = 1; i != order+2; ++i) {
        priminfo.tag = i;
        einfo.prims.push_back(fac->GetElement(priminfo));
    }
    fac->OrderElement(einfo);
    fac->PlaceOrder();

    Element* line = fac->GetElement(einfo);

    StdToPhysMapping *lineMap = new LineLGMapping{line};
    std::vector<double> map_pts = Legendre(6).GetZeros();

    std::vector<double> result {-9.32469514, -6.61209386, -2.38619186,  
                                2.38619186,  6.61209386, 9.32469514};

    for (int i = 0; i != map_pts.size(); ++i) {
        REQUIRE(lineMap->EvaluateFor(line, map_pts[i])[0] == Approx(result[i]));
    }

}