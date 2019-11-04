#include <vector>

int main() {

    size_t N = 10000000;
    int* a;
    // Section for many allocation of small objects.

    // We expect to see a huge memory overhead here, since the OS memory manager
    // uses a space of the newly allocated memory to save information, such as
    // the block (chunk) size -- so it can properly free it later.
    // In the case of a int the memory overhead expected is greater than 25%
    
    // /usr/bin/time -v output:
    //  Maximum resident set size (kbytes): 34040

    // for (int i = 0; i != 1000000; ++i)
    //     a = new int;


    // Section for one single allocation of these small objects.

    // Since only one new operation is done here, the memory manager probably
    // try to fit all requested memory in a single chunk / block of memory,
    // hence the portion used by it is very small compared to the overall size
    // asked

    // /usr/bin/time -v output:
    // Maximum resident set size (kbytes): 2820

    a = new int[N];
    

    // SUCCESSFULLY CHECKED!!!
    // Overhead: 1200% !!!

    delete[] a;
}