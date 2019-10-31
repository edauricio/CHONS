// MeshReader class and its composed classes.

#ifndef MESHREADER_H
#define MESHREADER_H

#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <tuple>
#include "ElementFactory.h"

namespace CHONS {

class Section;

// TODO
// This class must set the dimension of the problem (inferred by the mesh);
// this info is passed to another object, the one holding the configuration
class MeshReader {
    public:
        MeshReader(const std::string&);
        virtual ~MeshReader();
        MeshReader(const MeshReader&) = delete;
        MeshReader& operator=(const MeshReader&) = delete;

        virtual void ReadNodes() = 0;
        virtual void ReadEdges() = 0;
        virtual void ReadElements() = 0;
        
        int GetMeshDim() { return s_meshDim; };
        double GetMeshVersion() { return s_meshVersion; };
        int GetMeshFormat() { return s_meshFormat; };

    protected:
        virtual Section* GetSectionObj(std::ifstream&) = 0;
        virtual void ReadBoundaries() = 0;
        virtual void ReadMeshDim() = 0;

        std::string s_fName;
        std::ifstream s_meshFile;
        Section* s_sectionReader;

        // Variable to map a physical boundary tag to its name, in order to be
        // able to match and set it to the config. file (user input)
        std::map<int, std::string> s_physicalRegionNames;

        // Vector of 4 pointers to maps, each containing the entities and
        // the physical region which it belongs.
        // i.e. vec[0] contains the 0-dim. entities (points tag, map_key) that 
        // are in a physical boundary (mapped_value), vec[1] contains the 1-dim 
        // entities (curves tags, map_key) and so on.
        std::vector<std::map<int, int>*> s_physicalRegionEntities;

        // A set containing all the nodes added to the mesh during ReadNodes().
        // After that, each subsequent higher dimensional read erases the
        // corresponding element nodes from this list, so the ones who persists
        // in the list are those possible inside nodes of higher dimensional
        // elements (faces and volumes). This is done in order to speed up
        // the check for last nodes on the list describing 2D/3D elements, to 
        // see if they are internal (face/volume) nodes. Otherwise, a loop would
        // be needed for each primitive (edge) or primitive of primitive (edges
        // of faces in 3D) checking for each of its nodes to compare with the
        // node given in t he element definition so we can have the info about
        // whether its a primitive node or a interior one.
        std::set<size_t> s_danglingNodes;

        // Vector to maps of 1D and 2D elements, each containing a list with
        // the nodes that define its linear element so that a higher-dimensional
        // element can check for its primitive quick and efficiently, given the
        // first nodes in the mesh file
        std::vector<std::map<std::set<size_t>, size_t>> s_linearElementsNodes;

        // This is a vector to maps of tags and nodes defining 2D and 3D elements.
        // It is populated by ReadEdges function while reading the 1D elements (
        // edges). It is a cache-like vector to avoid reading the whole Elements
        // section two (three) times for 2D (3D) meshes, which would be very costly
        // for large meshes. Since ReadEdges reads the whole section anyways, it is
        // wise to write down the elements read but not (yet) processed. This
        // vector also make it easy to read all 2D elements before the 3D ones,
        // since they will be stored in separate maps.
        // Another approach would be to make ReadEdges skip entity blocks other
        // than that containing 1D elements; however, since the responsibility
        // for reading this info is on another object (Section), this approach
        // is trickier.
        // PAIR: 
        // first - block header
        // second - map from Element tag to a vector of Nodes defining it.
        std::vector<std::map<std::vector<int>, std::map<size_t, 
                            std::vector<size_t> > > > s_higherDimCache;

        // A vector containing the tag of the latest edge / face created
        // This is needed so whenever higher dimensional elements create its
        // primitives (i.e. primitives in entity with dimension higher than 
        // itself), they know which tag to add.
        std::vector<size_t> s_lastTagCreated;

        
        double s_meshVersion;
        int s_meshFormat; // bin / ascii
        int s_meshDim;
        bool s_nodesDone, s_edgesDone;
        ElementFactory* s_factory;

    private:
       // std::string checkExtension(std::string);
};

class GmshReader : public MeshReader {
    public:
        GmshReader(const std::string&);
        virtual ~GmshReader();

        virtual void ReadNodes() override;
        virtual void ReadEdges() override;
        virtual void ReadElements() override;

                void ShoutOutAll(); // DELETE THIS --- FOR DEBUGGING PURPOSES ONLY


    protected:
        virtual Section* GetSectionObj(std::ifstream&) override;
        virtual void ReadMeshDim() override;
        virtual void ReadBoundaries() override;


