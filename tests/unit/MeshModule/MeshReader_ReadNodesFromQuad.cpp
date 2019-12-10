#include "catch2/include/catch.hpp"
#include "MeshModule/MeshReader.h"
#include "MeshModule/ElementEnumInfo.h"

using namespace CHONS;

TEST_CASE("Read nodes from 2nd order quad test", "[nodesfquado2]") {
    GmshReader mshReader("2d_lagrangequad_o2.msh");
    mshReader.ReadNodes();
    mshReader.ReadEdges();
    mshReader.ReadElements();

    ElementFactory* fac = ElementFactory::GetInstance();

    ElementInfo einfo;
    einfo.type = eQuad;
    

    // Bottom-left element
    einfo.tag = 59;
    Element* quad = fac->GetElement(einfo);

    
    REQUIRE(quad->GetPrimitives()[0]->GetPrimitives()[0]->GetCoords()[0]
            == Approx(-1.0));
    REQUIRE(quad->GetPrimitives()[0]->GetPrimitives()[0]->GetCoords()[1]
            == Approx(-1.0));

    REQUIRE(quad->GetPrimitives()[0]->GetPrimitives()[1]->GetCoords()[0]
            == Approx(-0.3333333333));
    REQUIRE(quad->GetPrimitives()[0]->GetPrimitives()[1]->GetCoords()[1]
            == Approx(-1.0));

    REQUIRE(quad->GetPrimitives()[2]->GetPrimitives()[0]->GetCoords()[0]
            == Approx(-0.3333333));
    REQUIRE(quad->GetPrimitives()[2]->GetPrimitives()[0]->GetCoords()[1]
            == Approx(-0.3333333));

    REQUIRE(quad->GetPrimitives()[2]->GetPrimitives()[1]->GetCoords()[0]
            == Approx(-1.0));
    REQUIRE(quad->GetPrimitives()[2]->GetPrimitives()[1]->GetCoords()[1]
            == Approx(-0.3333333));

    double dx = (quad->GetPrimitives()[0]->GetPrimitives()[1]->GetCoords()[0]
                - quad->GetPrimitives()[0]->GetPrimitives()[0]->GetCoords()[0])
                / quad->GetElementOrder();

    // High order nodes
    // Bottom edge
    for (int i = 2; i != quad->GetElementOrder()+1; ++i)
        REQUIRE(quad->GetPrimitives()[0]->GetPrimitives()[i]->GetCoords()[0]
                == Approx(
                    quad->GetPrimitives()[0]->GetPrimitives()[0]->GetCoords()[0]
                    + (i-1)*dx));

    for (int i = 2; i != quad->GetElementOrder()+1; ++i)
        REQUIRE(quad->GetPrimitives()[0]->GetPrimitives()[i]->GetCoords()[1]
        == Approx(quad->GetPrimitives()[0]->GetPrimitives()[0]->GetCoords()[1]));

    // Right edge
    for (int i = 2; i != quad->GetElementOrder()+1; ++i)
        REQUIRE(quad->GetPrimitives()[1]->GetPrimitives()[i]->GetCoords()[1]
            == Approx(quad->GetPrimitives()[1]->GetPrimitives()[0]->GetCoords()[1]
                + (i-1)*dx));

    for (int i = 2; i != quad->GetElementOrder()+1; ++i)
        REQUIRE(quad->GetPrimitives()[1]->GetPrimitives()[i]->GetCoords()[0]
        == Approx(quad->GetPrimitives()[1]->GetPrimitives()[0]->GetCoords()[0]));

    // Top edge
    for (int i = 2; i != quad->GetElementOrder()+1; ++i)
        REQUIRE(quad->GetPrimitives()[2]->GetPrimitives()[i]->GetCoords()[0]
            == Approx(quad->GetPrimitives()[2]->GetPrimitives()[0]->GetCoords()[0]
                - (i-1)*dx));

    for (int i = 2; i != quad->GetElementOrder()+1; ++i)
        REQUIRE(quad->GetPrimitives()[2]->GetPrimitives()[i]->GetCoords()[1]
            == Approx(quad->GetPrimitives()[2]->GetPrimitives()[0]->GetCoords()[1]));

    // Left edge
    for (int i = 2; i != quad->GetElementOrder()+1; ++i)
        REQUIRE(quad->GetPrimitives()[3]->GetPrimitives()[i]->GetCoords()[1]
            == Approx(quad->GetPrimitives()[3]->GetPrimitives()[0]->GetCoords()[1]
                - (i-1)*dx));

    for (int i = 2; i != quad->GetElementOrder()+1; ++i)
        REQUIRE(quad->GetPrimitives()[3]->GetPrimitives()[i]->GetCoords()[0]
            == Approx(quad->GetPrimitives()[3]->GetPrimitives()[0]->GetCoords()[0]));


    //=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=--=-=-=-=-=-=-=-=-

    // Top-right element
    einfo.tag = 67;
    quad = fac->GetElement(einfo);

    REQUIRE(quad->GetPrimitives()[0]->GetPrimitives()[0]->GetCoords()[0]
            == Approx(0.3333333333));
    REQUIRE(quad->GetPrimitives()[0]->GetPrimitives()[0]->GetCoords()[1]
            == Approx(0.3333333333));

    REQUIRE(quad->GetPrimitives()[0]->GetPrimitives()[1]->GetCoords()[0]
            == Approx(1.0));
    REQUIRE(quad->GetPrimitives()[0]->GetPrimitives()[1]->GetCoords()[1]
            == Approx(0.3333333333));

    REQUIRE(quad->GetPrimitives()[2]->GetPrimitives()[0]->GetCoords()[0]
            == Approx(1.0));
    REQUIRE(quad->GetPrimitives()[2]->GetPrimitives()[0]->GetCoords()[1]
            == Approx(1.0));

    REQUIRE(quad->GetPrimitives()[2]->GetPrimitives()[1]->GetCoords()[0]
            == Approx(0.3333333333));
    REQUIRE(quad->GetPrimitives()[2]->GetPrimitives()[1]->GetCoords()[1]
            == Approx(1.0));

    dx = (quad->GetPrimitives()[0]->GetPrimitives()[1]->GetCoords()[0]
                - quad->GetPrimitives()[0]->GetPrimitives()[0]->GetCoords()[0])
                / quad->GetElementOrder();

    // High order nodes
    // Bottom edge
    for (int i = 2; i != quad->GetElementOrder()+1; ++i)
        REQUIRE(quad->GetPrimitives()[0]->GetPrimitives()[i]->GetCoords()[0]
                == Approx(
                    quad->GetPrimitives()[0]->GetPrimitives()[0]->GetCoords()[0]
                    + (i-1)*dx));

    for (int i = 2; i != quad->GetElementOrder()+1; ++i)
        REQUIRE(quad->GetPrimitives()[0]->GetPrimitives()[i]->GetCoords()[1]
        == Approx(quad->GetPrimitives()[0]->GetPrimitives()[0]->GetCoords()[1]));

    // Right edge
    for (int i = 2; i != quad->GetElementOrder()+1; ++i)
        REQUIRE(quad->GetPrimitives()[1]->GetPrimitives()[i]->GetCoords()[1]
            == Approx(quad->GetPrimitives()[1]->GetPrimitives()[0]->GetCoords()[1]
                + (i-1)*dx));

    for (int i = 2; i != quad->GetElementOrder()+1; ++i)
        REQUIRE(quad->GetPrimitives()[1]->GetPrimitives()[i]->GetCoords()[0]
        == Approx(quad->GetPrimitives()[1]->GetPrimitives()[0]->GetCoords()[0]));

    // Top edge
    for (int i = 2; i != quad->GetElementOrder()+1; ++i)
        REQUIRE(quad->GetPrimitives()[2]->GetPrimitives()[i]->GetCoords()[0]
            == Approx(quad->GetPrimitives()[2]->GetPrimitives()[0]->GetCoords()[0]
                - (i-1)*dx));

    for (int i = 2; i != quad->GetElementOrder()+1; ++i)
        REQUIRE(quad->GetPrimitives()[2]->GetPrimitives()[i]->GetCoords()[1]
            == Approx(quad->GetPrimitives()[2]->GetPrimitives()[0]->GetCoords()[1]));

    // Left edge
    for (int i = 2; i != quad->GetElementOrder()+1; ++i)
        REQUIRE(quad->GetPrimitives()[3]->GetPrimitives()[i]->GetCoords()[1]
            == Approx(quad->GetPrimitives()[3]->GetPrimitives()[0]->GetCoords()[1]
                - (i-1)*dx));

    for (int i = 2; i != quad->GetElementOrder()+1; ++i)
        REQUIRE(quad->GetPrimitives()[3]->GetPrimitives()[i]->GetCoords()[0]
            == Approx(quad->GetPrimitives()[3]->GetPrimitives()[0]->GetCoords()[0]));
}

