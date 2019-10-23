#include "MeshModule/MeshReader.h"
#include <iostream>
#include <sstream>

namespace CHONS {

// ---------- MeshReader Member Function Definitions --------- //

MeshReader::MeshReader(const std::string& fName) : s_sectionReader(nullptr),
                s_fName(fName), s_meshFile(fName) {    
    if (!s_meshFile.is_open()) {
        std::cout << "Unable to open given mesh file.\n";
        exit(-1);
    }
    s_sectionReader = this->GetSectionObj(s_meshFile);
    s_Dim = GetMeshDim();
}

// ---------- End Of MeshReader Member Function Definitions --------- //

// ---------- GmshReader Member Function Definitions --------- //

GmshReader::GmshReader(const std::string& fName) : MeshReader(fName) {
    // Check if msh version is supported.
    // TODO: implement support for format 2.2
    if (s_mshVersion != 4.1) {
        std::cout << "Gmsh version currently not supported."
                    << " Please upgrade to file version 4.1.\n";
        exit(-1);
    }
}

void GmshReader::ReadNodes() {
    s_sectionReader->GoToSection("Nodes")->ReadSectionHeader();
    ElementInfo einfo;
    einfo.type = eNode;
    std::map<int, std::vector<double>> tags_coords;
    // In Gmsh context, Next() means to read the next EntityBlock
    for (size_t i = 0; i != s_sectionReader->GetTotalEntityBlocks(); i++) {
        s_sectionReader->Next(tag_coords);
    }
}

Section* GmshReader::GetSectionObj(std::ifstream& file) {
    if (!s_sectionReader) {
        std::string section;
        char ch; 
        file.seekg(0, file.beg);
        file.get(ch);
        std::getline(file, section);
        if ((ch != '$') || (section != "MeshFormat")) {
            std::cout << "Invalid Gmsh file.\n";
            exit(-1);
        }
        std::getline(file, section);

        std::string::size_type where;
        s_mshVersion = stod(section, &where);
        s_mshFormat = stoi(section.substr(where));

        switch (s_mshFormat) {
            case 0:
                return new GmshASCIISection(s_meshFile);

            case 1:
                return new GmshBinarySection(s_meshFile);

            default:
                std::cout << "Unrecognized Gmsh file format.\n";
                exit(-1);
        }
    }
    else
        return s_sectionReader;
}

GmshReader::~GmshReader() {
    if (s_sectionReader)
        delete s_sectionReader;
}

// ---------- End Of GmshReader Member Function Definitions --------- //

// ---------- Section Member Function Definitions --------- //



// ---------- End Of Section Member Function Definitions --------- //


// ---------- GmshSection Member Function Definitions --------- //

Section* GmshSection::GoToSection(const std::string& sec) {

    std::string s;
    s_iFile.seekg(0, s_iFile.beg);
    while (std::getline(s_iFile, s)) {
        if (s[0] == '$') {
            if (s.substr(1) == sec)
                s_curSectionMarkers[0] = s_iFile.tellg();
            if (s.substr(1) == ("End" + sec)) {
                s_iFile.seekg(-(sec.size()+5), s_iFile.cur);
                s_curSectionMarkers[1] = s_iFile.tellg();
                break;
            }
        }
    }
    if (s_iFile.eof()) {
        std::cout << "Couldn't read section $" << sec << ".\n";
        exit(-1);
    }
    s_name = sec;

    // Get marker for the first entity block
    s_iFile.seekg(s_curSectionMarkers[0]); // set marker at section header
    std::getline(s_iFile, s); // read section header and set the marker to 
                              // first entity block
    s_firstEntityBlock = s_iFile.tellg(); // get first entity block marker
    s_lastEntityBlockRead = s_firstEntityBlock;

    return this;
}


// ---------- End Of GmshSection Member Function Definitions --------- //

// ---------- GmshASCIISection Member Function Definitions --------- //

bool GmshASCIISection::ReadSectionHeader() {
    if (s_name.empty() || s_curSectionMarkers[1]) {
        std::cout << "No Section to read. Use GoToSection(...) fisrt.\n";
        exit(-1);
    }
    s_iFile.seekg(s_curSectionMarkers[0]);
    std::string header;
    size_t next;
    std::getline(s_iFile, header);
    s_numEntityBlocksInSection = stoi(header, &next);
    s_numSectionItemsTotal = stoi(header.substr(next), &next);
    s_minTag = stoi(header.substr(next), &next);
    s_maxTag = stoi(header.substr(next));
    // Obvisouly, this function leaves the marker in the beginning of
    // the first entity block.
}

void GmshASCIISection::Next(std::map<int, std::vector<double>>& tagCoords) {
    // Check for marker (see if it's between s_firstEntityBlock and
    // s_curSectionMarkers[1]).
    // Check for number of arguments (must be 4 to define an entity block
    // header)
    // idea: write down the last entity block read (marker and number of
    // elements in it) for a better check.
    // etc
    if (s_name.empty() || !s_curSectionMarkers[1]) {
        std::cout << "No Section to read. Use GoToSection(...) fisrt.\n";
        exit(-1);
    }
    if ((s_iFile.tellg() < s_lastEntityBlockRead) || (s_iFile.tellg() >
        s_curSectionMarkers[1])) {
        std::cout << "Marker for reading Next entity block is out of place.\n";
        exit(-1);
    }

    std::vector<int> vinfo;
    char c;
    int itmp;
    while (s_iFile >> itmp) {
        vinfo.push_back(itmp);
        while (s_iFile.get(c))
            if (c != ' ') { s_iFile.unget(); break; }
        if (s_iFile.get(c) == '\n') { s_iFile.get(c); break; }
    }
    if (vinfo.size() != 4) {
        std::cout << "Invalid Entity Block header for reading Nodes.\n";
        exit(-1);
    }

}

void GmshASCIISection::Next(std::map<size_t, std::vector<double>>& tagNodes,
                                int& eleType) {

}

// ---------- GmshASCIISection Member Function Definitions --------- //

} // end of CHONS namespace