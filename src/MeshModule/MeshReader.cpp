#include "MeshModule/MeshReader.h"
#include <iostream>

namespace CHONS {

// ---------- MeshReader Member Function Definitions --------- //

MeshReader::MeshReader(std::string fName) {
    s_meshFile.open(fName);
    if (!s_meshFile.is_open()) {
        std::cout << "Unable to open mesh file given.\n";
        exit(-1);
    }
    s_sectionReader = this->GetSection(s_meshFile);
}

// ---------- End Of MeshReader Member Function Definitions --------- //

// ---------- GmshReader Member Function Definitions --------- //

Section* GmshReader::GetSection(const std::ifstream& file) {
    // read header to check whether it's in ASCI or binary format
    // and instantiate the right section.
    // currently: only ASCII implemented.
    return new GmshSection(s_meshFile);
}

GmshReader::~GmshReader() {
    if (s_sectionReader)
        delete s_sectionReader;
}

// ---------- GmshReader Member Function Definitions --------- //

} // end of CHONS namespace