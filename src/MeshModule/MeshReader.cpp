#include "MeshModule/MeshReader.h"
#include "MeshModule/GmshAuxiliaryDefs.h"
#include "boost/assert.hpp"
#include <iostream>
#include <sstream>
#include <cassert>
#include <algorithm>
#include <cmath>

namespace boost {
    void assertion_failed_msg(char const* expr, char const* msg,
        char const* function, char const* file, long line) {
        std::cout << "ASSERTION FAILED!\n";
        std::cout << "\"" << msg << "\"\n";
        std::cout << "failed: (" << expr << ")\n";
        std::cout << "in: " << file << ":" << line << ": " << function << "\n";
        std::abort();
    }
}

namespace CHONS {

// ---------- Auxiliary Functions ------------- //

// A simple and fast unordered pairing function.
// Growth of its result seems acceptable for our mesh sizes
// (i.e. fits a size_t).
inline size_t edge_upairing(const size_t& a, const size_t& b) {
    // return (a+b)*(a*b);
    return 0.5*std::fmax(a,b)*(std::fmax(a,b)+1) + std::fmin(a,b);
}
inline size_t face_upairing(const size_t& a, const size_t& b, const size_t& c) {
    return (a+b+c)+(a*b*c);
}
inline size_t face_upairing(const size_t& a, const size_t& b, const size_t& c,
                            const size_t& d) {
    return (a+b+c+d)+(a*b*c*d);
}

// ---------- End of Auxiliary Functions ------------- //

// ---------- MeshReader Member Function Definitions --------- //

MeshReader::MeshReader(const std::string& fName) : s_sectionReader(nullptr),
                s_fName(fName), s_meshFile(fName), s_physicalRegionEntities(4),
                s_nodesDone(false), s_edgesDone(false) {
    if (!s_meshFile.is_open()) {
        std::cout << "Unable to open given mesh file.\n";
        exit(-1);
    }
    s_factory = ElementFactory::GetInstance();
}

MeshReader::~MeshReader() {

}

// ---------- End Of MeshReader Member Function Definitions --------- //

// ---------- GmshReader Member Function Definitions --------- //

GmshReader::GmshReader(const std::string& fName) : MeshReader(fName) {
    BOOST_ASSERT_MSG(s_sectionReader = GetSectionObj(s_meshFile), 
            "Unable to get a Section reader object");

    // Check if msh version is supported.
    // TODO: implement support for format 2.2
    if (s_meshVersion != 4.1) {
        std::cerr << "Gmsh version currently not supported."
                    << " Please upgrade to file version 4.1.\n";
        exit(-1);
    }
    ReadMeshDim();
    s_graphProc = std::unique_ptr<GraphInfoProcessor>(
                                            new GraphInfoProcessor(s_meshDim));
    for (int i = 0; i != s_meshDim-1; ++i) {
        s_linearElementsNodes.push_back(std::unordered_map<size_t, size_t>());
        s_higherDimCache.push_back(std::map<std::vector<int>, 
                                    std::unordered_map<size_t, std::vector<size_t>>>());
        s_maxTagCreated.push_back(0);
    }
    ReadBoundaries();
}

void GmshReader::ReadMeshDim() {
    std::vector<int> header;
    header = s_sectionReader->GoToSection("Entities")->ReadSectionHeader();

    BOOST_ASSERT_MSG(header.size() == 4, "Invalid header for Entities section");

    s_meshDim = -1;
    for (auto& e : header) {
        if (e>0)
            s_meshDim++;
    }

    BOOST_ASSERT_MSG(s_meshDim > 0, 
                "Invalid Mesh dimension. (Check Entities section?)");
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
    
    BOOST_ASSERT_MSG(header.size() == 1, 
                "Invalid header for PhysicalNames section");
    
    
    std::tuple<int, int, std::string> tbnd;
    for (int i = 0; i != header.front(); ++i) {
        s_sectionReader->Next(tbnd);

        BOOST_ASSERT_MSG(std::get<0>(tbnd) <= s_meshDim,
            "Invalid definition of Physical boundary entity. "
            "Dimension of entity is greater than mesh dimension");

        s_physicalRegionNames.emplace(std::get<1>(tbnd), std::get<2>(tbnd));
    }

    // Now reading which entities belong to which boundary region
    header = s_sectionReader->GoToSection("Entities")->ReadSectionHeader();

    BOOST_ASSERT_MSG(header.size() == 4, 
                "Invalid header for Entities section");


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

void GmshReader::ReadNodes() {
    std::vector<int> header;
    header = s_sectionReader->GoToSection("Nodes")->ReadSectionHeader();
    
    BOOST_ASSERT_MSG(header.size() == 4, 
                "Invalid header for Nodes section");

    std::vector<int> blockHeader;
    ElementInfo einfo;
    int rTag;
    einfo.type = eNode;
    std::unordered_map<size_t, std::vector<double>> tags_coords;
    // In Gmsh context, Next() means to read the next EntityBlock, outputting
    // a set of tags mapped to node coordinates
    for (size_t i = 0; i != header.front(); i++) {
        rTag = -1; // reset rTag
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
            einfo.clear(); // reset containers in ElementInfo
            einfo.tag = me.first;
            einfo.coords = me.second;
            BOOST_ASSERT_MSG(s_factory->OrderElement(einfo), "Element placed in "
                "Order has already been created");
            s_graphProc->AddToMeshInfo(einfo, rTag);
        }
    }
    s_factory->PlaceOrder();
    s_nodesDone = true;
}

void GmshReader::ReadEdges() {
    BOOST_ASSERT_MSG(s_nodesDone, "Can't read Edges before Nodes");

    std::vector<int> header;
    header = s_sectionReader->GoToSection("Elements")->ReadSectionHeader();

    BOOST_ASSERT_MSG(header.size() == 4, 
                "Invalid header for Nodes section");

    std::vector<int> blockHeader;
    std::unordered_map<size_t, std::vector<size_t>> tagsAndNodes;
    size_t edgeNodes;
    int rTag;
    ElementInfo einfo;
    ElementInfo priminfo;
    einfo.type = eLine;
    priminfo.type = eNode;
    for (int i = 0; i != header.front(); ++i) {
        rTag = -1; // reset rTag
        blockHeader = s_sectionReader->Next(tagsAndNodes);
        if (blockHeader[0] == 1) {
            // Set order of element
            einfo.eleOrder = GmshElementsMapping[blockHeader[2]].second;
            // Check if the entity is a physical region
            if ((s_meshDim == 2) && s_physicalRegionEntities[1])
                if (s_physicalRegionEntities[1]->find(blockHeader[1])
                    != s_physicalRegionEntities[1]->end())
                    rTag = s_physicalRegionEntities[1]->at(blockHeader[1]);

            // Read nodes and tags
            // Add nodes for linear Edge into associated map for later retrieval
            for (auto& me : tagsAndNodes) {
                einfo.clear(); // reset containers in ElementInfo
                einfo.tag = me.first;
                for (auto it = me.second.begin(); it != me.second.end(); ++it) {
                    priminfo.tag = *it;
                    einfo.prims.push_back(s_factory->GetElement(priminfo));
                }
                BOOST_ASSERT_MSG(s_factory->OrderElement(einfo), 
                    "Element placed in Order has already been created");

                s_graphProc->AddToMeshInfo(einfo, rTag);

                s_maxTagCreated[0] = (einfo.tag > s_maxTagCreated[0]) ?
                                        einfo.tag : s_maxTagCreated[0];

                if (!s_linearElementsNodes.empty()) {
                    s_linearElementsNodes[0].emplace(
                                    edge_upairing(me.second[0], me.second[1]), 
                                    einfo.tag);
                }
            }
        } else if (blockHeader[0] == 2) {
            int numEdges;
            rTag = -1; // edges on surface entities are not a region
            einfo.eleOrder = GmshElementsMapping[blockHeader[2]].second;
            switch (GmshElementsMapping[blockHeader[2]].first) {
                case eQuad:
                    numEdges = 4; // numNodes == numEdges
                    break;

                case eTri:
                    numEdges = 3; // numNodes == numEdges
                    break;

                default:
                std::cerr << "Invalid element type for 2D entity.\n";
                exit(-1);
            }

            for (auto& eleNodes : tagsAndNodes) {
            // for each edge defining the element
                for (int i = 0; i != numEdges; ++i) {
                    edgeNodes = edge_upairing(eleNodes.second[i%numEdges],
                                            eleNodes.second[(i+1)%numEdges]);
                    if (s_linearElementsNodes[0].count(edgeNodes)) {
                        // std::cout << "Supposedly existing edge: "
                        //         << s_linearElementsNodes[0].at(edgeNodes)
                        //         << "\nLin. nodes: " << eleNodes.second[i%numEdges]
                        //         << " " << eleNodes.second[(i+1)%numEdges]
                        //         << "\nBXOR of Lin Nodes: " << edgeNodes
                        //         << "\n\n";
                        continue;
                    }
                    else {
                        // Edge doesn't exist; create it then
                        einfo.clear();
                        // for each node defining edge the linear edge
                        for (int j = i; j != i+2; ++j) {
                            priminfo.tag = eleNodes.second[j%numEdges];
                            einfo.prims.push_back(s_factory->GetElement(
                                                                priminfo));
                        }
                        // for each node defining the higher order of the edge
                        for (int j = 0; j != einfo.eleOrder-1; ++j) {
                            priminfo.tag = eleNodes.second[
                                            numEdges+(i*(einfo.eleOrder-1))+j];
                            einfo.prims.push_back(s_factory->GetElement(
                                                                priminfo));
                        }
                        // TODO: Change edge creation from 1D entity blocks to
                        // be sequential too, instead of the tags on gmsh file;
                        // this prevents double tags when the entity blocks in
                        // gmsh file aren't ordered (e.g. a 2D entity block
                        // before a 1D one will create edges with tags adding
                        // to the last one, then when defining edges itself
                        // in 1D entity block, those tags might already be taken)
                        einfo.tag = ++s_maxTagCreated[0];
                        // Add it to the linear elements list so that the adjacent
                        // 2D elements doesn't create it with another tag
                        s_linearElementsNodes[0].emplace(edgeNodes,
                                                                einfo.tag);

                        BOOST_ASSERT_MSG(s_factory->OrderElement(einfo), 
                            "Element placed in Order has already been created");
                        
                        s_graphProc->AddToMeshInfo(einfo, rTag);
                    }
                }
            }
            // Cache this entity block so we don't need to read again for 2D
            // elements ---- this would be removed after implementing the
            // "native" mesh file, since we wouldn't have mixed elements defined
            // there. Hence, less memory needed.
            s_higherDimCache[0].emplace(blockHeader, tagsAndNodes);

        } else if (blockHeader[0] == 3) {
            int numEdges, numNodes;
            rTag = -1; // edges on a 3D entity are disregarded as inside regions
            einfo.eleOrder = GmshElementsMapping[blockHeader[2]].second;
            ElementType cur_type = GmshElementsMapping[blockHeader[2]].first;
            switch (cur_type) {
                case eHexa:
                    numNodes = 8;
                    numEdges = 12;
                    break;

                case eTetra:
                    numNodes = 4;
                    numEdges = 6;
                    break;

                case ePrism:
                    numNodes = 6;
                    numEdges = 9;
                    break;

                case ePyram:
                    numNodes = 5;
                    numEdges = 8;
                    break;

                default:
                    std::cerr << "Unrecognized 3D Element type.\n";
                    break;
            }

            // For each element defined in this entity block
            for (auto& eleNodes : tagsAndNodes) {
                // for each edge on the 3D element
                for (int i = 0; i != numEdges; ++i) {
                    // set the nodes defining the linear edge to the list
                    edgeNodes = edge_upairing(eleNodes.second[
                                        GmshEdgesOn3DElements[cur_type][i][0]
                                        ]
                                    ,   eleNodes.second[
                                        GmshEdgesOn3DElements[cur_type][i][1]
                                        ]);
                    // check if edge already exists; if so, go to the next
                    if (s_linearElementsNodes[0].count(edgeNodes)) continue;
                    else {
                    // edge doesn't exist; create it then
                        einfo.clear();
                        // for each node defining the linear edge
                        for (int j = 0; j != 2; ++j) {
                            priminfo.tag = eleNodes.second[
                                        GmshEdgesOn3DElements[cur_type][i][j]
                                        ];
                            einfo.prims.push_back(
                                            s_factory->GetElement(priminfo));
                        }
                        // now add the nodes defining the higher order line
                        for (int j = 0; j != einfo.eleOrder-1; ++j) {
                            priminfo.tag = eleNodes.second[numNodes
                                            + (i*(einfo.eleOrder-1)) + j];
                            einfo.prims.push_back(
                                            s_factory->GetElement(priminfo));
                        }
                        // TODO: Change edge creation from 1D entity blocks to
                        // be sequential too, instead of the tags on gmsh file;
                        // this prevents double tags when the entity blocks in
                        // gmsh file aren't ordered (e.g. a 2D entity block
                        // before a 1D one will create edges with tags adding
                        // to the last one, then when defining edges itself
                        // in 1D entity block, those tags might already be taken)
                        // OR: also cache 1D entity blocks so we read only from
                        // cached info, guaranteeing reading in dimension order
                        einfo.tag = ++s_maxTagCreated[0];
                        s_linearElementsNodes[0].emplace(edgeNodes, 
                                                                einfo.tag);

                        BOOST_ASSERT_MSG(s_factory->OrderElement(einfo), 
                            "Element placed in Order has already been created");

                        s_graphProc->AddToMeshInfo(einfo, rTag);
                    }
                }
            }
            // Cache this entity block so we don't need to read again for 3D
            // elements
            s_higherDimCache[1].emplace(blockHeader, tagsAndNodes);
        }
    }
    s_factory->PlaceOrder();
    s_edgesDone = true;
}

// Divide further into (private) Read2DElements / Read3DElements ?
void GmshReader::ReadElements() {

    BOOST_ASSERT_MSG(s_edgesDone, "Can't read Element before Edges");

    if (s_meshDim < 2) return; // should this check be here?
                            // maybe add a ReadMesh() interface for MeshReader
                            // so that it checks meshDim and call accordingly?
    
    // Since Edges have been read already, all we need to do is read the cached
    // information for 2D and 3D elements
    Read2DElements();
    if (s_meshDim == 3) Read3DElements();
    s_graphProc->Done();
}


void GmshReader::Read2DElements() {
    ElementInfo einfo;
    ElementInfo priminfo;
    int numNodes, numEdges, numFaces;
    int rTag;
    size_t edgeNodes, faceNodes;
    // read 2D elements on cached 2D entity blocks
    for (auto& blkHeader_TagsAndNodes : s_higherDimCache[0]) {
        rTag = -1; // reset region tag
        // Check if entity is a physical region
        if (s_physicalRegionEntities[2])
            if (s_physicalRegionEntities[2]->find(
                                        blkHeader_TagsAndNodes.first[1]) 
                != s_physicalRegionEntities[2]->end())
                rTag = s_physicalRegionEntities[2]->at(
                                            blkHeader_TagsAndNodes.first[1]);
        
        einfo.type = GmshElementsMapping[
                            blkHeader_TagsAndNodes.first[2]].first;
        einfo.eleOrder = GmshElementsMapping[
                            blkHeader_TagsAndNodes.first[2]].second;
        switch (einfo.type) {
            case eQuad:
                numEdges = 4; // numNodes == numEdges
                break;

            case eTri:
                numEdges = 3; // numNodes == numEdges
                break;

            default:
                std::cerr << "Cached non-2D element in place of 2D elements.\n";
                std::abort();
        }
        // Run through the nodes list to retrive the primitive Edges that 
        // contain those nodes. 
        // Also, add 2D linear elements to the list of linear elements so that
        // 3D elements can find its primitives (on 2D entities) easily.
        for (auto& tagsAndNodes : blkHeader_TagsAndNodes.second) {
            einfo.clear(); // reset containers in ElementInfo
            einfo.tag = tagsAndNodes.first;
            // Retrive primitive edges from the first nodes in list
            priminfo.type = eLine;
            for (int i = 0; i != numEdges; ++i) {
                priminfo.clear(); // reset containers in ElementInfo
                edgeNodes = edge_upairing(tagsAndNodes.second[i%numEdges]
                                ,tagsAndNodes.second[(i+1)%numEdges]);
                // since all edges have been read, no check needed here, i.e.
                // the edge exists and this find will return its tag.
                priminfo.tag = s_linearElementsNodes[0].at(
                                                    edgeNodes);

                einfo.prims.push_back(s_factory->GetElement(priminfo));
            }
            // Now check for face nodes (higher order elements)
            //TODO: Change this to start at the correct face node
            // (known from numNodes, numEdges and eleOrder)
            priminfo.type = eNode;
            for (int j = 0; j != einfo.eleOrder-1; ++j) {
                priminfo.tag = tagsAndNodes.second[numEdges // numNodes
                                                + numEdges*(einfo.eleOrder-1)
                                                + j];
                einfo.prims.push_back(s_factory->GetElement(priminfo));
            }

            // Check primitive info, create element and add it to linear 
            // elements map
            BOOST_ASSERT_MSG(einfo.prims.size() >= numEdges,
                        "Invalid number of Edges to create 2D element.\n");
            
            BOOST_ASSERT_MSG(s_factory->OrderElement(einfo), 
                            "Element placed in Order has already been created");

            s_graphProc->AddToMeshInfo(einfo, rTag);

            s_maxTagCreated[1] = (einfo.tag > s_maxTagCreated[1]) ?
                                    einfo.tag : s_maxTagCreated[1];

            if (s_linearElementsNodes.size() > 1) { // add element to 
                // linear nodes->element map.
                    faceNodes = (numEdges == 3) ? 
                                face_upairing(tagsAndNodes.second[0],
                                                tagsAndNodes.second[1],
                                                tagsAndNodes.second[2]) :
                                face_upairing(tagsAndNodes.second[0],
                                                tagsAndNodes.second[1],
                                                tagsAndNodes.second[2],
                                                tagsAndNodes.second[3]);
                    s_linearElementsNodes[1].emplace(faceNodes,
                                                    tagsAndNodes.first);
            }                    
        }
    }

    // read 2D element on cached 3D entity block
    if (s_meshDim == 3)
    for (auto& blkHeader_TagsAndNodes : s_higherDimCache[1]) {
        rTag = -1; // this element being in a region makes no sense
        ElementType cur_type = GmshElementsMapping[
                                        blkHeader_TagsAndNodes.first[2]].first;
        

        einfo.eleOrder = GmshElementsMapping[
                            blkHeader_TagsAndNodes.first[2]].second;

        numFaces = GmshFacesOn3DElements[cur_type].size();
        // for each element in the entity block being read
        for (auto& tagsAndNodes : blkHeader_TagsAndNodes.second) {
            // for each face of this element
            for (int i = 0; i != numFaces; ++i) {
                std::vector<size_t> face_inds = GmshFacesOn3DElements[cur_type][i];
                numEdges = face_inds.size();
                einfo.type = (numEdges == 3) ? eTri : eQuad;
                // get the unique face (unordered) nodes pairing
                faceNodes = (face_inds.size() == 3) ? 
                            face_upairing(tagsAndNodes.second[face_inds[0]],
                                            tagsAndNodes.second[face_inds[1]],
                                            tagsAndNodes.second[face_inds[2]]) :
                            face_upairing(tagsAndNodes.second[face_inds[0]],
                                            tagsAndNodes.second[face_inds[1]],
                                            tagsAndNodes.second[face_inds[2]],
                                            tagsAndNodes.second[face_inds[3]]);

                if (s_linearElementsNodes[1].count(faceNodes)) continue;
                else {
                // face doesn't exist; create it then
                    einfo.clear(); // reset containers in face ElementInfo
                    std::vector<size_t> face_nodes(face_inds.size());
                    for (int j = 0; j != face_inds.size(); ++j)
                        face_nodes[j] = tagsAndNodes.second[face_inds[j]];
                    // find primitive edges of this face
                    priminfo.type = eLine;
                    for (int j = 0; j != numEdges; ++j) {
                        edgeNodes = edge_upairing(face_nodes[j%numEdges],
                                                face_nodes[(j+1)%numEdges]);
                        priminfo.tag = s_linearElementsNodes[0].at(
                                                                edgeNodes);
                        // since every single edge in the mesh has already been
                        // created, there's no way that .find() returns .end();
                        // if it does, a run-time error will occur because the
                        // above types don't match when .end() is returned
                        einfo.prims.push_back(s_factory->GetElement(priminfo));
                        // assert(!s_factory->Created());
                    }

                    // now find primitive face nodes
                    priminfo.type = eNode;
                    for (int j = 0; j != einfo.eleOrder-1; ++j) {
                        size_t face_node;
                        face_node = tagsAndNodes.second[numNodes
                                    + (numEdges*(einfo.eleOrder-1))
                                    + (i*(einfo.eleOrder-1))
                                    + j];
                        priminfo.tag = face_node;
                        einfo.prims.push_back(s_factory->GetElement(priminfo));
                    }
                    einfo.tag = ++s_maxTagCreated[1];
                    // Add it to the linear elements so that we can easily find
                    // it while reading 3D elements
                    s_linearElementsNodes[1].emplace(faceNodes, einfo.tag);

                    BOOST_ASSERT_MSG(s_factory->OrderElement(einfo), 
                            "Element placed in Order has already been created");

                    s_graphProc->AddToMeshInfo(einfo, rTag);
                }
            }
        }
    }
    s_factory->PlaceOrder();
}

void GmshReader::Read3DElements() {
    ElementInfo einfo, priminfo;
    int numNodes, numEdges, numFaces;
    int rTag;
    size_t faceNodes;
    // For each cached 3D entity block
    for (auto& blkHeader_TagsAndNodes : s_higherDimCache[1]) {
        rTag = -1; // reset region tag
        // Check if entity is a physical region
        if (s_physicalRegionEntities[3])
            if (s_physicalRegionEntities[3]->find(
                                    blkHeader_TagsAndNodes.first[1])
                != s_physicalRegionEntities[3]->end())
                rTag = s_physicalRegionEntities[3]->at(
                                            blkHeader_TagsAndNodes.first[1]);

        einfo.type = GmshElementsMapping[blkHeader_TagsAndNodes.first[2]].first;
        einfo.eleOrder = GmshElementsMapping[
                                    blkHeader_TagsAndNodes.first[2]].second;
        numFaces = GmshFacesOn3DElements[einfo.type].size();

        // for each element (tag + nodes)
        for (auto& tagsAndNodes : blkHeader_TagsAndNodes.second) {
            einfo.clear(); // clear containers in ElementInfo
            einfo.tag = tagsAndNodes.first;
            // get primitive faces
            for (int i = 0; i != numFaces; ++i) { // for each face
                std::vector<size_t> face_inds = GmshFacesOn3DElements[
                                                                einfo.type][i];
                priminfo.type = (face_inds.size() == 3) ? eTri : eQuad;
                faceNodes = (face_inds.size() == 3) ? 
                            face_upairing(tagsAndNodes.second[face_inds[0]],
                                            tagsAndNodes.second[face_inds[1]],
                                            tagsAndNodes.second[face_inds[2]]) :
                            face_upairing(tagsAndNodes.second[face_inds[0]],
                                            tagsAndNodes.second[face_inds[1]],
                                            tagsAndNodes.second[face_inds[2]],
                                            tagsAndNodes.second[face_inds[3]]);

                priminfo.tag = s_linearElementsNodes[1].at(faceNodes);

                einfo.prims.push_back(s_factory->GetElement(priminfo));
                // assert(!s_factory->Created());
            }
            // get interior (volume) nodes
            priminfo.type = eNode;
            for (int j = 0; j != einfo.eleOrder-1; ++j) {
                // if (!s_danglingNodes.erase(*lastNodes)) break;
                priminfo.tag = tagsAndNodes.second[numNodes
                                + numEdges*(einfo.eleOrder-1)
                                + numFaces*(einfo.eleOrder-1)
                                + j];
                einfo.prims.push_back(s_factory->GetElement(priminfo));
            }
            // finally order element
            BOOST_ASSERT_MSG(s_factory->OrderElement(einfo), 
                            "Element placed in Order has already been created");

            s_graphProc->AddToMeshInfo(einfo, rTag);
        }
    }
    s_factory->PlaceOrder();
}

std::unique_ptr<Section> GmshReader::GetSectionObj(std::ifstream& file) {
    if (!s_sectionReader.get()) {
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
                return std::unique_ptr<Section>(
                                            new GmshASCIISection(s_meshFile));

            case 1:
                return std::unique_ptr<Section>(
                                            new GmshBinarySection(s_meshFile));

            default:
                std::cout << "Unrecognized Gmsh file format.\n";
                exit(-1);
        }
    }
    else
        // mode needed because copy is not allowed
        return std::move(s_sectionReader); 
}

