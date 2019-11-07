// Class to process the information given by MeshReader, putting it in a right
// format to 'push' to MeshInfoHolder class

#include "ElementEnumInfo.h"
#include "MeshInfoHolder.h"
#include <unordered_map>
#include <vector>

namespace CHONS {

class GraphInfoProcessor {
    public:
        GraphInfoProcessor(const short int& d);
        void AddToMeshInfo(const ElementInfo&, const int&);
        void Done() { s_meshInfo->Consolidate(); }

    private:
        short int s_meshDim;
        std::unordered_map<size_t, 
                        std::vector<std::pair<ElementType, size_t>>> s_tmpInt;
        MeshInfoHolder* s_meshInfo;
};

} // end of CHONS namespace