TEST_CASE("Read nodes from 3rd order quad test", "[nodesfquado3]") {
    GmshReader mshReader("2d_lagrangequad_o3.msh");
    mshReader.ReadNodes();
    mshReader.ReadEdges();
    mshReader.ReadElements();

    ElementFactory* fac = ElementFactory::GetInstance();

    ElementInfo einfo;
    einfo.type = eQuad;


    // Bottom-left element
    einfo.tag = 80;
    Element* quad = fac->GetElement(einfo);

    
    REQUIRE(quad->GetPrimitives()[0]->GetPrimitives()[0]->GetCoords()[0]
            == Approx(-1.0));
    REQUIRE(quad->GetPrimitives()[0]->GetPrimitives()[0]->GetCoords()[1]
            == Approx(-1.0));

    REQUIRE(quad->GetPrimitives()[0]->GetPrimitives()[1]->GetCoords()[0]
            == Approx(-0.3333333333));
    REQUIRE(quad->GetPrimitives()[0]->GetPrimitives()[1]->GetCoords()[1]
            == Approx(-1.0));

    REQUIRE(quad->GetPrimitives()[2]->GetPrimitives()[0]->GetCoords()[0]
            == Approx(-0.3333333));
    REQUIRE(quad->GetPrimitives()[2]->GetPrimitives()[0]->GetCoords()[1]
            == Approx(-0.3333333));

    REQUIRE(quad->GetPrimitives()[2]->GetPrimitives()[1]->GetCoords()[0]
            == Approx(-1.0));
    REQUIRE(quad->GetPrimitives()[2]->GetPrimitives()[1]->GetCoords()[1]
            == Approx(-0.3333333));

    double dx = (quad->GetPrimitives()[0]->GetPrimitives()[1]->GetCoords()[0]
                - quad->GetPrimitives()[0]->GetPrimitives()[0]->GetCoords()[0])
                / quad->GetElementOrder();

    // High order nodes
    // Bottom edge
    for (int i = 2; i != quad->GetElementOrder()+1; ++i)
        REQUIRE(quad->GetPrimitives()[0]->GetPrimitives()[i]->GetCoords()[0]
                == Approx(
                    quad->GetPrimitives()[0]->GetPrimitives()[0]->GetCoords()[0]
                    + (i-1)*dx));

    for (int i = 2; i != quad->GetElementOrder()+1; ++i)
        REQUIRE(quad->GetPrimitives()[0]->GetPrimitives()[i]->GetCoords()[1]
        == Approx(quad->GetPrimitives()[0]->GetPrimitives()[0]->GetCoords()[1]));

    // Right edge
    for (int i = 2; i != quad->GetElementOrder()+1; ++i)
        REQUIRE(quad->GetPrimitives()[1]->GetPrimitives()[i]->GetCoords()[1]
            == Approx(quad->GetPrimitives()[1]->GetPrimitives()[0]->GetCoords()[1]
                + (i-1)*dx));

    for (int i = 2; i != quad->GetElementOrder()+1; ++i)
        REQUIRE(quad->GetPrimitives()[1]->GetPrimitives()[i]->GetCoords()[0]
        == Approx(quad->GetPrimitives()[1]->GetPrimitives()[0]->GetCoords()[0]));

    // Top edge
    for (int i = 2; i != quad->GetElementOrder()+1; ++i)
        REQUIRE(quad->GetPrimitives()[2]->GetPrimitives()[i]->GetCoords()[0]
            == Approx(quad->GetPrimitives()[2]->GetPrimitives()[0]->GetCoords()[0]
                - (i-1)*dx));

    for (int i = 2; i != quad->GetElementOrder()+1; ++i)
        REQUIRE(quad->GetPrimitives()[2]->GetPrimitives()[i]->GetCoords()[1]
            == Approx(quad->GetPrimitives()[2]->GetPrimitives()[0]->GetCoords()[1]));

    // Left edge
    for (int i = 2; i != quad->GetElementOrder()+1; ++i)
        REQUIRE(quad->GetPrimitives()[3]->GetPrimitives()[i]->GetCoords()[1]
            == Approx(quad->GetPrimitives()[3]->GetPrimitives()[0]->GetCoords()[1]
                - (i-1)*dx));

    for (int i = 2; i != quad->GetElementOrder()+1; ++i)
        REQUIRE(quad->GetPrimitives()[3]->GetPrimitives()[i]->GetCoords()[0]
            == Approx(quad->GetPrimitives()[3]->GetPrimitives()[0]->GetCoords()[0]));


    // Top-right element
    einfo.tag = 88;
    quad = fac->GetElement(einfo);

    
    REQUIRE(quad->GetPrimitives()[0]->GetPrimitives()[0]->GetCoords()[0]
            == Approx(0.33333333));
    REQUIRE(quad->GetPrimitives()[0]->GetPrimitives()[0]->GetCoords()[1]
            == Approx(0.33333333));

    REQUIRE(quad->GetPrimitives()[0]->GetPrimitives()[1]->GetCoords()[0]
            == Approx(1.0));
    REQUIRE(quad->GetPrimitives()[0]->GetPrimitives()[1]->GetCoords()[1]
            == Approx(0.33333333));

    REQUIRE(quad->GetPrimitives()[2]->GetPrimitives()[0]->GetCoords()[0]
            == Approx(1.0));
    REQUIRE(quad->GetPrimitives()[2]->GetPrimitives()[0]->GetCoords()[1]
            == Approx(1.0));

    REQUIRE(quad->GetPrimitives()[2]->GetPrimitives()[1]->GetCoords()[0]
            == Approx(0.33333333));
    REQUIRE(quad->GetPrimitives()[2]->GetPrimitives()[1]->GetCoords()[1]
            == Approx(1.0));

    dx = (quad->GetPrimitives()[0]->GetPrimitives()[1]->GetCoords()[0]
                - quad->GetPrimitives()[0]->GetPrimitives()[0]->GetCoords()[0])
                / quad->GetElementOrder();

    // High order nodes
    // Bottom edge
    for (int i = 2; i != quad->GetElementOrder()+1; ++i)
        REQUIRE(quad->GetPrimitives()[0]->GetPrimitives()[i]->GetCoords()[0]
                == Approx(
                    quad->GetPrimitives()[0]->GetPrimitives()[0]->GetCoords()[0]
                    + (i-1)*dx));

    for (int i = 2; i != quad->GetElementOrder()+1; ++i)
        REQUIRE(quad->GetPrimitives()[0]->GetPrimitives()[i]->GetCoords()[1]
        == Approx(quad->GetPrimitives()[0]->GetPrimitives()[0]->GetCoords()[1]));

    // Right edge
    for (int i = 2; i != quad->GetElementOrder()+1; ++i)
        REQUIRE(quad->GetPrimitives()[1]->GetPrimitives()[i]->GetCoords()[1]
            == Approx(quad->GetPrimitives()[1]->GetPrimitives()[0]->GetCoords()[1]
                + (i-1)*dx));

    for (int i = 2; i != quad->GetElementOrder()+1; ++i)
        REQUIRE(quad->GetPrimitives()[1]->GetPrimitives()[i]->GetCoords()[0]
        == Approx(quad->GetPrimitives()[1]->GetPrimitives()[0]->GetCoords()[0]));

    // Top edge
    for (int i = 2; i != quad->GetElementOrder()+1; ++i)
        REQUIRE(quad->GetPrimitives()[2]->GetPrimitives()[i]->GetCoords()[0]
            == Approx(quad->GetPrimitives()[2]->GetPrimitives()[0]->GetCoords()[0]
                - (i-1)*dx));

    for (int i = 2; i != quad->GetElementOrder()+1; ++i)
        REQUIRE(quad->GetPrimitives()[2]->GetPrimitives()[i]->GetCoords()[1]
            == Approx(quad->GetPrimitives()[2]->GetPrimitives()[0]->GetCoords()[1]));

    // Left edge
    for (int i = 2; i != quad->GetElementOrder()+1; ++i)
        REQUIRE(quad->GetPrimitives()[3]->GetPrimitives()[i]->GetCoords()[1]
            == Approx(quad->GetPrimitives()[3]->GetPrimitives()[0]->GetCoords()[1]
                - (i-1)*dx));

    for (int i = 2; i != quad->GetElementOrder()+1; ++i)
        REQUIRE(quad->GetPrimitives()[3]->GetPrimitives()[i]->GetCoords()[0]
            == Approx(quad->GetPrimitives()[3]->GetPrimitives()[0]->GetCoords()[0]));
}