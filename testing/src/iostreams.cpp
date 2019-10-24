#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <map>
#include <algorithm>
#include <cctype>
#include <tuple>


void Next(std::tuple<int, int, std::string>& bndRegion, std::ifstream& s_iFile) {
    

    std::vector<int> vi;
    int tmp, cnt=0;
    std::string name;
    char ch;

    while (s_iFile >> tmp) {
        ++cnt;
        vi.push_back(tmp);
    }
    if (vi.size() != 2) {
        std::cerr << "2Invalid definition of Physical Boundaries.\n";
        exit(-1);
    }
    s_iFile.clear();
    s_iFile.get(ch);
    if (ch != '\"') {
        std::cerr << ch << "2Invalid definition of Physical Boundaries.\n";
        exit(-1);
    }
    if (!(std::getline(s_iFile, name, '\"'))) {
        std::cerr << "3Invalid definition of Physical Boundaries.\n";
        exit(-1);
    }
    bndRegion = std::make_tuple(vi[0], vi[1], name);

}

int main() {

    std::vector<std::map<int, int>*> vt{4};
    vt[0] = new std::map<int, int>;
    vt[0]->insert(std::make_pair(2,4));
    std::cout << vt[0]->at(2) << ",\n";


    std::ifstream msh("../mesh/2d_quad.msh");
    if (!msh.is_open()) {
        std::cout << "How unfortunate!\n";
        exit(-1);
    }
    
    std::string section;
    char ch;
    msh.seekg(0, msh.beg);
    msh.get(ch);
    
    std::getline(msh, section);
    std::getline(msh, section);

    std::string::size_type where;
    double s_mshVersion = stod(section, &where);
    int s_mshFormat = stoi(section.substr(where));
    std::cout << "Version: " << s_mshVersion << "\nFormat: " << s_mshFormat
                << std::endl;

    std::vector<std::ifstream::pos_type> vp(2);
    std::string sec = "PhysicalNames";

    while (std::getline(msh, section)) {
        if (section[0] == '$') {
            if (section.substr(1) == sec) {
                vp[0] = msh.tellg();
            }
            if (section.substr(1) == ("End" + sec)) {
                msh.seekg(-(sec.size()+5), msh.cur);
                vp[1] = msh.tellg();
                break;
            }
        }
    }
    if (msh.eof())
        std::cout << "Couldn't find section $" << sec << "\n";

    // while (std::getline(msh, section));
    // if (msh.eof())
    //     std::cout << "End of file marker: " << msh.tellg() << "\n";

    std::cout << "SecBegin marker: " << vp[0] << "\nSecEnd marker: " << vp[1] << std::endl;
    msh.seekg(vp[0]);
    std::getline(msh, sec);
    std::cout << "First line of Section: " << sec << "\n";
    msh.seekg(vp[1]);
    std::getline(msh, sec);
    std::cout << "One past the last line of Section: " << sec << "\n";



    std::vector<int> vinfo;
    msh.seekg(vp[0]);
    //std::getline(msh, sec);
    char c;
    int itmp;
    while (msh >> itmp) {
        vinfo.push_back(itmp);
        while (msh.get(c))
            if (c != ' ') { msh.unget(); break; }
        if (msh.peek() == '\n') { msh.get(c); break; }
    }
    for (auto e : vinfo) std::cout << e << " ";
        std::cout << std::endl;
    // if (vinfo.size() != 4) {
    //     std::cout << "Invalid Nodes entity block header.\n";
    //     exit(-1);
    // }

    std::tuple<int, int, std::string> tup;
    for (int i = 0; i != vinfo.front(); ++i) {
        Next(tup, msh);
        std::cout << "Dim: " << std::get<0>(tup) << "\n";
        std::cout << "Tag: " << std::get<1>(tup) << "\n";
        std::cout << "Name: " << std::get<2>(tup) << "\n";
        std::cout << "----\n";
    }


}
