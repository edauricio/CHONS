#include "MeshModule/ElementComposite.h"
#include "MeshModule/ElementFactory.h"
#include "MeshModule/MeshReader.h"
#include <iostream>

using namespace CHONS;

int main() {
    GmshReader readertest{"/home/eron/CHONS/testing/mesh/3d_cubeo1.msh"};
    std::cout << "Trying to read Nodes.\n";
    readertest.ReadNodes();
    //readertest.ShoutOutAll();
    std::cout << "Trying to read Edges.\n";
    readertest.ReadEdges();
    //readertest.ShoutOutAll();
    std::cout << "Trying to read Elements.\n";
    readertest.ReadElements();
    //readertest.ShoutOutAll();

    ElementFactory* fac = ElementFactory::GetInstance();

    std::cout << "Number of Nodes read: " << fac->HowMany(eNode) << "\n";
    std::cout << "Number of Edges read: " << fac->HowMany(eLine) << "\n";
    std::cout << "Number of Quads read: " << fac->HowMany(eQuad) << "\n";
    std::cout << "Number of Hexas read: " << fac->HowMany(eHexa) << "\n";

    // fac->RunThrough();

    // std::cin.get();
    
}