#include "catch2/include/catch.hpp"
#include "MeshModule/MeshReader.h"
#include "MeshModule/ElementEnumInfo.h"

using namespace CHONS;

TEST_CASE("Read nodes from 2nd order line test", "[nodesflineo2]") {
    GmshReader mshReader("1d_lagrangeline_o2.msh");
    mshReader.ReadNodes();
    mshReader.ReadEdges();
    ElementFactory* fac = ElementFactory::GetInstance();

    ElementInfo einfo;
    einfo.type = eLine;
    einfo.tag = 6;

    Element* edge = fac->GetElement(einfo);

    REQUIRE(edge->GetPrimitives()[0]->GetCoords()[0] == Approx(-10.0));
    REQUIRE(edge->GetPrimitives()[1]->GetCoords()[0] == Approx(10.0));
    REQUIRE(std::abs(edge->GetPrimitives()[2]->GetCoords()[0]) < 1.e-6);

}

TEST_CASE("Read nodes from 4th order line test", "[nodesflineo4]") {
    GmshReader mshReader("1d_lagrangeline_o4.msh");
    mshReader.ReadNodes();
    mshReader.ReadEdges();
    ElementFactory* fac = ElementFactory::GetInstance();

    ElementInfo einfo;
    einfo.type = eLine;
    einfo.tag = 7;

    Element* edge = fac->GetElement(einfo);

    REQUIRE(edge->GetPrimitives()[0]->GetCoords()[0] == Approx(-10.0));
    REQUIRE(edge->GetPrimitives()[1]->GetCoords()[0] == Approx(10.0));
    
    double dx = 20.0 / edge->GetElementOrder();
    for (int i = 2; i != edge->GetElementOrder()+1; ++i)
        REQUIRE((edge->GetPrimitives()[i]->GetCoords()[0] - (-10.0 + (i-1)*dx))
                < 1.e-6);
}

TEST_CASE("Read nodes from 5th order line test", "[nodesflineo5]") {
    GmshReader mshReader("1d_lagrangeline_o5.msh");
    mshReader.ReadNodes();
    mshReader.ReadEdges();
    ElementFactory* fac = ElementFactory::GetInstance();

    ElementInfo einfo;
    einfo.type = eLine;
    einfo.tag = 9;

    Element* edge = fac->GetElement(einfo);

    REQUIRE(edge->GetPrimitives()[0]->GetCoords()[0] == Approx(-10.0));
    REQUIRE(edge->GetPrimitives()[1]->GetCoords()[0] == Approx(10.0));
    
    double dx = 20.0 / edge->GetElementOrder();
    for (int i = 2; i != edge->GetElementOrder()+1; ++i)
        REQUIRE((edge->GetPrimitives()[i]->GetCoords()[0] - (-10.0 + (i-1)*dx))
                < 1.e-6);
}