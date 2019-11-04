#include <iostream>

int main() {

    size_t bit1, bit2, bit3, bit4;
    bit1 = 99;
    bit2 = 4;
    bit3 = 97;
    bit4 = 6;

    std::cout << "Sum of 1+2: " << bit1+bit2 << "\nSum of 3+4: " << bit3+bit4
                << "\nBXOR of 1&2: " << (bit1 ^ bit2) << "\nBXOR of 3&4: " << (bit3 ^ bit4)
                << "\nBXOR of 2&1: " << (bit2 ^ bit1) << "\nBXOR of 4&3: " << (bit4 ^ bit3) << std::endl;

    std::cout << std::endl;

    std::cout << "\n1XOR2XOR3XOR4: " << (bit1 & bit2 & bit3 & bit4)
              << "\n1XOR2XOR4XOR3: " << (bit1 & bit2 & bit4 & bit3)
              << "\n1XOR3XOR2XOR4: " << (bit1 & bit3 & bit2 & bit4)
              << "\n1XOR3XOR4XOR2: " << (bit1 & bit3 & bit4 & bit2)
              << "\n1XOR4XOR2XOR3: " << (bit1 & bit4 & bit2 & bit3)
              << "\n1XOR4XOR3XOR2: " << (bit1 & bit4 & bit3 & bit2)

              << "\n2XOR1XOR3XOR4: " << (bit2 & bit1 & bit3 & bit4)
              << "\n2XOR1XOR4XOR3: " << (bit2 & bit1 & bit4 & bit3)
              << "\n2XOR3XOR1XOR4: " << (bit2 & bit3 & bit1 & bit4)
              << "\n2XOR3XOR4XOR1: " << (bit2 & bit3 & bit4 & bit1)
              << "\n2XOR4XOR1XOR3: " << (bit2 & bit4 & bit1 & bit3)
              << "\n2XOR4XOR3XOR1: " << (bit2 & bit4 & bit3 & bit1)

              << "\n3XOR1XOR2XOR4: " << (bit3 & bit1 & bit2 & bit4)
              << "\n3XOR1XOR4XOR2: " << (bit3 & bit1 & bit4 & bit2)
              << "\n3XOR2XOR1XOR4: " << (bit3 & bit2 & bit1 & bit4)
              << "\n3XOR2XOR4XOR1: " << (bit3 & bit2 & bit4 & bit1)
              << "\n3XOR4XOR1XOR2: " << (bit3 & bit4 & bit1 & bit2)
              << "\n3XOR4XOR2XOR1: " << (bit3 & bit4 & bit2 & bit1)

              << "\n4XOR1XOR2XOR3: " << (bit4 & bit1 & bit2 & bit3)
              << "\n4XOR1XOR3XOR2: " << (bit4 & bit1 & bit3 & bit2)
              << "\n4XOR2XOR1XOR3: " << (bit4 & bit2 & bit1 & bit3)
              << "\n4XOR2XOR3XOR1: " << (bit4 & bit2 & bit3 & bit1)
              << "\n4XOR3XOR1XOR2: " << (bit4 & bit3 & bit1 & bit2)
              << "\n4XOR3XOR2XOR1: " << (bit4 & bit3 & bit2 & bit1)
              << std::endl;

    // std::cout << "Numbers: " << bit1 << "\t" << bit2 << "\t" << bit3;
    // std::cout << "\n1LOR2: " << (bit1 | bit2) << "\n1LAND2: "
    //         << (bit1 & bit2) << "\n1LOR3: " << (bit1 | bit3)
    //         << "\n1LAND3 " << (bit1 & bit3);
    // std::cout << "\n2LAND1: " << (bit2 & bit1) << "\n3LAND1: " << (bit3 & bit1)
    //             << "\n";
    // std::cout << "\n(N)OR: " << (~bit1 | bit2) << "\n(N)OR(N): "
    //         << (~bit1 | ~bit2) << "\nOR(N): " << (bit1 | ~bit2) << "\n";

    // std::cout << "\n(N)AND: " << (~bit1 & bit2) << "\n(N)AND(N): "
    //         << (~bit1 & ~bit2) << "\nAND(N): " << (bit1 & ~bit2) << "\n";        
}

// 0000000001011111 : 95
// 0000000000000100 : 4