GmshReader::~GmshReader() {

    for (auto& pm : s_physicalRegionEntities)
        if (pm) delete pm;
}

// DELETE THIS
// FUNCTION FOR PURPOSES OF DEBUGGING ONLY
void GmshReader::ShoutOutAll() {
    // std::cout << "Number of dangling Nodes: " << s_danglingNodes.size();
    std::cout << "\nNumber of Edges read: " << s_factory->HowMany(eLine);
    std::cout << "\nNumber of 2D Elements read: " << s_factory->HowMany(eQuad);
    std::cout << "\nNumber of cached 2D Elements blocks: " << s_higherDimCache[0].size();
    if (s_meshDim == 3)
        std::cout << "\nNumber of cached 3D Elements blocks: " << s_higherDimCache[1].size();
    std::cout << std::endl;
}

// ---------- End Of GmshReader Member Function Definitions --------- //

// ---------- Section Member Function Definitions --------- //

Section::Section(std::ifstream& f) : s_iFile(f), s_curSectionMarkers(2),
                s_headerDone(false), s_sectionDone(false) {

}

Section::~Section() {

}

// ---------- End Of Section Member Function Definitions --------- //


// ---------- GmshSection Member Function Definitions --------- //

GmshSection::GmshSection(std::ifstream& f) : Section(f), 
                                        s_lastEntityBlockRead(0) {

}

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

    return this;
}

