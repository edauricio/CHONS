#include "catch2/include/catch.hpp"
#include "MeshModule/MeshReader.h"
#include "MeshModule/ElementEnumInfo.h"

using namespace CHONS;

std::vector<std::vector<int>> hexa_intEdge_map{
                        {1, 5, 3, 0},
                        {0, 4, 8, 2},
                        {2, 9, 7, 1},
                        {3, 6, 10, 4},
                        {5, 7, 11, 6},
                        {8, 10, 11, 9}};


TEST_CASE("Read nodes from 1st order hexa", "[nodesfhexao1]") {
    GmshReader mshReader("3d_lagrangehexa_o1.msh");
    mshReader.ReadNodes();
    mshReader.ReadEdges();
    mshReader.ReadElements();

    ElementFactory* fac = ElementFactory::GetInstance();

    ElementInfo einfo;
    einfo.type = eHexa;

    // Origin (-1,-1,0) element
    einfo.tag = 153;
    Element *hexa = fac->GetElement(einfo);

    REQUIRE(hexa->GetNodes()[0]->GetCoords()[0] == Approx(-1.0));
    REQUIRE(hexa->GetNodes()[0]->GetCoords()[1] == Approx(-1.0));
    REQUIRE(hexa->GetNodes()[0]->GetCoords()[2] < 1.e-6);

    REQUIRE(hexa->GetNodes()[1]->GetCoords()[0] == Approx(-0.5));
    REQUIRE(hexa->GetNodes()[1]->GetCoords()[1] == Approx(-1.0));
    REQUIRE(hexa->GetNodes()[1]->GetCoords()[2] < 1.e-6);

    REQUIRE(hexa->GetNodes()[2]->GetCoords()[0] == Approx(-0.5));
    REQUIRE(hexa->GetNodes()[2]->GetCoords()[1] == Approx(-0.5));
    REQUIRE(hexa->GetNodes()[2]->GetCoords()[2] < 1.e-6);

    REQUIRE(hexa->GetNodes()[3]->GetCoords()[0] == Approx(-1.0));
    REQUIRE(hexa->GetNodes()[3]->GetCoords()[1] == Approx(-0.5));
    REQUIRE(hexa->GetNodes()[3]->GetCoords()[2] < 1.e-6);

    REQUIRE(hexa->GetNodes()[4]->GetCoords()[0] == Approx(-1.0));
    REQUIRE(hexa->GetNodes()[4]->GetCoords()[1] == Approx(-1.0));
    REQUIRE(hexa->GetNodes()[4]->GetCoords()[2] == Approx(0.5));

    REQUIRE(hexa->GetNodes()[5]->GetCoords()[0] == Approx(-0.5));
    REQUIRE(hexa->GetNodes()[5]->GetCoords()[1] == Approx(-1.0));
    REQUIRE(hexa->GetNodes()[5]->GetCoords()[2] == Approx(0.5));

    REQUIRE(hexa->GetNodes()[6]->GetCoords()[0] == Approx(-0.5));
    REQUIRE(hexa->GetNodes()[6]->GetCoords()[1] == Approx(-0.5));
    REQUIRE(hexa->GetNodes()[6]->GetCoords()[2] == Approx(0.5));

    REQUIRE(hexa->GetNodes()[7]->GetCoords()[0] == Approx(-1.0));
    REQUIRE(hexa->GetNodes()[7]->GetCoords()[1] == Approx(-0.5));
    REQUIRE(hexa->GetNodes()[7]->GetCoords()[2] == Approx(0.5));
}

