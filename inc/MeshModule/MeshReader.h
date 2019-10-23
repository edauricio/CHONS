// MeshReader class and its composed classes.

#ifndef MESHREADER_H
#define MESHREADER_H

#include <string>
#include <fstream>
#include <vector>
#include <map>
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
        virtual void ReadBoundaries() = 0;
        double GetMeshVersion() { return s_mshVersion; };
        int GetMeshFormat() { return s_mshFormat; };

    protected:
        virtual Section* GetSectionObj(std::ifstream&) = 0;
        virtual size_t GetMeshDim() = 0;

        std::string s_fName;
        std::ifstream s_meshFile;
        Section* s_sectionReader;
        double s_mshVersion; 
        int s_mshFormat; // bin / ascii
        size_t s_Dim = 0;
        //ElementFactory* s_factory; -- singleton static (global) class...

    private:
       // std::string checkExtension(std::string);
};

class GmshReader : public MeshReader {
    public:
        GmshReader(const std::string&);
        virtual ~GmshReader();

        virtual void ReadNodes();
        virtual void ReadEdges();
        virtual void ReadElements();

    protected:
        virtual Section* GetSectionObj(std::ifstream&) override;
        virtual size_t GetMeshDim() override;

    private:



};




class Section {
    public:

        Section(std::ifstream& f) : s_iFile(f) {};

        virtual void ReadFirst() = 0;
        virtual void Next(std::map<int, std::vector<double>>) = 0;
        virtual void Next(std::map<int, std::vector<double>>, int) = 0;
        virtual Section* GoToSection(const std::string&) = 0;
        virtual bool ReadSectionHeader() = 0;
        virtual std::string GetName() = 0;
        virtual size_t GetTotalSecEntities() = 0;
        virtual size_t GetTotalEntityBlocks() = 0;


    protected:
        std::ifstream& s_iFile;
        std::vector<std::ifstream::pos_type> s_curSectionMarkers(2);

        

};

class GmshSection : public Section {
    public:
        GmshSection(std::ifstream& f) : Section(f) {};

        virtual void ReadFirst() = 0;
        virtual void Next(std::map<int, std::vector<double>>) = 0;
        virtual void Next(std::map<int, std::vector<double>>, int) = 0;
        virtual Section* GoToSection(const std::string&) override;
        virtual bool ReadSectionHeader() = 0;
        virtual std::string GetName() { return s_name; };
        size_t GetTotalSecItems() { return s_numSectionItemsTotal; };
        size_t GetTotalEntityBlocks() { return s_numEntityBlocksInSection; };
        
    protected:


        std::string s_name;
        size_t s_numEntityBlocksInSection;
        size_t s_numSectionItemsTotal;
        size_t s_numSectionEntitiesInCurBlock;
        size_t s_minTag;
        size_t s_maxTag;
        std::map<size_t, 
                    std::vector<std::ifstream::pos_type>(2)> s_tagMarkerMap;
        std::ifstream::pos_type s_firstEntityBlock = 0;
        std::ifstream::pos_type s_lastEntityBlockRead = 0;
};

class GmshASCIISection : public GmshSection {
    public:
        GmshASCIISection(std::ifstream& f) : GmshSection(f) {} ;


        virtual void ReadFirst();
        virtual void Next(std::map<int, std::vector<double>>) override;
        virtual void Next(std::map<int, std::vector<double>>, int) override;
        virtual bool ReadSectionHeader() override;
        virtual std::vector<double> begin();
        virtual std::vector<double> end();

    protected:


    private:
        
};

} // end of CHONS namespace

#endif