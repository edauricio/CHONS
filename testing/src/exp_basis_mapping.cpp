#include <iostream>
#include <cmath>
#include <initializer_list>

int main() {
    int a, b;
    int Nx=4, Ny=3, Nz=2;
    int eleOrder = 3;
    int numVertices = 4;
    int numEdges = 4;

    int vertMap[] = {0, eleOrder, eleOrder, 0};
    // int vertMapy[] = {0, 0, eleOrder, eleOrder};
    int edgeMap[] = {1, eleOrder, eleOrder-1, 0};
    // int edgeMapy[] = {0, 1, eleOrder, eleOrder-1};
    int run[] = {1, 0, -1, 0};
    // int runy[] = {0, 1, 0, -1};
    // ...x and ...y above only differ in the order (%modulo), i.e., the mapping
    // for y coord is always one "behind" the x coord in the modulo sequence;
    // Hence only one mapping is needed and we can take advantage of that during
    // the looping through the nodes
    

    // for (int b = 0; b != Ny; ++b)
    //     for (int a = 0; a != Nx; ++a)
    //         std::cout << b*Nx + a << " ";

    // std::cout << "\n\n";
    int cnt=-1;

    for (int k = 0; k != std::ceil((eleOrder-1)/2.) + 1; k++) {
        // Vertex loop
        // The %(2*eleOrder) thing is needed for the last node, where i
        // will be 2 times the eleOrder (because of the post-loop addition part)
        // Before that, it will always be less than 2*eleOrder, hence i%... = i.
        // We can always do like the edge and face loops below, too...
        // Since these auxiliary arrays will be local, stack-allocated variables,
        // there's no memory issues
        
        for (int i = 0; i != numVertices; ++i)
            std::cout << ++cnt << ": " << vertMap[i] << " " 
                        << vertMap[(i+numVertices-1) % numVertices] << "\n";

        std::cout << "\n\n";

        // Edge loops
        

        for (int i = 0; i != numEdges; ++i) {
            int indx = edgeMap[i];
            int indy = edgeMap[(i+numEdges-1) % numEdges];
            for (int j = k; j != eleOrder-(k+1); ++j) {
                std::cout << ++cnt << ": " << indx << " " << indy << "\n";
                indx += run[i];
                indy += run[(i+numEdges-1) % numEdges];
            }
        }

        std::cout << "\n\n";

        for (int i = 0; i != numVertices; ++i)
            (i % (numVertices-1)) ? vertMap[i]-- : vertMap[i]++;

        for (int i = 0; i != numEdges; ++i)
            (i % (numEdges-1)) ? edgeMap[i]-- : edgeMap[i]++;
    }

    // straightforward "tensor product" manner
    // requires remapping the element nodes vector though
    for (int j = 0; j != Ny; ++j) {
        for (int i = 0; i != Nx; ++i)
            std::cout << j*Nx + i << ": " << i << " " << j << "\n";
    }

    std::cout << "\n--------------------------\n\n";

    // Edge node numbering remap (CORRECT!!! ANY ORDER)

    eleOrder = 7;
    int cnt_n = 0;
    int ini_pos[] = {0, (eleOrder+1)*(eleOrder+1)-1, 1, 2*(eleOrder+1)-1, 
                    (eleOrder+1)*(eleOrder+1)-1-1, 
                    ((eleOrder+1)*(eleOrder+1)-1) - (2*(eleOrder+1)-1)};
    for (int k = eleOrder; k > 0; k -= 2) {
        // Bottom vertices
        for (int i = 0; i != 2; ++cnt_n, ++i)
            std::cout << cnt_n << ": " << ini_pos[0] + i*k  << "\n";
        // Top vertices
        for (int i = 0; i != 2; ++cnt_n, ++i)
            std::cout << cnt_n << ": " << ini_pos[1] - i*k  << "\n";

        // Bottom edge interior nodes
        for (int i = 0; i != k-1; ++i, ++cnt_n)
            std::cout << cnt_n << ": " << ini_pos[2] + i << "\n"; // (cnt_n - k) + i ?

        // Right edge interior nodes
        for (int i = 0; i != k-1; ++i, ++cnt_n)
            std::cout << cnt_n << ": " << ini_pos[3] + i*(eleOrder+1) << "\n";

        // Top edge interior nodes
        for (int i = 0; i != k-1; ++i, ++cnt_n)
            std::cout << cnt_n << ": " << ini_pos[4] - i << "\n";

        // Left edge interior nodes
        for (int i = 0; i != k-1; ++i, ++cnt_n)
            std::cout << cnt_n << ": " << ini_pos[5] - i * (eleOrder+1) << "\n";

        // If the quad we're analyzing is of 2nd order, it'll have a single internal
        // node, so let's account for it
        if (k == 2)
            std::cout << cnt_n++ << ": " 
                            << ini_pos[5] + 1 << "\n";

        // Recalculate initial positions for the next, inner quad
        ini_pos[0] += (eleOrder + 1 + 1);
        ini_pos[1] -= (eleOrder + 1 + 1);
        ini_pos[2] = ini_pos[0] + 1;
        ini_pos[3] += eleOrder;
        ini_pos[4] = ini_pos[1] - 1;
        ini_pos[5] -= eleOrder;
    }


}