TEST_CASE("Read nodes from 2nd order hexa", "[nodesfhexao2]") {
    GmshReader mshReader("3d_lagrangehexa_o2.msh");
    mshReader.ReadNodes();
    mshReader.ReadEdges();
    mshReader.ReadElements();

    ElementFactory* fac = ElementFactory::GetInstance();

    ElementInfo einfo;
    einfo.type = eHexa;

    // Origin (-1,-1,0) element
    einfo.tag = 113;
    Element *hexa = fac->GetElement(einfo);

    REQUIRE(hexa->GetNodes()[0]->GetCoords()[0] == Approx(-1.0));
    REQUIRE(hexa->GetNodes()[0]->GetCoords()[1] == Approx(-1.0));
    REQUIRE(hexa->GetNodes()[0]->GetCoords()[2] < 1.e-6);

    REQUIRE(hexa->GetNodes()[1]->GetCoords()[0] < 1.e-6);
    REQUIRE(hexa->GetNodes()[1]->GetCoords()[1] == Approx(-1.0));
    REQUIRE(hexa->GetNodes()[1]->GetCoords()[2] < 1.e-6);

    REQUIRE(hexa->GetNodes()[2]->GetCoords()[0] < 1.e-6);
    REQUIRE(hexa->GetNodes()[2]->GetCoords()[1] < 1.e-6);
    REQUIRE(hexa->GetNodes()[2]->GetCoords()[2] < 1.e-6);

    REQUIRE(hexa->GetNodes()[3]->GetCoords()[0] == Approx(-1.0));
    REQUIRE(hexa->GetNodes()[3]->GetCoords()[1] < 1.e-6);
    REQUIRE(hexa->GetNodes()[3]->GetCoords()[2] < 1.e-6);

    REQUIRE(hexa->GetNodes()[4]->GetCoords()[0] == Approx(-1.0));
    REQUIRE(hexa->GetNodes()[4]->GetCoords()[1] == Approx(-1.0));
    REQUIRE(hexa->GetNodes()[4]->GetCoords()[2] == Approx(1.0));

    REQUIRE(hexa->GetNodes()[5]->GetCoords()[0] < 1.e-6);
    REQUIRE(hexa->GetNodes()[5]->GetCoords()[1] == Approx(-1.0));
    REQUIRE(hexa->GetNodes()[5]->GetCoords()[2] == Approx(1.0));

    REQUIRE(hexa->GetNodes()[6]->GetCoords()[0] < 1.e-6);
    REQUIRE(hexa->GetNodes()[6]->GetCoords()[1] < 1.e-6);
    REQUIRE(hexa->GetNodes()[6]->GetCoords()[2] == Approx(1.0));

    REQUIRE(hexa->GetNodes()[7]->GetCoords()[0] == Approx(-1.0));
    REQUIRE(hexa->GetNodes()[7]->GetCoords()[1] < 1.e-6);
    REQUIRE(hexa->GetNodes()[7]->GetCoords()[2] == Approx(1.0));

    // High order nodes
    
    double dx = (hexa->GetNodes()[1]->GetCoords()[0]
                    - hexa->GetNodes()[0]->GetCoords()[0])
                / hexa->GetElementOrder();

    // First face, first edge
    int ind = 8 + (hexa->GetElementOrder()-1)*hexa_intEdge_map[0][0];
    for (int i = 0; i != hexa->GetElementOrder()-1; ++i, ind++) {
        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[1] 
                == Approx(hexa->GetNodes()[0]->GetCoords()[1] + (i+1)*dx));

        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[0] 
                == Approx(hexa->GetNodes()[0]->GetCoords()[0]));

        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[2] 
                == Approx(hexa->GetNodes()[0]->GetCoords()[2]));
    }

    // First face, second edge
    ind = 8 + (hexa->GetElementOrder()-1)*hexa_intEdge_map[0][1]
            + hexa->GetElementOrder()-2;
    for (int i = 0; i != hexa->GetElementOrder()-1; ++i, ind--) {
        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[0]
                == Approx(hexa->GetNodes()[3]->GetCoords()[0] + (i+1)*dx));

        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[1] < 1.e-6);

        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[2] 
                == Approx(hexa->GetNodes()[3]->GetCoords()[2]));
    }

    // First face, third edge
    ind = 8 + (hexa->GetElementOrder()-1)*hexa_intEdge_map[0][2]
            + hexa->GetElementOrder()-2;
    for (int i = 0; i != hexa->GetElementOrder()-1; ++i, ind--) {
        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[1]
                == Approx(hexa->GetNodes()[2]->GetCoords()[1] - (i+1)*dx));

        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[0] < 1.e-6);

        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[2]
                == Approx(hexa->GetNodes()[2]->GetCoords()[2]));
    }

    // First face, fourth edge
    ind = 8 + (hexa->GetElementOrder()-1)*hexa_intEdge_map[0][3]
            + hexa->GetElementOrder()-2;
    for (int i = 0; i != hexa->GetElementOrder()-1; ++i, ind--) {
        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[0]
                == Approx(hexa->GetNodes()[1]->GetCoords()[0] - (i+1)*dx));

        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[1]
                == Approx(hexa->GetNodes()[1]->GetCoords()[1]));

        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[2]
                == Approx(hexa->GetNodes()[1]->GetCoords()[2]));
    }

    // =-=-=-=-=
    // Sixth face, first edge
    ind = 8 + (hexa->GetElementOrder()-1)*hexa_intEdge_map[5][0];
    for (int i = 0; i != hexa->GetElementOrder()-1; ++i, ind++) {
        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[0]
                == Approx(hexa->GetNodes()[4]->GetCoords()[0] + (i+1)*dx));

        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[1]
                == Approx(hexa->GetNodes()[4]->GetCoords()[1]));

        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[2]
                == Approx(hexa->GetNodes()[4]->GetCoords()[2]));
    }

    // Sixth face, second edge
    ind = 8 + (hexa->GetElementOrder()-1)*hexa_intEdge_map[5][1];
    for (int i = 0; i != hexa->GetElementOrder()-1; ++i, ind++) {
        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[1]
                == Approx(hexa->GetNodes()[5]->GetCoords()[1] + (i+1)*dx));

        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[0] < 1.e-6);

        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[2]
                == Approx(hexa->GetNodes()[5]->GetCoords()[2]));
    }

    // Sixth face, third edge
    ind = 8 + (hexa->GetElementOrder()-1)*hexa_intEdge_map[5][2];
    for (int i = 0; i != hexa->GetElementOrder()-1; ++i, ind++) {
        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[0]
                == Approx(hexa->GetNodes()[6]->GetCoords()[0] - (i+1)*dx));

        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[1] < 1.e-6);

        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[2]
                == Approx(hexa->GetNodes()[6]->GetCoords()[2]));
    }

    // Sixth face, fourth edge
    ind = 8 + (hexa->GetElementOrder()-1)*hexa_intEdge_map[5][3];
    for (int i = 0; i != hexa->GetElementOrder()-1; ++i, ind++) {
        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[1]
                == Approx(hexa->GetNodes()[7]->GetCoords()[1] - (i+1)*dx));

        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[0] < 1.e-6);

        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[2]
                == Approx(hexa->GetNodes()[7]->GetCoords()[2]));
    }

    // =-=-=-=-= Left-over edges
    // Second face, second edge
    ind = 8 + (hexa->GetElementOrder()-1)*hexa_intEdge_map[1][1];
    for (int i = 0; i != hexa->GetElementOrder()-1; ++i, ind++) {
        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[2]
                == Approx(hexa->GetNodes()[1]->GetCoords()[2] + (i+1)*dx));

        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[0] < 1.e-6);

        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[1] 
            == Approx(hexa->GetNodes()[1]->GetCoords()[1]));
    }

    // Second face, fourth edge
    ind = 8 + (hexa->GetElementOrder()-1)*hexa_intEdge_map[1][3]
            + hexa->GetElementOrder()-2;
    for (int i = 0; i != hexa->GetElementOrder()-1; ++i, ind--) {
        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[2]
                == Approx(hexa->GetNodes()[4]->GetCoords()[2] - (i+1)*dx));

        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[0] 
            == Approx(hexa->GetNodes()[4]->GetCoords()[0]));

        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[1] 
            == Approx(hexa->GetNodes()[4]->GetCoords()[1]));
    }

    // Third face, third edge
    ind = 8 + (hexa->GetElementOrder()-1)*hexa_intEdge_map[2][2]
            + hexa->GetElementOrder()-2;
    for (int i = 0; i != hexa->GetElementOrder()-1; ++i, ind--) {
        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[2]
                == Approx(hexa->GetNodes()[7]->GetCoords()[2] - (i+1)*dx));

        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[0] 
            == Approx(hexa->GetNodes()[7]->GetCoords()[0]));

        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[1] 
            == Approx(hexa->GetNodes()[7]->GetCoords()[1]));
    }

    // Fourth face, second edge
    ind = 8 + (hexa->GetElementOrder()-1)*hexa_intEdge_map[3][1];
    for (int i = 0; i != hexa->GetElementOrder()-1; ++i, ind++) {
        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[2]
                == Approx(hexa->GetNodes()[2]->GetCoords()[2] + (i+1)*dx));

        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[0] < 1.e-6);

        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[1] < 1.e-6);
    }

}

