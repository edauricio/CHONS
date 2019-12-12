#include "catch2/include/catch.hpp"
#include "MeshModule/MeshReader.h"
#include "MeshModule/MeshInfoHolder.h"
#include "MathModule/Mappings.h"
#include "MeshModule/ElementComposite.h"
#include "MeshModule/ElementFactory.h"
#include <iostream>
#include <vector>

using namespace CHONS;

TEST_CASE("Lagrangian Quad mapping test 1", "[quadlgmap1]") {
    GmshReader reader("2d_lagrangequad_o2.msh");
    reader.ReadNodes();
    reader.ReadEdges();
    reader.ReadElements();

    ElementFactory* fac = ElementFactory::GetInstance();

    // MeshInfoHolder* meshinfo = MeshInfoHolder::GetInstance();

    ElementInfo einfo;
    einfo.tag = 63;
    einfo.type = eQuad;

    Element* quad = fac->GetElement(einfo);

    StdToPhysMapping* quadmap = new QuadLGMapping{quad};

    std::vector<double> map_pts_x = Legendre(6).GetZeros();
    std::vector<double> map_pts_y = Legendre(6).GetZeros();

    std::vector<double> result_x{
        -0.31082317, -0.22040313, -0.07953973,  0.07953973,  0.22040313,
        0.31082317, -0.31082317, -0.22040313, -0.07953973,  0.07953973,
        0.22040313,  0.31082317, -0.31082317, -0.22040313, -0.07953973,
        0.07953973,  0.22040313,  0.31082317, -0.31082317, -0.22040313,
       -0.07953973,  0.07953973,  0.22040313,  0.31082317, -0.31082317,
       -0.22040313, -0.07953973,  0.07953973,  0.22040313,  0.31082317,
       -0.31082317, -0.22040313, -0.07953973,  0.07953973,  0.22040313,
        0.31082317
                                };

    std::vector<double> result_y{
        -0.31082317, -0.31082317, -0.31082317, -0.31082317, -0.31082317,
       -0.31082317, -0.22040313, -0.22040313, -0.22040313, -0.22040313,
       -0.22040313, -0.22040313, -0.07953973, -0.07953973, -0.07953973,
       -0.07953973, -0.07953973, -0.07953973,  0.07953973,  0.07953973,
        0.07953973,  0.07953973,  0.07953973,  0.07953973,  0.22040313,
        0.22040313,  0.22040313,  0.22040313,  0.22040313,  0.22040313,
        0.31082317,  0.31082317,  0.31082317,  0.31082317,  0.31082317,
        0.31082317
                            };

    for (int j = 0; j != map_pts_y.size(); ++j) {
        for (int i = 0; i != map_pts_y.size(); ++i) {
            REQUIRE(quadmap->EvaluateFor(quad, map_pts_x[i], map_pts_y[j])[0]
                == Approx(result_x[j*(map_pts_x.size()) + i]));
            REQUIRE(quadmap->EvaluateFor(quad, map_pts_x[i], map_pts_y[j])[1]
                == Approx(result_y[j*(map_pts_y.size()) + i]));
        }
    }    
}

TEST_CASE("Lagrangian Quad mapping test 2", "[quadlgmap2]") {
    GmshReader reader("2d_lagrangequad_o3.msh");
    reader.ReadNodes();
    reader.ReadEdges();
    reader.ReadElements();

    ElementFactory* fac = ElementFactory::GetInstance();

    // MeshInfoHolder* meshinfo = MeshInfoHolder::GetInstance();

    ElementInfo einfo;
    einfo.tag = 84;
    einfo.type = eQuad;

    Element* quad = fac->GetElement(einfo);

    StdToPhysMapping* quadmap = new QuadLGMapping{quad};

    std::vector<double> map_pts_x = Legendre(6).GetZeros();
    std::vector<double> map_pts_y = Legendre(6).GetZeros();

    std::vector<double> result_x{
        -0.31082317, -0.22040313, -0.07953973,  0.07953973,  0.22040313,
        0.31082317, -0.31082317, -0.22040313, -0.07953973,  0.07953973,
        0.22040313,  0.31082317, -0.31082317, -0.22040313, -0.07953973,
        0.07953973,  0.22040313,  0.31082317, -0.31082317, -0.22040313,
       -0.07953973,  0.07953973,  0.22040313,  0.31082317, -0.31082317,
       -0.22040313, -0.07953973,  0.07953973,  0.22040313,  0.31082317,
       -0.31082317, -0.22040313, -0.07953973,  0.07953973,  0.22040313,
        0.31082317
                                };

    std::vector<double> result_y{
        -0.31082317, -0.31082317, -0.31082317, -0.31082317, -0.31082317,
       -0.31082317, -0.22040313, -0.22040313, -0.22040313, -0.22040313,
       -0.22040313, -0.22040313, -0.07953973, -0.07953973, -0.07953973,
       -0.07953973, -0.07953973, -0.07953973,  0.07953973,  0.07953973,
        0.07953973,  0.07953973,  0.07953973,  0.07953973,  0.22040313,
        0.22040313,  0.22040313,  0.22040313,  0.22040313,  0.22040313,
        0.31082317,  0.31082317,  0.31082317,  0.31082317,  0.31082317,
        0.31082317
                            };

    for (int j = 0; j != map_pts_y.size(); ++j) {
        for (int i = 0; i != map_pts_y.size(); ++i) {
            REQUIRE(quadmap->EvaluateFor(quad, map_pts_x[i], map_pts_y[j])[0]
                == Approx(result_x[j*(map_pts_x.size()) + i]));
            REQUIRE(quadmap->EvaluateFor(quad, map_pts_x[i], map_pts_y[j])[1]
                == Approx(result_y[j*(map_pts_y.size()) + i]));
        }
    }    
}

/* Mapped coordinates for curved element
    x = -10, 0, 10 ...
    y = -10, -7.5, -10; 0, 2.5, 0; 10, 12.5, 10

    std::vector<double> result_x{
        -9.32469514, -6.61209386, -2.38619186,  2.38619186,  6.61209386,
        9.32469514, -9.32469514, -6.61209386, -2.38619186,  2.38619186,
        6.61209386,  9.32469514, -9.32469514, -6.61209386, -2.38619186,
        2.38619186,  6.61209386,  9.32469514, -9.32469514, -6.61209386,
       -2.38619186,  2.38619186,  6.61209386,  9.32469514, -9.32469514,
       -6.61209386, -2.38619186,  2.38619186,  6.61209386,  9.32469514,
       -9.32469514, -6.61209386, -2.38619186,  2.38619186,  6.61209386,
        9.32469514
                            };

    std::vector<double> result_y{
        -8.99844363, -7.91768977, -6.96704293, -6.96704293, -7.91768977,
       -8.99844363, -6.28584235, -5.2050885 , -4.25444165, -4.25444165,
       -5.2050885 , -6.28584235, -2.05994035, -0.97918649, -0.02853965,
       -0.02853965, -0.97918649, -2.05994035,  2.71244337,  3.79319723,
        4.74384407,  4.74384407,  3.79319723,  2.71244337,  6.93834538,
        8.01909923,  8.96974607,  8.96974607,  8.01909923,  6.93834538,
        9.65094665, 10.73170051, 11.68234735, 11.68234735, 10.73170051,
        9.65094665
                            };*/