    private:
        void Read2DElements();
        void Read3DElements();


};




class Section {
    public:

        Section(std::ifstream& f);

        //virtual void ReadFirst() = 0;

        // Next for Nodes
        virtual std::vector<int> Next(std::map<size_t, 
                                        std::vector<double>>&) = 0;

        // Next for Elements (tag, vector of nodes comprising it, element type)
        virtual std::vector<int> Next(std::map<size_t, 
                                    std::vector<size_t>>&) = 0;

        // Next for Boundaries (entity dimension -- curve/surface, region, name)
        virtual std::vector<int> Next(std::tuple<int, int, std::string>&) = 0;

        // Next for Entities/Boundaries relationship (which entity belongs to
        // which boundary tag)
        virtual std::vector<int> Next(std::pair<int, int>&, int) = 0;

        virtual Section* GoToSection(const std::string&) = 0;
        virtual std::vector<int> ReadSectionHeader() = 0;
        virtual std::string GetName() = 0;
        // virtual size_t GetTotalSecItems() = 0;
        // virtual size_t GetTotalEntityBlocks() = 0;

        virtual ~Section();

    protected:
        std::ifstream& s_iFile;
        std::vector<std::ifstream::pos_type> s_curSectionMarkers;
        bool s_headerDone;
        bool s_sectionDone;

};

class GmshSection : public Section {
    public:
        GmshSection(std::ifstream& f);

        //virtual void ReadFirst() = 0;
        // Next for Nodes
        virtual std::vector<int> Next(std::map<size_t, 
                                        std::vector<double>>&) = 0;

        // Next for Elements (tag, vector of nodes comprising it, element type)
        virtual std::vector<int> Next(std::map<size_t, 
                                    std::vector<size_t>>&) = 0;

        // Next for Boundaries (entity dimension -- curve/surface, region, name)
        virtual std::vector<int> Next(std::tuple<int, int, std::string>&) = 0;

        // Next for Entities/Boundaries relationship (which entity belongs to
        // which boundary tag)
        virtual std::vector<int> Next(std::pair<int, int>&, int) = 0;

        virtual Section* GoToSection(const std::string&) override;
        virtual std::vector<int> ReadSectionHeader() = 0;
        virtual std::string GetName() { return s_name; };
        // size_t GetTotalSecItems() { return s_numSectionItemsTotal; };
        // size_t GetTotalEntityBlocks() { return s_numEntityBlocksInSection; };

        virtual ~GmshSection();
        
    protected:
        void CheckNext(const std::string&);


        std::string s_name;
        std::ifstream::pos_type s_lastEntityBlockRead;
};

class GmshASCIISection : public GmshSection {
    public:
        GmshASCIISection(std::ifstream& f);


        //virtual void ReadFirst() override;
        // Next for Nodes
        virtual std::vector<int> Next(std::map<size_t, 
                                        std::vector<double>>&) override;

        // Next for Elements (tag, vector of nodes comprising it, element type)
        virtual std::vector<int> Next(std::map<size_t, 
                                    std::vector<size_t>>&) override;

        // Next for Boundaries (entity dimension -- curve/surface, region, name)
        virtual std::vector<int> Next(std::tuple<int, int, std::string>&) override;

        // Next for Entities/Boundaries relationship (which entity belongs to
        // which boundary tag)
        virtual std::vector<int> Next(std::pair<int, int>&, int) override;

        virtual std::vector<int> ReadSectionHeader() override;

        virtual ~GmshASCIISection();

    protected:


    private:
        
};

class GmshBinarySection : public GmshSection {
    public:
        GmshBinarySection(std::ifstream& f) : GmshSection(f) {};

        //virtual void ReadFirst() override;
        // Next for Nodes
        virtual std::vector<int> Next(std::map<size_t, 
                                        std::vector<double>>&) override;

        // Next for Elements (tag, vector of nodes comprising it, element type)
        virtual std::vector<int> Next(std::map<size_t, 
                                    std::vector<size_t>>&) override;

        // Next for Boundaries (entity dimension -- curve/surface, region, name)
        virtual std::vector<int> Next(std::tuple<int, int, std::string>&) override;

        // Next for Entities/Boundaries relationship (which entity belongs to
        // which boundary tag)
        virtual std::vector<int> Next(std::pair<int, int>&, int) override;
        virtual std::vector<int> ReadSectionHeader() override;

        virtual ~GmshBinarySection();
};

} // end of CHONS namespace

#endif