void GmshSection::CheckNext(const std::string& sec) {

    BOOST_ASSERT_MSG(s_headerDone, 
        "No header has been read for this section. Can't proceed to Next");

    BOOST_ASSERT_MSG(!s_sectionDone, 
        "End of Section has been reached. Can't read Next.");

    BOOST_ASSERT_MSG((s_name == sec) && s_curSectionMarkers[1],
        "Markers in wrong section or nonexistent. "
        "Use GoToSection(...) fisrt.\n");

    BOOST_ASSERT_MSG((s_iFile.tellg() > s_lastEntityBlockRead)
                    || (s_iFile.tellg() < s_curSectionMarkers[1]), 
                    "Marker for reading Next entity block is out of place");

}

GmshSection::~GmshSection() {

}

// ---------- End Of GmshSection Member Function Definitions --------- //

// ---------- GmshASCIISection Member Function Definitions --------- //

GmshASCIISection::GmshASCIISection(std::ifstream& f) : GmshSection(f) {

}

std::vector<int> GmshASCIISection::ReadSectionHeader() {
    BOOST_ASSERT_MSG(!s_name.empty() && s_curSectionMarkers[1], 
        "No Section to read. Use GoToSection(...) fisrt");

    std::vector<int> ret;
    s_iFile.seekg(s_curSectionMarkers[0]); // Position marker at section header
    char ch;
    int itmp;
    while (s_iFile >> itmp) {
        ret.push_back(itmp);
        while (s_iFile.get(ch))
            if (ch != ' ') { s_iFile.unget(); break; }
        if (s_iFile.peek() == '\n') { s_iFile.get(ch); break; }
    }
    s_lastEntityBlockRead = s_iFile.tellg(); // set last block read to init.

    s_headerDone = true;
    s_sectionDone = false;

    return ret;
}

