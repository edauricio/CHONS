#include "catch2/include/catch.hpp"
#include "MeshModule/MeshReader.h"
#include "MeshModule/MeshInfoHolder.h"
#include "MathModule/Mappings.h"
#include "MeshModule/ElementComposite.h"
#include "MeshModule/ElementFactory.h"
#include <iostream>
#include <vector>

using namespace CHONS;

TEST_CASE("Lagrangian Quad mapping test", "[quadlgmap]") {
    GmshReader reader("2d_lagrangequad_o2.msh");
    reader.ReadNodes();
    reader.ReadEdges();
    reader.ReadElements();

    ElementFactory* fac = ElementFactory::GetInstance();

    MeshInfoHolder* meshinfo = MeshInfoHolder::GetInstance();

    ElementInfo einfo;
    einfo.tag = 59;
    einfo.type = eQuad;

    Element* quad = fac->GetElement(einfo);
    for (int i = 0; i != 4; ++i) {
        Element* edge = quad->GetPrimitives()[i];
        std::cout << "edge " << i << "\n";
        for (int j = 0; j != edge->GetPrimitives().size(); ++j)
            std::cout << "node " << j << ": " << edge->GetPrimitives()[j]->GetCoords()[0] << " " 
                    << edge->GetPrimitives()[j]->GetCoords()[1] << "\n";
    }
    
    REQUIRE(quad->GetElementOrder() == 2);

    // This won't work, because the mesh reader is bugged; node ordering is as
    // expected (lagragian ordering), but node coordinates are all wrong...
    // UNIT TEST MESH READING FIRST
}