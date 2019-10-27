#include "MeshModule/MeshReader.h"
#include <iostream>
#include <sstream>

namespace CHONS {

// ---------- MeshReader Member Function Definitions --------- //

MeshReader::MeshReader(const std::string& fName) : s_sectionReader(nullptr),
                s_fName(fName), s_meshFile(fName), s_physicalRegionEntities(4),
                s_nodesDone(false), s_edgesDone(false) {    
    if (!s_meshFile.is_open()) {
        std::cout << "Unable to open given mesh file.\n";
        exit(-1);
    }
    if (!(s_sectionReader = GetSectionObj(s_meshFile))) {
        std::cerr << "Unable to get a Section reader object.\n";
        exit(-1);
    }
    ReadMeshDim();
    ReadBoundaries();
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

    s_meshDim = -1;
    for (auto& e : header) {
        if (e>0)
            s_meshDim++;
    }

    if (s_meshDim < 0) {
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
    // This info may be gathered from PhysicalNames and Entities sections.

    // Reading physical boundary tags and its name into the associated map
    std::vector<int> header;
    header = s_sectionReader->GoToSection("PhysicalNames")->ReadSectionHeader();
    if (header.size() != 1) {
        std::cerr << "Invalid header for PhysicalNames section.\n";
        exit(-1);
    }
    
    std::tuple<int, int, std::string> tbnd;
    for (int i = 0; i != header.front(); ++i) {
        s_sectionReader->Next(tbnd);
        if (std::get<0>(tbnd) > s_meshDim) {
            std::cerr << "Invalid definition of Physical boundary entity. "
                    << "Dimension of entity is greater than mesh dimension.\n";
            exit(-1);
        }
        s_physicalRegionNames.emplace(std::get<1>(tbnd), std::get<2>(tbnd));
    }

    // Now reading which entities belong to which boundary region
    header = s_sectionReader->GoToSection("Entities")->ReadSectionHeader();
    if (header.size() != 4) {
        std::cerr << "Invalid header for Entities section.\n";
        exit(-1);
    }

    std::pair<int, int> mbndents;
    for (int i = 0; i != header.size(); ++i) {
        if (s_physicalRegionEntities[i]) { 
            std::cerr << "Pointer to map of entities/physical region "
                    << "already exists in dimension " << i << " ?\n";
            continue;
        }
        for (int j = 0; j != header[i]; ++j) {
            s_sectionReader->Next(mbndents, i);
            if (mbndents.second > 0) {
                if (!s_physicalRegionEntities[i])
                    s_physicalRegionEntities[i] = new std::map<int, int>;
                s_physicalRegionEntities[i]->insert(mbndents);
            }
        }
    }
}

// TODO: Insert boundary region awareness into Node creation
// REMINDER: Boundary region awareness is important for elements of dimension
// GetMeshDim-1, i.e., if mesh is 2D, only 1D+ elements is of interest.
void GmshReader::ReadNodes() {
    std::vector<int> header;
    header = s_sectionReader->GoToSection("Nodes")->ReadSectionHeader();
    if (header.size() != 4) {
        std::cerr << "Invalid header for Nodes section.\n";
        exit(-1);
    }

    std::vector<int> blockHeader;
    ElementInfo einfo;
    int rTag = -1;
    einfo.type = eNode;
    std::map<size_t, std::vector<double>> tags_coords;
    // In Gmsh context, Next() means to read the next EntityBlock, outputting
    // a set of tags mapped to node coordinates
    for (size_t i = 0; i != header.front(); i++) {
        blockHeader = s_sectionReader->Next(tags_coords);
        // Checking for boundary/region entity is only meaningful for entities
        // greater or equal to s_meshDim-1. That is, if mesh dimension is 1D,
        // checking for a boundary node is meaningful. However, if it is a 2D
        // mesh, an edge should be a boundary entity, not a node. Making a node
        // a boundary entity would break the logic afterwards, since the logic
        // to check if an element is a boundary one is to check it any of its
        // primitives are. For example, in a Quad, if any of its primitives, 
        // which are 1D edges, are in a boundary, then the Quad is a boundary
        // element. Even though a Node check wouldn't be performed in this case, 
        // marking a Node a boundary element is a waste of resources.
        if ((s_meshDim == 1) && (s_physicalRegionEntities[0]))
            if (s_physicalRegionEntities[0]->find(blockHeader[1]) !=
                s_physicalRegionEntities[0]->end())
                rTag = s_physicalRegionEntities[0]->at(blockHeader[1]);
        for (auto& me : tags_coords) {
            einfo.tag = me.first;
            einfo.coords = me.second;
            s_factory->GetElement(einfo);
            if (!s_factory->Created()) {
                std::cerr << "Duplicated Node tag. Double check mesh file.\n";
                exit(-1);
            }
            // Region Tag should be handled here by GraphHandler
            // s_graphHandler->Add(s_factory->GetElement(einfo), rTag)
            // or something like it
            rTag = -1; // reset rTag
        }
    }
    s_nodesDone = true;
}

void GmshReader::ReadEdges() {
    if (!s_nodesDone) {
        std::cerr << "Can't read Edges before Nodes.\n";
        exit(-1);
    }
    std::vector<int> header;
    header = s_sectionReader->GoToSection("Elements")->ReadSectionHeader();
    if (header.size() != 4) {
        std::cerr << "Invalid header for Elements section.\n";
        exit(-1);
    }

    std::vector<int> blockHeader;
    std::map<size_t, std::vector<int>> tagsAndNodes;
    int rTag = -1;
    ElementInfo einfo;
    ElementInfo priminfo;
    einfo.type = eLine;
    priminfo.type = eNode;
    for (int i = 0; i != header.front(); ++i) {
        blockHeader = s_sectionReader->Next(tagsAndNodes);
        if (blockHeader[0] == 1) {
            // Check if the entity is a physical region
            if ((s_meshDim == 2) && s_physicalRegionEntities[1])
                if (s_physicalRegionEntities[1]->find(blockHeader[1])
                    != s_physicalRegionEntities[1]->end())
                    rTag = s_physicalRegionEntities[1]->at(blockHeader[1]);

            // Read nodes and tags
            for (auto& me : tagsAndNodes) {
                einfo.tag = me.first;
                //TODO for each node tag in me.second, add it to primitives list
                for (auto it = me.second.begin(); it != me.second.end(); ++it) {
                    priminfo.tag = *it;
                    einfo.prims.push_back(s_factory->GetElement(priminfo));
                    if (s_factory->Created()) {
                        std::cerr << "Duplicated Node tag. "
                                    << "Double check mesh file.\n";
                        exit(-1);
                    }
                }
            }
            s_factory->GetElement(einfo);
        }
    }

    s_edgesDone = true;
}

// Divide further into (private) Read2DElements / Read3DElements ?
void GmshReader::ReadElements() {
    if (!s_edgesDone) {
        std::cerr << "Can't read Element before Edges.\n";
        exit(-1);
    }
    std::vector<int> header;
    header = s_sectionReader->GoToSection("Elements")->ReadSectionHeader();
    if (header.size() != 4) {
        std::cerr << "Invalid header for Elements section.\n";
        exit(-1);
    }

    std::vector<int> blockHeader;
    std::map<size_t, std::vector<int>> tagsAndNodes;
    ElementInfo einfo;
    int rTag = -1;
    for (int i = 0; i != header.front(); ++i) {
        blockHeader = s_sectionReader->Next(tagsAndNodes);
        if (tagsAndNodes.empty()) continue; // 0D elements read; nothing to do
        // 1) check if the entity is a physical region
        if ((blockHeader[0] >= s_meshDim-1) 
            && s_physicalRegionEntities[blockHeader[0]])
            if (s_physicalRegionEntities[blockHeader[0]]->find(blockHeader[1])
                != s_physicalRegionEntities[blockHeader[0]]->end())
                rTag = s_physicalRegionEntities[blockHeader[0]]->at(
                                                            blockHeader[1]);
        // 2) do the magic trick to get the primitives of the element by
        // the node tags given
        if (blockHeader[0] == 1) { // Reading Edges

        }
        // 3) setup einfo
        // 4) create element and add to graph
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

    for (auto& pm : s_physicalRegionEntities)
        if (pm) delete pm;
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
    s_sectionDone = false;
    // std::getline(s_iFile, s); // read section header and set the marker to 
    //                           // first entity block
    // s_firstEntityBlock = s_iFile.tellg(); // get first entity block marker
    // s_lastEntityBlockRead = s_firstEntityBlock;

    return this;
}

void GmshSection::CheckNext(const std::string& sec) {
    if (!s_headerDone) {
        std::cerr << "No header has been read for this section. Can't proceed"
                    << " to Next.\n";
        exit(-1);
    }
    if (s_sectionDone) {
        std::cerr << "End of Section has been reached. Can't read Next.\n";
        exit(-1);
    }
    if ((s_name != sec) || !s_curSectionMarkers[1]) {
        std::cerr << "Markers in wrong section or nonexistent."
                  << " Use GoToSection(\"" << sec << "\") fisrt.\n";
        exit(-1);
    }
    if ((s_iFile.tellg() < s_lastEntityBlockRead) || (s_iFile.tellg() >
        s_curSectionMarkers[1])) {
        std::cerr << "Marker for reading Next entity block is out of place.\n";
        exit(-1);
    }
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
    s_sectionDone = false;

    return ret;
}

// Next for reading Nodes entity blocks
std::vector<int> GmshASCIISection::Next(std::map<size_t, 
                                        std::vector<double>>& tagCoords) {
    CheckNext("Nodes");

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

    return vinfo;
}

// Next for reading Elements (tag, vector of nodes it comprises) entity blocks
std::vector<int> GmshASCIISection::Next(std::map<size_t, 
                                        std::vector<int>>& tagNodes) {
    CheckNext("Elements");

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
        std::cerr << "Invalid Entity Block header for reading Elements.\n";
        exit(-1);
    }

    std::string garbage;
    tagNodes.clear();
    for (int i = 0; i != vinfo.back(); ++i) {
        if (vinfo.front()) {
            // Register tag key into the map with an empty vector of nodes
            if ((s_iFile >> itmp) && (itmp > 0))
                tagNodes.emplace(itmp, std::vector<int>());
            else {
                std::cerr << "Invalid element tag in entity tagged "
                            << vinfo[1] << ".\n";
                exit(-1);
            }
            // Now run through the nodes list
            auto& vec = tagNodes.at(itmp);
            while (s_iFile >> itmp) {
                vec.push_back(itmp);
                while (s_iFile.get(c))
                    if (c != ' ') { s_iFile.unget(); break; }
                if (s_iFile.peek() == '\n') { s_iFile.get(c); break; }
            }
        } else
            std::getline(s_iFile, garbage);
    }

    s_lastEntityBlockRead = s_iFile.tellg();
    if (s_lastEntityBlockRead >= s_curSectionMarkers[1]) {
        s_curSectionMarkers[0] = s_curSectionMarkers[1] = 0;
        s_sectionDone = true;
    }

    return vinfo;
}

// Next for reading Boundaries (entity dim. -- curve/suface, region tag, name)
std::vector<int> GmshASCIISection::Next(std::tuple<int, int, 
                                        std::string>& bndRegion) {
    CheckNext("PhysicalNames");

    std::vector<int> vi;
    int tmp;
    std::string name;
    char ch;
    while (s_iFile >> tmp) {
        vi.push_back(tmp);
    }
    if (vi.size() != 2) {
        std::cerr << "Invalid definition of Physical Boundaries.\n";
        exit(-1);
    }
    s_iFile.clear();
    s_iFile.get(ch);
    if (ch != '\"') {
        std::cerr << ch << "Invalid definition of Physical Boundaries.\n";
        exit(-1);
    }
    if (!(std::getline(s_iFile, name, '\"'))) {
        std::cerr << "Invalid definition of Physical Boundaries.\n";
        exit(-1);
    }
    bndRegion = std::make_tuple(vi[0], vi[1], name);

    s_lastEntityBlockRead = s_iFile.tellg();
    if (s_lastEntityBlockRead >= s_curSectionMarkers[1]) {
        s_curSectionMarkers[0] = s_curSectionMarkers[1] = 0;
        s_sectionDone = true;
    }
    return std::vector<int>();
    
}


// Next for reading Entities (entity tag and physical boundary tag)
std::vector<int> GmshASCIISection::Next(std::pair<int, int>& bndEnts, 
                                        int entDim) {
    CheckNext("Entities");

    double dtmp;
    char ch;
    std::vector<double> tvd;
    while (s_iFile >> dtmp) {
        tvd.push_back(dtmp);
        while (s_iFile.get(ch))
            if (ch != ' ') { s_iFile.unget(); break; }
        if (s_iFile.peek() == '\n') { s_iFile.get(ch); break; }
    }

    s_lastEntityBlockRead = s_iFile.tellg();
    if (s_lastEntityBlockRead >= s_curSectionMarkers[1]) {
        s_curSectionMarkers[0] = s_curSectionMarkers[1] = 0;
        s_sectionDone = true;
    }

    switch (entDim) {
        case 0: // Point entity
            if (tvd.size() < 5) {
                std::cerr << "Invalid Point entity definition.\n";
                exit(-1);
            }
            if (tvd[4])
                bndEnts = std::make_pair(static_cast<int>(tvd[0]), 
                            static_cast<int>(tvd[5]));
            else
                bndEnts = std::make_pair(static_cast<int>(tvd.front()),
                                         -1);

            break;

        case 1: // Curve entity
            if (tvd.size() < 11) {
                std::cerr << "Invalid Curve entity definition.\n";
                exit(-1);
            }
            if (tvd[7])
                bndEnts = std::make_pair(static_cast<int>(tvd[0]), 
                            static_cast<int>(tvd[8]));
            else
                bndEnts = std::make_pair(static_cast<int>(tvd.front()),
                                         -1);
            
            break;
            

        case 2: // Surface entity
            if (tvd.size() < 10) {
                std::cerr << "Invalid Surface entity definition.\n";
                exit(-1);
            }
            if (tvd[7])
                bndEnts = std::make_pair(static_cast<int>(tvd[0]), 
                            static_cast<int>(tvd[8]));
            else
                bndEnts = std::make_pair(static_cast<int>(tvd.front()),
                                         -1);

            break;

        case 3: // Volume entity
            if (tvd.size() < 10) {
                std::cerr << "Invalid Volume entity definition.\n";
                exit(-1);
            }
            if (tvd[7])
                bndEnts = std::make_pair(static_cast<int>(tvd[0]), 
                            static_cast<int>(tvd[8]));
            else
                bndEnts = std::make_pair(static_cast<int>(tvd.front()),
                                         -1);

            break;

        default:
            std::cerr << "Unknown entity dimension.\n";
            exit(-1);
            break;
    }

    return std::vector<int>();
}



// ---------- GmshASCIISection Member Function Definitions --------- //

} // end of CHONS namespace