// Next for reading Nodes entity blocks
std::vector<int> GmshASCIISection::Next(std::unordered_map<size_t, 
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

    BOOST_ASSERT_MSG(vinfo.size() == 4, 
        "Invalid Entity Block header for reading Nodes");
    
    std::string stemp;
    double ctemp;
    tagCoords.clear();

    for (int i = 0; i != vinfo.back(); ++i) {
        std::getline(s_iFile, stemp);
        
        BOOST_ASSERT_MSG(stoi(stemp) > 0, "Invalid negative tag for a Node");
        
        tagCoords.emplace(stoul(stemp), std::vector<double>());
    }

    for (auto& tagCoords_pair : tagCoords) {
        while(s_iFile >> ctemp) {
            tagCoords_pair.second.push_back(ctemp);
            while (s_iFile.get(c))
                if (c != ' ') { s_iFile.unget(); break; }
            if (s_iFile.peek() == '\n') { s_iFile.get(c); break; }
        }
        BOOST_ASSERT_MSG(tagCoords_pair.second.size() == 3,
                            "Invalid coordinates given for node");
    }

    s_lastEntityBlockRead = s_iFile.tellg();
    if (s_lastEntityBlockRead >= s_curSectionMarkers[1]) {
        s_curSectionMarkers[0] = s_curSectionMarkers[1] = 0;
        s_sectionDone = true;
        s_name.clear();
    }

    return vinfo;
}

