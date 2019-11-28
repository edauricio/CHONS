#include <iostream>

int main() {
    int a, b, j;
    int Nx=4, Ny=4;
    int eleOrder = 3;
    

    for (int b = 0; b != Ny; ++b)
        for (int a = 0; a != Nx; ++a)
            std::cout << b*Nx + a << " ";

    std::cout << "\n\n";

    // Vertex loop
    for (int j = 0; j != 2; ++j)
        for (int i = j; i != j+2; ++i)
            std::cout << i%2 << " " << j << "\n";

    std::cout << "\n\n";

    // Edge loops
    int numEdges = 4;
    int ix = 2, iy = 0, runx=1, runy=0;
    bool edgeFlag = false;
    for (int i = 0; i != numEdges; ++i) {
        if (!(i % 2)) {
            // we're at an horizontal edge; now what?
            ix = (i == 0) ? 2 : eleOrder;
            runx = (i == 0) ? 1 : -1;
            iy = (i == 0) ? 0 : 1;
            runy = 0;
        } else {
            // we're at a vertical edge; now what?
            ix = (i == 1) ? i : 0;
            runx = 0;
            iy = (i == 1) ? 2 : eleOrder;
            runy = (i == 1) ? 1 : -1;
        }
        for (int j = 0; j != eleOrder-1; ++j) {
            std::cout << ix << " " << iy << "\n";
            ix += runx;
            iy += runy;
        }        
    }

    std::cout << "\n\n";

    // Face loops


}