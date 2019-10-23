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
    if (!(s_sectionReader = GetSectionObj(s_meshFile))) {
        std::cerr << "Unable to get a Section reader object.\n";
        exit(-1);
    }
    ReadMeshDim();
    s_factory = ElementFactory::GetInstance();
}

// ---------- End Of MeshReader Member Function Definitions --------- //

// ---------- GmshReader Member Function Definitions --------- //

GmshReader::GmshReader(const std::string& fName) : MeshReader(fName) {
    // Check if msh version is supported.
    // TODO: implement support for format 2.2
    if (s_meshVersion != 4.1) {
        std::cout << "Gmsh version currently not supported."
                    << " Please upgrade to file version 4.1.\n";
        exit(-1);
    }
}

void GmshReader::ReadMeshDim() {
    std::vector<int> header;
    header = s_sectionReader->GoToSection("Entities")->ReadSectionHeader();
    if (header.size() != 4) {
        std::cerr << "Invalid header for Entities section.\n";
        exit(-1);
    }

    s_dim = -1;
    for (auto& e : header) {
        if (e>0)
            s_dim++;
    }

    if (s_dim < 0) {
        std::cerr << "Invalid Mesh dimension. (Check Entities section?)\n";
        exit(-1);
    }
}

void GmshReader::ReadBoundaries() {
    // In Gmsh context, to read the boundaries means to write down the entities
    // comprising the named boundaries of the domain, i.e., get the entity
    // tags and dimension (curve, surface) so that when reading an entity block
    // in Elements section, it's crystal clear which elements belong to which
    // named boundaries.
    // This info may be gathered from both PhysicalNames and Entities sections.
    std::vector<int> header;
    header = s_sectionReader->GoToSection("PhysicalNames")->ReadSectionHeader();
    if (header.size() != 1) {
        std::cerr << "Invalid header for PhysicalNames section.\n";
        exit(-1);
    }
    
    std::tuple<size_t, size_t, std::string> tbnd;
    s_sectionReader->Next(tbnd);
}