// Next for reading Elements (tag, vector of nodes it comprises) entity blocks
std::vector<int> GmshASCIISection::Next(std::unordered_map<size_t, 
                                        std::vector<size_t>>& tagNodes) {
    CheckNext("Elements");

    std::vector<int> vinfo;
    char c;
    int itmp;
    size_t stmp;
    while (s_iFile >> itmp) {
        vinfo.push_back(itmp);
        while (s_iFile.get(c))
            if (c != ' ') { s_iFile.unget(); break; }
        if (s_iFile.peek() == '\n') { s_iFile.get(c); break; }
    }
    BOOST_ASSERT_MSG(vinfo.size() == 4, 
                    "Invalid Entity Block header for reading Elements");

    std::string garbage;
    tagNodes.clear();
    for (int i = 0; i != vinfo.back(); ++i) {
        if (vinfo.front()) {
            // Register tag key into the map with an empty vector of nodes
            BOOST_ASSERT_MSG((s_iFile >> itmp) && (itmp > 0), 
                                "Invalid element tag in entity");
            tagNodes.emplace(static_cast<size_t>(itmp), std::vector<size_t>());
            
            // Now run through the nodes list
            stmp = static_cast<size_t>(itmp);
            auto& vec = tagNodes.at(stmp);
            while (s_iFile >> stmp) {
                vec.push_back(stmp);
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
    BOOST_ASSERT_MSG(vi.size() == 2, 
                    "Invalid definition of Physical Boundaries");
    s_iFile.clear();
    s_iFile.get(ch);
    BOOST_ASSERT_MSG(ch == '\"', "Invalid definition of Physical Boundaries");
    
    BOOST_ASSERT_MSG(std::getline(s_iFile, name, '\"'),
                        "Invalid definition of Physical Boundaries");
    
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
            BOOST_ASSERT_MSG(tvd.size() >= 5, 
                            "Invalid Point entity definition");
            if (tvd[4])
                bndEnts = std::make_pair(static_cast<int>(tvd[0]), 
                            static_cast<int>(tvd[5]));
            else
                bndEnts = std::make_pair(static_cast<int>(tvd.front()),
                                         -1);

            break;

        case 1: // Curve entity
            BOOST_ASSERT_MSG(tvd.size() >= 11, 
                            "Invalid Curve entity definition");
            if (tvd[7])
                bndEnts = std::make_pair(static_cast<int>(tvd[0]), 
                            static_cast<int>(tvd[8]));
            else
                bndEnts = std::make_pair(static_cast<int>(tvd.front()),
                                         -1);
            
            break;
            

        case 2: // Surface entity
            BOOST_ASSERT_MSG(tvd.size() >= 10, 
                            "Invalid Surface entity definition");
            if (tvd[7])
                bndEnts = std::make_pair(static_cast<int>(tvd[0]), 
                            static_cast<int>(tvd[8]));
            else
                bndEnts = std::make_pair(static_cast<int>(tvd.front()),
                                         -1);

            break;

        case 3: // Volume entity
            BOOST_ASSERT_MSG(tvd.size() >= 10, 
                            "Invalid Volume entity definition");
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

GmshASCIISection::~GmshASCIISection() {

}

// ---------- End of GmshASCIISection Member Function Definitions --------- //

// ---------- GmshBinarySection Member Function Definitions --------- //

std::vector<int> GmshBinarySection::ReadSectionHeader() {

}

// Next for reading Nodes entity blocks
std::vector<int> GmshBinarySection::Next(std::unordered_map<size_t, 
                                        std::vector<double>>& tagCoords) {

}


// Next for reading Elements (tag, vector of nodes it comprises) entity blocks
std::vector<int> GmshBinarySection::Next(std::unordered_map<size_t, 
                                        std::vector<size_t>>& tagNodes) {

}

// Next for reading Boundaries (entity dim. -- curve/suface, region tag, name)
std::vector<int> GmshBinarySection::Next(std::tuple<int, int, 
                                        std::string>& bndRegion) {

}

// Next for reading Entities (entity tag and physical boundary tag)
std::vector<int> GmshBinarySection::Next(std::pair<int, int>& bndEnts, 
                                        int entDim) {

}

GmshBinarySection::~GmshBinarySection() {

}


// ---------- End of GmshBinarySection Member Function Definitions --------- //

} // end of CHONS namespace