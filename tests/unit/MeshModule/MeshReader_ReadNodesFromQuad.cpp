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

    
    REQUIRE(quad->GetNodes()[0]->GetCoords()[0] == Approx(-1.0));
    REQUIRE(quad->GetNodes()[0]->GetCoords()[1] == Approx(-1.0));

    REQUIRE(quad->GetNodes()[1]->GetCoords()[0] == Approx(-0.3333333333));
    REQUIRE(quad->GetNodes()[1]->GetCoords()[1] == Approx(-1.0));

    REQUIRE(quad->GetNodes()[2]->GetCoords()[0] == Approx(-0.33333333));
    REQUIRE(quad->GetNodes()[2]->GetCoords()[0] == Approx(-0.33333333));

    REQUIRE(quad->GetNodes()[3]->GetCoords()[0] == Approx(-1.0));
    REQUIRE(quad->GetNodes()[3]->GetCoords()[1] == Approx(-0.33333333));

    double dx = (quad->GetNodes()[1]->GetCoords()[0]
                - quad->GetNodes()[0]->GetCoords()[0])
                / quad->GetElementOrder();

    // High order nodes
    int ind = 4;
    // Bottom edge
    for (int i = 0; i != quad->GetElementOrder()-1; ++i, ind++) {
        REQUIRE(quad->GetNodes()[ind]->GetCoords()[0]
                == Approx(quad->GetNodes()[0]->GetCoords()[0] + (i+1)*dx));

        REQUIRE(quad->GetNodes()[ind]->GetCoords()[1]
                == Approx(quad->GetNodes()[0]->GetCoords()[1]));

    }        

    // Right edge
    for (int i = 0; i != quad->GetElementOrder()-1; ++i, ind++) {
        REQUIRE(quad->GetNodes()[ind]->GetCoords()[1]
            == Approx(quad->GetNodes()[1]->GetCoords()[1] + (i+1)*dx));

        REQUIRE(quad->GetNodes()[ind]->GetCoords()[0]
            == Approx(quad->GetNodes()[1]->GetCoords()[0]));
    }


    // Top edge
    for (int i = 0; i != quad->GetElementOrder()-1; ++i, ind++) {
        REQUIRE(quad->GetNodes()[ind]->GetCoords()[0]
            == Approx(quad->GetNodes()[2]->GetCoords()[0] - (i+1)*dx));

        REQUIRE(quad->GetNodes()[ind]->GetCoords()[1]
            == Approx(quad->GetNodes()[2]->GetCoords()[1]));
    }

    // Left edge
    for (int i = 0; i != quad->GetElementOrder()-1; ++i, ind++) {
        REQUIRE(quad->GetNodes()[ind]->GetCoords()[1]
            == Approx(quad->GetNodes()[3]->GetCoords()[1] - (i+1)*dx));

        REQUIRE(quad->GetNodes()[ind]->GetCoords()[0]
            == Approx(quad->GetNodes()[3]->GetCoords()[0]));
    }

    // Single internal node
    REQUIRE(quad->GetNodes()[ind]->GetCoords()[0] == 
                    Approx(quad->GetNodes()[7]->GetCoords()[0] + dx));


    //=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=--=-=-=-=-=-=-=-=-

    // Top-right element
    einfo.tag = 67;
    quad = fac->GetElement(einfo);

    
    REQUIRE(quad->GetNodes()[0]->GetCoords()[0] == Approx(0.333333333));
    REQUIRE(quad->GetNodes()[0]->GetCoords()[1] == Approx(0.333333333));

    REQUIRE(quad->GetNodes()[1]->GetCoords()[0] == Approx(1.0));
    REQUIRE(quad->GetNodes()[1]->GetCoords()[1] == Approx(0.333333333));

    REQUIRE(quad->GetNodes()[2]->GetCoords()[0] == Approx(1.0));
    REQUIRE(quad->GetNodes()[2]->GetCoords()[0] == Approx(1.0));

    REQUIRE(quad->GetNodes()[3]->GetCoords()[0] == Approx(0.333333333));
    REQUIRE(quad->GetNodes()[3]->GetCoords()[1] == Approx(1.0));

    dx = (quad->GetNodes()[1]->GetCoords()[0]
                - quad->GetNodes()[0]->GetCoords()[0])
                / quad->GetElementOrder();

    // High order nodes
    ind = 4;
    // Bottom edge
    for (int i = 0; i != quad->GetElementOrder()-1; ++i, ind++) {
        REQUIRE(quad->GetNodes()[ind]->GetCoords()[0]
                == Approx(quad->GetNodes()[0]->GetCoords()[0] + (i+1)*dx));

        REQUIRE(quad->GetNodes()[ind]->GetCoords()[1]
                == Approx(quad->GetNodes()[0]->GetCoords()[1]));

    }        

    // Right edge
    for (int i = 0; i != quad->GetElementOrder()-1; ++i, ind++) {
        REQUIRE(quad->GetNodes()[ind]->GetCoords()[1]
            == Approx(quad->GetNodes()[1]->GetCoords()[1] + (i+1)*dx));

        REQUIRE(quad->GetNodes()[ind]->GetCoords()[0]
            == Approx(quad->GetNodes()[1]->GetCoords()[0]));
    }


    // Top edge
    for (int i = 0; i != quad->GetElementOrder()-1; ++i, ind++) {
        REQUIRE(quad->GetNodes()[ind]->GetCoords()[0]
            == Approx(quad->GetNodes()[2]->GetCoords()[0] - (i+1)*dx));

        REQUIRE(quad->GetNodes()[ind]->GetCoords()[1]
            == Approx(quad->GetNodes()[2]->GetCoords()[1]));
    }

    // Left edge
    for (int i = 0; i != quad->GetElementOrder()-1; ++i, ind++) {
        REQUIRE(quad->GetNodes()[ind]->GetCoords()[1]
            == Approx(quad->GetNodes()[3]->GetCoords()[1] - (i+1)*dx));

        REQUIRE(quad->GetNodes()[ind]->GetCoords()[0]
            == Approx(quad->GetNodes()[3]->GetCoords()[0]));
    }

    // Single internal node
    REQUIRE(quad->GetNodes()[ind]->GetCoords()[0] == 
                    Approx(quad->GetNodes()[7]->GetCoords()[0] + dx));
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

    
    REQUIRE(quad->GetNodes()[0]->GetCoords()[0] == Approx(-1.0));
    REQUIRE(quad->GetNodes()[0]->GetCoords()[1] == Approx(-1.0));

    REQUIRE(quad->GetNodes()[1]->GetCoords()[0] == Approx(-0.3333333333));
    REQUIRE(quad->GetNodes()[1]->GetCoords()[1] == Approx(-1.0));

    REQUIRE(quad->GetNodes()[2]->GetCoords()[0] == Approx(-0.33333333));
    REQUIRE(quad->GetNodes()[2]->GetCoords()[0] == Approx(-0.33333333));

    REQUIRE(quad->GetNodes()[3]->GetCoords()[0] == Approx(-1.0));
    REQUIRE(quad->GetNodes()[3]->GetCoords()[1] == Approx(-0.33333333));

    double dx = (quad->GetNodes()[1]->GetCoords()[0]
                - quad->GetNodes()[0]->GetCoords()[0])
                / quad->GetElementOrder();

    // High order, edge interior nodes
    int ind = 4;
    // Bottom edge
    for (int i = 0; i != quad->GetElementOrder()-1; ++i, ind++) {
        REQUIRE(quad->GetNodes()[ind]->GetCoords()[0]
                == Approx(quad->GetNodes()[0]->GetCoords()[0] + (i+1)*dx));

        REQUIRE(quad->GetNodes()[ind]->GetCoords()[1]
                == Approx(quad->GetNodes()[0]->GetCoords()[1]));

    }        

    // Right edge
    for (int i = 0; i != quad->GetElementOrder()-1; ++i, ind++) {
        REQUIRE(quad->GetNodes()[ind]->GetCoords()[1]
            == Approx(quad->GetNodes()[1]->GetCoords()[1] + (i+1)*dx));

        REQUIRE(quad->GetNodes()[ind]->GetCoords()[0]
            == Approx(quad->GetNodes()[1]->GetCoords()[0]));
    }


    // Top edge
    for (int i = 0; i != quad->GetElementOrder()-1; ++i, ind++) {
        REQUIRE(quad->GetNodes()[ind]->GetCoords()[0]
            == Approx(quad->GetNodes()[2]->GetCoords()[0] - (i+1)*dx));

        REQUIRE(quad->GetNodes()[ind]->GetCoords()[1]
            == Approx(quad->GetNodes()[2]->GetCoords()[1]));
    }

    // Left edge
    for (int i = 0; i != quad->GetElementOrder()-1; ++i, ind++) {
        REQUIRE(quad->GetNodes()[ind]->GetCoords()[1]
            == Approx(quad->GetNodes()[3]->GetCoords()[1] - (i+1)*dx));

        REQUIRE(quad->GetNodes()[ind]->GetCoords()[0]
            == Approx(quad->GetNodes()[3]->GetCoords()[0]));
    }

    // Inner quad vertices
    REQUIRE(quad->GetNodes()[ind]->GetCoords()[0] == Approx(-0.77777777));
    REQUIRE(quad->GetNodes()[ind++]->GetCoords()[1] == Approx(-0.77777777));

    REQUIRE(quad->GetNodes()[ind]->GetCoords()[0] == Approx(-0.5555555555));
    REQUIRE(quad->GetNodes()[ind++]->GetCoords()[1] == Approx(-0.77777777));

    REQUIRE(quad->GetNodes()[ind]->GetCoords()[0] == Approx(-0.5555555555));
    REQUIRE(quad->GetNodes()[ind++]->GetCoords()[0] == Approx(-0.5555555555));

    REQUIRE(quad->GetNodes()[ind]->GetCoords()[0] == Approx(-0.77777777));
    REQUIRE(quad->GetNodes()[ind++]->GetCoords()[1] == Approx(-0.5555555555));


    //=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=--=-=-=-=-=-=-=-=-

    // Top-right element
    einfo.tag = 88;
    quad = fac->GetElement(einfo);

    
    REQUIRE(quad->GetNodes()[0]->GetCoords()[0] == Approx(0.333333333));
    REQUIRE(quad->GetNodes()[0]->GetCoords()[1] == Approx(0.333333333));

    REQUIRE(quad->GetNodes()[1]->GetCoords()[0] == Approx(1.0));
    REQUIRE(quad->GetNodes()[1]->GetCoords()[1] == Approx(0.333333333));

    REQUIRE(quad->GetNodes()[2]->GetCoords()[0] == Approx(1.0));
    REQUIRE(quad->GetNodes()[2]->GetCoords()[0] == Approx(1.0));

    REQUIRE(quad->GetNodes()[3]->GetCoords()[0] == Approx(0.333333333));
    REQUIRE(quad->GetNodes()[3]->GetCoords()[1] == Approx(1.0));

    dx = (quad->GetNodes()[1]->GetCoords()[0]
                - quad->GetNodes()[0]->GetCoords()[0])
                / quad->GetElementOrder();

    // High order, edge interior nodes
    ind = 4;
    // Bottom edge
    for (int i = 0; i != quad->GetElementOrder()-1; ++i, ind++) {
        REQUIRE(quad->GetNodes()[ind]->GetCoords()[0]
                == Approx(quad->GetNodes()[0]->GetCoords()[0] + (i+1)*dx));

        REQUIRE(quad->GetNodes()[ind]->GetCoords()[1]
                == Approx(quad->GetNodes()[0]->GetCoords()[1]));

    }        

    // Right edge
    for (int i = 0; i != quad->GetElementOrder()-1; ++i, ind++) {
        REQUIRE(quad->GetNodes()[ind]->GetCoords()[1]
            == Approx(quad->GetNodes()[1]->GetCoords()[1] + (i+1)*dx));

        REQUIRE(quad->GetNodes()[ind]->GetCoords()[0]
            == Approx(quad->GetNodes()[1]->GetCoords()[0]));
    }


    // Top edge
    for (int i = 0; i != quad->GetElementOrder()-1; ++i, ind++) {
        REQUIRE(quad->GetNodes()[ind]->GetCoords()[0]
            == Approx(quad->GetNodes()[2]->GetCoords()[0] - (i+1)*dx));

        REQUIRE(quad->GetNodes()[ind]->GetCoords()[1]
            == Approx(quad->GetNodes()[2]->GetCoords()[1]));
    }

    // Left edge
    for (int i = 0; i != quad->GetElementOrder()-1; ++i, ind++) {
        REQUIRE(quad->GetNodes()[ind]->GetCoords()[1]
            == Approx(quad->GetNodes()[3]->GetCoords()[1] - (i+1)*dx));

        REQUIRE(quad->GetNodes()[ind]->GetCoords()[0]
            == Approx(quad->GetNodes()[3]->GetCoords()[0]));
    }

    // Inner quad vertices
    REQUIRE(quad->GetNodes()[ind]->GetCoords()[0] == Approx(0.55555555));
    REQUIRE(quad->GetNodes()[ind++]->GetCoords()[1] == Approx(0.55555555));

    REQUIRE(quad->GetNodes()[ind]->GetCoords()[0] == Approx(0.777777777));
    REQUIRE(quad->GetNodes()[ind++]->GetCoords()[1] == Approx(0.55555555));

    REQUIRE(quad->GetNodes()[ind]->GetCoords()[0] == Approx(0.777777777));
    REQUIRE(quad->GetNodes()[ind++]->GetCoords()[0] == Approx(0.777777777));

    REQUIRE(quad->GetNodes()[ind]->GetCoords()[0] == Approx(0.55555555));
    REQUIRE(quad->GetNodes()[ind++]->GetCoords()[1] == Approx(0.777777777));
}