void GmshReader::ReadNodes() {
    std::vector<int> header;
    header = s_sectionReader->GoToSection("Nodes")->ReadSectionHeader();
    if (header.size() != 4) {
        std::cerr << "Invalid header for Nodes section.\n";
        exit(-1);
    }

    ElementInfo einfo;
    einfo.type = eNode;
    std::map<size_t, std::vector<double>> tags_coords;
    // In Gmsh context, Next() means to read the next EntityBlock, outputting
    // a set of tags mapped to its node coordinates
    for (size_t i = 0; i != header.front(); i++) {
        s_sectionReader->Next(tags_coords);
        for (auto& me : tags_coords) {
            einfo.tag = me.first;
            einfo.coords = me.second;
            s_factory->GetElement(einfo);
            if (!s_factory->Created()) {
                std::cerr << "Duplicated Node tag. Double check mesh file.\n";
                exit(-1);
            }
        }
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
        s_meshVersion = stod(section, &where);
        s_meshFormat = stoi(section.substr(where));

        switch (s_meshFormat) {
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

    s_iFile.seekg(s_curSectionMarkers[0]); // set marker at section header
    s_headerDone = false;
    // std::getline(s_iFile, s); // read section header and set the marker to 
    //                           // first entity block
    // s_firstEntityBlock = s_iFile.tellg(); // get first entity block marker
    // s_lastEntityBlockRead = s_firstEntityBlock;

    return this;
}


// ---------- End Of GmshSection Member Function Definitions --------- //

// ---------- GmshASCIISection Member Function Definitions --------- //

std::vector<int> GmshASCIISection::ReadSectionHeader() {
    if (s_name.empty() || s_curSectionMarkers[1]) {
        std::cerr << "No Section to read. Use GoToSection(...) fisrt.\n";
        exit(-1);
    }

    std::vector<int> ret;

    s_iFile.seekg(s_curSectionMarkers[0]); // Position marker at section header
    char ch;
    int itmp;
    while (s_iFile >> itmp) {
        ret.push_back(itmp);
        while (s_iFile >> ch)
            if (ch != ' ') { s_iFile.unget(); break; }
        if (s_iFile.peek() == '\n') { s_iFile.get(ch); break; }
    }
    s_lastEntityBlockRead = s_iFile.tellg(); // set last block read to init.

    s_headerDone = true;
    return ret;
}

Section* GmshASCIISection::Next(std::map<size_t, std::vector<double>>& tagCoords) {
    // Check for marker (see if it's between s_firstEntityBlock and
    // s_curSectionMarkers[1]).
    // Check for number of arguments (must be 4 to define an entity block
    // header)
    // idea: write down the last entity block read (marker and number of
    // elements in it) for a better check.
    // etc
    if (!s_headerDone) {
        std::cerr << "No header has been read for this section. Can't proceed"
                    << " to Next.\n";
        exit(-1);
    }
    if (s_sectionDone) {
        std::cerr << "End of Section has been reached. Can't read Next.\n";
        exit(-1);
    }
    if ((s_name != "Nodes") || !s_curSectionMarkers[1]) {
        std::cerr << "Markers in wrong section or nonexistent."
                  << " Use GoToSection(\"Nodes\") fisrt.\n";
        exit(-1);
    }
    if ((s_iFile.tellg() < s_lastEntityBlockRead) || (s_iFile.tellg() >
        s_curSectionMarkers[1])) {
        std::cerr << "Marker for reading Next entity block is out of place.\n";
        exit(-1);
    }

    std::vector<int> vinfo;
    char c;
    int itmp;
    while (s_iFile >> itmp) {
        vinfo.push_back(itmp);
        while (s_iFile.get(c))
            if (c != ' ') { s_iFile.unget(); break; }
        if (s_iFile.peek() == '\n') { s_iFile.get(c); break; }
    }
    if (vinfo.size() != 4) {
        std::cerr << "Invalid Entity Block header for reading Nodes.\n";
        exit(-1);
    }
    // For nodes, the header contains no useful information (for the code)
    // other than the number of nodes in the entity block. Hence, all elements
    // from vinfo are discarded but the last one.
    std::string stemp;
    double ctemp;
    tagCoords.clear();

    for (int i = 0; i != vinfo.back(); ++i) {
        std::getline(s_iFile, stemp);
        if (stoi(stemp) < 0) {
            std::cerr << "Invalid negative tag for a Node.\n";
            exit(-1);
        }
        tagCoords.emplace(stoul(stemp), std::vector<double>());
    }

    itmp = tagCoords.begin()->first;
    for (int i = 0; i != vinfo.back(); ++i) {
        while(s_iFile >> ctemp) {
            tagCoords[itmp].push_back(ctemp);
            while (s_iFile.get(c))
                if (c != ' ') { s_iFile.unget(); break; }
            if (s_iFile.peek() == '\n') { s_iFile.get(c); break; }
        }
        if (tagCoords[itmp].size() != 3) {
            std::cerr << "Invalid coordinates given for node " << itmp << " in"
                        << " mesh file.\n";
            exit(-1);
        }
        itmp++;
    }

    s_lastEntityBlockRead = s_iFile.tellg();
    if (s_lastEntityBlockRead >= s_curSectionMarkers[1]) {
        s_curSectionMarkers[0] = s_curSectionMarkers[1] = 0;
        s_sectionDone = true;
    }

    return this;
}

// Next for reading Elements (tag, vector of nodes it comprises, element type)
Section* GmshASCIISection::Next(std::map<size_t, std::vector<double>>& tagNodes,
                                int& eleType) {

}

// Next for reading Boundaries (entity dim. -- curve/suface, region tag, name)
Section* GmshASCIISection::Next(std::tuple<size_t, size_t, 
                                std::string>& bndRegion) {
    if (!s_headerDone) {
        std::cerr << "No header has been read for this section. Can't proceed"
                    << " to Next.\n";
        exit(-1);
    }
    if (s_sectionDone) {
        std::cerr << "End of Section has been reached. Can't read Next.\n";
        exit(-1);
    }
    if ((s_name != "Entities") || !s_curSectionMarkers[1]) {
        std::cerr << "Markers in wrong section or nonexistent."
                  << " Use GoToSection(\"Entities\") fisrt.\n";
        exit(-1);
    }
    if ((s_iFile.tellg() < s_lastEntityBlockRead) || (s_iFile.tellg() >
        s_curSectionMarkers[1])) {
        std::cerr << "Marker for reading Next entity block is out of place.\n";
        exit(-1);
    }

    // read and return line containing physical boundary info
    // TODO
}


// ---------- GmshASCIISection Member Function Definitions --------- //

} // end of CHONS namespace