TEST_CASE("Read nodes from 3rd order hexa", "[nodesfhexao3]") {
    GmshReader mshReader("3d_lagrangehexa_o3.msh");
    mshReader.ReadNodes();
    mshReader.ReadEdges();
    mshReader.ReadElements();

    ElementFactory* fac = ElementFactory::GetInstance();

    ElementInfo einfo;
    einfo.type = eHexa;

    // Origin (-1,-1,0) element
    einfo.tag = 217;
    Element *hexa = fac->GetElement(einfo);

    REQUIRE(hexa->GetNodes()[0]->GetCoords()[0] == Approx(-1.0));
    REQUIRE(hexa->GetNodes()[0]->GetCoords()[1] == Approx(-1.0));
    REQUIRE(hexa->GetNodes()[0]->GetCoords()[2] < 1.e-6);

    REQUIRE(hexa->GetNodes()[1]->GetCoords()[0] < 1.e-6);
    REQUIRE(hexa->GetNodes()[1]->GetCoords()[1] == Approx(-1.0));
    REQUIRE(hexa->GetNodes()[1]->GetCoords()[2] < 1.e-6);

    REQUIRE(hexa->GetNodes()[2]->GetCoords()[0] < 1.e-6);
    REQUIRE(hexa->GetNodes()[2]->GetCoords()[1] < 1.e-6);
    REQUIRE(hexa->GetNodes()[2]->GetCoords()[2] < 1.e-6);

    REQUIRE(hexa->GetNodes()[3]->GetCoords()[0] == Approx(-1.0));
    REQUIRE(hexa->GetNodes()[3]->GetCoords()[1] < 1.e-6);
    REQUIRE(hexa->GetNodes()[3]->GetCoords()[2] < 1.e-6);

    REQUIRE(hexa->GetNodes()[4]->GetCoords()[0] == Approx(-1.0));
    REQUIRE(hexa->GetNodes()[4]->GetCoords()[1] == Approx(-1.0));
    REQUIRE(hexa->GetNodes()[4]->GetCoords()[2] == Approx(1.0));

    REQUIRE(hexa->GetNodes()[5]->GetCoords()[0] < 1.e-6);
    REQUIRE(hexa->GetNodes()[5]->GetCoords()[1] == Approx(-1.0));
    REQUIRE(hexa->GetNodes()[5]->GetCoords()[2] == Approx(1.0));

    REQUIRE(hexa->GetNodes()[6]->GetCoords()[0] < 1.e-6);
    REQUIRE(hexa->GetNodes()[6]->GetCoords()[1] < 1.e-6);
    REQUIRE(hexa->GetNodes()[6]->GetCoords()[2] == Approx(1.0));

    REQUIRE(hexa->GetNodes()[7]->GetCoords()[0] == Approx(-1.0));
    REQUIRE(hexa->GetNodes()[7]->GetCoords()[1] < 1.e-6);
    REQUIRE(hexa->GetNodes()[7]->GetCoords()[2] == Approx(1.0));

    // High order nodes
    
    double dx = (hexa->GetNodes()[1]->GetCoords()[0]
                    - hexa->GetNodes()[0]->GetCoords()[0])
                / hexa->GetElementOrder();

    // First face, first edge
    int ind = 8 + (hexa->GetElementOrder()-1)*hexa_intEdge_map[0][0];
    for (int i = 0; i != hexa->GetElementOrder()-1; ++i, ind++) {
        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[1] 
                == Approx(hexa->GetNodes()[0]->GetCoords()[1] + (i+1)*dx));

        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[0] 
                == Approx(hexa->GetNodes()[0]->GetCoords()[0]));

        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[2] 
                == Approx(hexa->GetNodes()[0]->GetCoords()[2]));
    }

    // First face, second edge
    ind = 8 + (hexa->GetElementOrder()-1)*hexa_intEdge_map[0][1]
            + hexa->GetElementOrder()-2;
    for (int i = 0; i != hexa->GetElementOrder()-1; ++i, ind--) {
        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[0]
                == Approx(hexa->GetNodes()[3]->GetCoords()[0] + (i+1)*dx));

        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[1] < 1.e-6);

        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[2] 
                == Approx(hexa->GetNodes()[3]->GetCoords()[2]));
    }

    // First face, third edge
    ind = 8 + (hexa->GetElementOrder()-1)*hexa_intEdge_map[0][2]
            + hexa->GetElementOrder()-2;
    for (int i = 0; i != hexa->GetElementOrder()-1; ++i, ind--) {
        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[1]
                == Approx(hexa->GetNodes()[2]->GetCoords()[1] - (i+1)*dx));

        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[0] < 1.e-6);

        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[2]
                == Approx(hexa->GetNodes()[2]->GetCoords()[2]));
    }

    // First face, fourth edge
    ind = 8 + (hexa->GetElementOrder()-1)*hexa_intEdge_map[0][3]
            + hexa->GetElementOrder()-2;
    for (int i = 0; i != hexa->GetElementOrder()-1; ++i, ind--) {
        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[0]
                == Approx(hexa->GetNodes()[1]->GetCoords()[0] - (i+1)*dx));

        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[1]
                == Approx(hexa->GetNodes()[1]->GetCoords()[1]));

        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[2]
                == Approx(hexa->GetNodes()[1]->GetCoords()[2]));
    }

    // =-=-=-=-=
    // Sixth face, first edge
    ind = 8 + (hexa->GetElementOrder()-1)*hexa_intEdge_map[5][0];
    for (int i = 0; i != hexa->GetElementOrder()-1; ++i, ind++) {
        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[0]
                == Approx(hexa->GetNodes()[4]->GetCoords()[0] + (i+1)*dx));

        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[1]
                == Approx(hexa->GetNodes()[4]->GetCoords()[1]));

        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[2]
                == Approx(hexa->GetNodes()[4]->GetCoords()[2]));
    }

    // Sixth face, second edge
    ind = 8 + (hexa->GetElementOrder()-1)*hexa_intEdge_map[5][1];
    for (int i = 0; i != hexa->GetElementOrder()-1; ++i, ind++) {
        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[1]
                == Approx(hexa->GetNodes()[5]->GetCoords()[1] + (i+1)*dx));

        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[0] < 1.e-6);

        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[2]
                == Approx(hexa->GetNodes()[5]->GetCoords()[2]));
    }

    // Sixth face, third edge
    ind = 8 + (hexa->GetElementOrder()-1)*hexa_intEdge_map[5][2];
    for (int i = 0; i != hexa->GetElementOrder()-1; ++i, ind++) {
        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[0]
                == Approx(hexa->GetNodes()[6]->GetCoords()[0] - (i+1)*dx));

        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[1] < 1.e-6);

        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[2]
                == Approx(hexa->GetNodes()[6]->GetCoords()[2]));
    }

    // Sixth face, fourth edge
    ind = 8 + (hexa->GetElementOrder()-1)*hexa_intEdge_map[5][3];
    for (int i = 0; i != hexa->GetElementOrder()-1; ++i, ind++) {
        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[1]
                == Approx(hexa->GetNodes()[7]->GetCoords()[1] - (i+1)*dx));

        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[0] < 1.e-6);

        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[2]
                == Approx(hexa->GetNodes()[7]->GetCoords()[2]));
    }

    // =-=-=-=-= Left-over edges
    // Second face, second edge
    ind = 8 + (hexa->GetElementOrder()-1)*hexa_intEdge_map[1][1];
    for (int i = 0; i != hexa->GetElementOrder()-1; ++i, ind++) {
        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[2]
                == Approx(hexa->GetNodes()[1]->GetCoords()[2] + (i+1)*dx));

        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[0] < 1.e-6);

        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[1] 
            == Approx(hexa->GetNodes()[1]->GetCoords()[1]));
    }

    // Second face, fourth edge
    ind = 8 + (hexa->GetElementOrder()-1)*hexa_intEdge_map[1][3]
            + hexa->GetElementOrder()-2;
    for (int i = 0; i != hexa->GetElementOrder()-1; ++i, ind--) {
        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[2]
                == Approx(hexa->GetNodes()[4]->GetCoords()[2] - (i+1)*dx));

        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[0] 
            == Approx(hexa->GetNodes()[4]->GetCoords()[0]));

        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[1] 
            == Approx(hexa->GetNodes()[4]->GetCoords()[1]));
    }

    // Third face, third edge
    ind = 8 + (hexa->GetElementOrder()-1)*hexa_intEdge_map[2][2]
            + hexa->GetElementOrder()-2;
    for (int i = 0; i != hexa->GetElementOrder()-1; ++i, ind--) {
        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[2]
                == Approx(hexa->GetNodes()[7]->GetCoords()[2] - (i+1)*dx));

        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[0] 
            == Approx(hexa->GetNodes()[7]->GetCoords()[0]));

        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[1] 
            == Approx(hexa->GetNodes()[7]->GetCoords()[1]));
    }

    // Fourth face, second edge
    ind = 8 + (hexa->GetElementOrder()-1)*hexa_intEdge_map[3][1];
    for (int i = 0; i != hexa->GetElementOrder()-1; ++i, ind++) {
        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[2]
                == Approx(hexa->GetNodes()[2]->GetCoords()[2] + (i+1)*dx));

        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[0] < 1.e-6);

        REQUIRE(hexa->GetNodes()[ind]->GetCoords()[1] < 1.e-6);
    }

}