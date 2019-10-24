// MeshReader class and its composed classes.

#ifndef MESHREADER_H
#define MESHREADER_H

#include <string>
#include <fstream>
#include <vector>
#include <map>
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
        
        int GetMeshDim() { return s_dim; };
        double GetMeshVersion() { return s_meshVersion; };
        int GetMeshFormat() { return s_meshFormat; };

    protected:
        virtual Section* GetSectionObj(std::ifstream&) { return nullptr; };
        virtual void ReadBoundaries() = 0;
        virtual void ReadMeshDim() {};

        std::string s_fName;
        std::ifstream s_meshFile;
        Section* s_sectionReader;

        // Variable to map a physical boundary tag to its name, in order to be
        // able to match and set it to the config. file (user input)
        std::map<int, std::string> s_boundaryRegionNames;

        // Vector of 4 pointers to maps, each containing the entities and
        // the physical region which it belongs.
        // i.e. vec[0] contains the 0-dim. entities (points tag, map_key) that 
        // are in a physical boundary (mapped_value), vec[1] contains the 1-dim 
        // entities (curves tags, map_key) and so on.
        std::vector<std::map<int, int>*> s_boundaryRegionEntities;
        
        double s_meshVersion; 
        int s_meshFormat; // bin / ascii
        int s_dim;
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

    protected:
        virtual Section* GetSectionObj(std::ifstream&) override;
        virtual void ReadMeshDim() override;
        virtual void ReadBoundaries() override;


    private:



};




class Section {
    public:

        Section(std::ifstream& f) : s_iFile(f), s_curSectionMarkers(2) {};

        virtual void ReadFirst() = 0;

        // Next for Nodes
        virtual Section* Next(std::map<size_t, std::vector<double>>&) = 0;

        // Next for Elements (tag, vector of nodes comprising it, element type)
        virtual Section* Next(std::map<size_t, std::vector<double>>&, int&) = 0;

        // Next for Boundaries (entity dimension -- curve/surface, region, name)
        virtual Section* Next(std::tuple<int, int, std::string>&) = 0;

        // Next for Entities/Boundaries relationship (which entity belongs to
        // which boundary tag)
        virtual Section* Next(std::pair<int, int>&, int) = 0;

        virtual Section* GoToSection(const std::string&) = 0;
        virtual std::vector<int> ReadSectionHeader() = 0;
        virtual std::string GetName() = 0;
        // virtual size_t GetTotalSecItems() = 0;
        // virtual size_t GetTotalEntityBlocks() = 0;

        virtual ~Section();

    protected:
        std::ifstream& s_iFile;
        std::vector<std::ifstream::pos_type> s_curSectionMarkers;
        bool s_headerDone = false;
        bool s_sectionDone = false;

};

class GmshSection : public Section {
    public:
        GmshSection(std::ifstream& f) : Section(f) {};

        virtual void ReadFirst() = 0;
        // Next for Nodes
        virtual Section* Next(std::map<size_t, std::vector<double>>&) = 0;

        // Next for Elements (tag, vector of nodes comprising it, element type)
        virtual Section* Next(std::map<size_t, std::vector<double>>&, int&) = 0;

        // Next for Boundaries (entity dimension -- curve/surface, region, name)
        virtual Section* Next(std::tuple<int, int, std::string>&) = 0;

        // Next for Entities/Boundaries relationship (which entity belongs to
        // which boundary tag)
        virtual Section* Next(std::pair<int, int>&, int) = 0;

        virtual Section* GoToSection(const std::string&) override;
        virtual std::vector<int> ReadSectionHeader() = 0;
        virtual std::string GetName() { return s_name; };
        // size_t GetTotalSecItems() { return s_numSectionItemsTotal; };
        // size_t GetTotalEntityBlocks() { return s_numEntityBlocksInSection; };

        virtual ~GmshSection();
        
    protected:
        void CheckNext(const std::string&);


        std::string s_name;
        std::ifstream::pos_type s_lastEntityBlockRead = 0;
};

class GmshASCIISection : public GmshSection {
    public:
        GmshASCIISection(std::ifstream& f) : GmshSection(f) {} ;


        virtual void ReadFirst() override;
        // Next for Nodes
        virtual Section* Next(std::map<size_t, std::vector<double>>&) override;

        // Next for Elements (tag, vector of nodes comprising it, element type)
        virtual Section* Next(std::map<size_t, std::vector<double>>&, 
                                int&) override;

        // Next for Boundaries (entity dimension -- curve/surface, region, name)
        virtual Section* Next(std::tuple<int, int, 
                                std::string>&) override;

        // Next for Entities/Boundaries relationship (which entity belongs to
        // which boundary tag)
        virtual Section* Next(std::pair<int, int>&, int) override;

        virtual std::vector<int> ReadSectionHeader() override;

        virtual ~GmshASCIISection();

    protected:


    private:
        
};

class GmshBinarySection : public GmshSection {
    public:
        GmshBinarySection(std::ifstream& f) : GmshSection(f) {};

        virtual void ReadFirst() override;
        virtual Section* Next(std::map<size_t, std::vector<double>>&) override;

        // Next for Elements (tag, vector of nodes comprising it, element type)
        virtual Section* Next(std::map<size_t, std::vector<double>>&, 
                                int&) override;

        // Next for Boundaries (entity dimension -- curve/surface, region, name)
        virtual Section* Next(std::tuple<int, int, 
                                std::string>&) override;

        // Next for Entities/Boundaries relationship (which entity belongs to
        // which boundary tag)
        virtual Section* Next(std::pair<int, int>&, int) override;
        virtual std::vector<int> ReadSectionHeader() override;

        virtual ~GmshBinarySection();
};

} // end of CHONS namespace

#endif