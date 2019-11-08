#include "MeshModule/ElementComposite.h"
#include "MeshModule/ElementFactory.h"
#include "MeshModule/MeshReader.h"
#include "MeshModule/MeshInfoHolder.h"
#include "gperftools/profiler.h"
#include "boost/assert.hpp"
#include <iostream>

using namespace CHONS;

int main(int argc, char* argv[]) {
    // if (argc < 4) { 
    //     std::cout << "Wrong number of arguments. Should be " << argv[0]
    //                 << " edges_in_x edges_in_z  edges_in_y \n";
    //     exit(-1);
    // }
    // int xedg, zedg, yedg, totaled, totalfc;
    // xedg = atoi(argv[1]);
    // zedg = atoi(argv[2]);
    // yedg = atoi(argv[3]);
    // std::cout << "Edges in x: " << xedg
    //             << "\nEdges in z: " << zedg
    //             << "\nEdges in y: " << yedg << "\n";

    // totaled = (xedg*(zedg+1) + zedg*(xedg+1))*(yedg+1) // top grid running through
    //         + yedg*(zedg+1)*(xedg+1); // lateral vertical edges running through

    // totalfc = xedg*zedg*(yedg+1) // top quads running through y
    //         + zedg*yedg*(xedg+1) // lateral quads running through x
    //         + xedg*yedg*(zedg+1); // back quads running through z
    // std::cout << "Should read " << totaled << " edges.\n"
    //             "and " << totalfc << " quads.\n\n";

    ProfilerStart("dump.txt");
    ElementFactory* fac = ElementFactory::GetInstance();
    MeshInfoHolder* minfo = MeshInfoHolder::GetInstance();

    GmshReader readertest{"/home/eron/CHONS/testing/mesh/2d_quad.msh"};
    // GmshReader readertest{"/home/eron/workspace/CHONS/testing/mesh/3d_cube_hexao2.msh"};
    // std::cout << "Trying to read Nodes.\n";
    readertest.ReadNodes();
    std::cout << "Number of Nodes read: " << fac->HowMany(eNode) << "\n";
    //readertest.ShoutOutAll();
    // std::cout << "Trying to read Edges.\n";
    readertest.ReadEdges();
    // BOOST_ASSERT_MSG(fac->HowMany(eLine) == totaled, "Number of edges read differs from the number of calculated edges.");
    std::cout << "Number of Edges read: " << fac->HowMany(eLine) << "\n";
    //readertest.ShoutOutAll();
    // std::cout << "Trying to read Elements.\n";
    readertest.ReadElements();
    // std::cout << "Number of Tris read: " << fac->HowMany(eTri) << "\n";
    std::cout << "Number of Quads read: " << fac->HowMany(eQuad) << "\n";
    std::cout << "Number of Hexas read: " << fac->HowMany(eHexa) << "\n";
    // std::cout << "Number of Tetras read: " << fac->HowMany(eTetra) << "\n";
    // std::cout << "Number of Prisms read: " << fac->HowMany(ePrism) << "\n";
    // std::cout << "Number of Pyrams read: " << fac->HowMany(ePyram) << "\n";

    std::cout << "-=-=- Boundary elements -=-=-\n";
    for (int i = 1; i != 5; ++i) {
        std::cout << "Region: " << i << "\nElements:\n";
        for (auto it = minfo->BoundaryBegin(i); it != minfo->BoundaryEnd(i); ++it)
            std::cout << it->GetTag() << " ";
        std::cout << "\n";
    }
    std::cout << "-=-=- Interface elements -=-=-\n";
    for (auto it = minfo->InterfaceBegin(); it != minfo->InterfaceEnd(); ++it)
        std::cout << it->GetTag() << " ";
    std::cout << "\n";
    std::cout << "-=-=- Elements -=-=-\n";
    for (auto it = minfo->ElementsBegin(); it != minfo->ElementsEnd(); ++it)
        std::cout << it->GetTag() << " ";
    std::cout << "\n";
    


    //readertest.ShoutOutAll();


    ProfilerFlush();
    ProfilerStop();

    // fac->RunThrough();

    // std::cin.get();
    
}
