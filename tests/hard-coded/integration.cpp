#include "MeshModule/ElementComposite.h"
#include "MeshModule/ElementFactory.h"
#include "MeshModule/MeshReader.h"
#include <iostream>

using namespace CHONS;

int main() {
    ElementFactory* fac = ElementFactory::GetInstance();

    GmshReader readertest{"/home/eron/CHONS/testing/mesh/3d_cube_hexao2.msh"};
    std::cout << "Trying to read Nodes.\n";
    readertest.ReadNodes();
    std::cout << "Number of Nodes read: " << fac->HowMany(eNode) << "\n";
    //readertest.ShoutOutAll();
    std::cout << "Trying to read Edges.\n";
    readertest.ReadEdges();
    std::cout << "Number of Edges read: " << fac->HowMany(eLine) << "\n";
    //readertest.ShoutOutAll();
    std::cout << "Trying to read Elements.\n";
    readertest.ReadElements();
    std::cout << "Number of Tris read: " << fac->HowMany(eTri) << "\n";
    std::cout << "Number of Quads read: " << fac->HowMany(eQuad) << "\n";
    std::cout << "Number of Hexas read: " << fac->HowMany(eHexa) << "\n";
    //readertest.ShoutOutAll();


    

    //fac->RunThrough();

    // std::cin.get();
    
}