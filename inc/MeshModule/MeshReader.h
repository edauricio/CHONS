// MeshReader class and its composed classes.

#ifndef MESHREADER_H
#define MESHREADER_H

#include <string>
#include <fstream>
#include <vector>

namespace CHONS {

class Section;

// TODO
// This class must set the dimension of the problem (inferred by the mesh);
// this info is passed to another object, the one holding the configuration
class MeshReader {
    public:
        MeshReader(std::string);
        virtual ~MeshReader();
        MeshReader(const MeshReader&) = delete;
        MeshReader& operator=(const MeshReader&) = delete;

        virtual void ReadNodes() = 0;
        virtual void ReadEdges() = 0;
        virtual void ReadElements() = 0;

    protected:
        virtual Section* GetSection(const std::ifstream&) { return nullptr; };

        std::ifstream s_meshFile;
        Section* s_sectionReader;

    private:
       // std::string checkExtension(std::string);
};

class GmshReader : public MeshReader {
    public:
        GmshReader(std::string);
        virtual ~GmshReader();

        virtual void ReadNodes();
        virtual void ReadEdges();
        virtual void ReadElements();

    protected:
        virtual Section* GetSection(const std::ifstream&) override;


};

class Section {
    public:
        Section(const std::ifstream&);

        virtual std::string Next() = 0;
        virtual void ReadSection(std::string) = 0;
        virtual std::string GetName() = 0;
        virtual std::vector<double> begin() = 0;
        virtual std::vector<double> end() = 0;

    private:

};

class GmshSection : public Section {
    public:
        GmshSection(const std::ifstream&);

        virtual std::string Next();
        virtual void ReadSection(std::string);
        virtual std::string GetName();
        virtual std::vector<double> begin();
        virtual std::vector<double> end();
};

} // end of CHONS namespace

#endif