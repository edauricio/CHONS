#include "MeshModule/GraphInfoProcessor.h"

namespace CHONS {

GraphInfoProcessor::GraphInfoProcessor(const short int& d) : s_meshDim(d),
                        s_meshInfo(MeshInfoHolder::GetInstance()) {

}
// MARKED TO BE REFACTORED IN ITERATION 2:
// Since in this iteration (2) a native mesh file will be created instead
// of reading directly from gmsh/cgns, we will know in advance how many elements
// (nodes, lines, 2D/3D) will be created, so we can call for memory allocation
// in the beginning of the section reading. Hence, no more "OrderElement" and
// "PlaceOrder"thing will be needed and elements will be constructed in the 
// previsouly allocated memory right after reading it. Hence, the first parameter
// to Add...Element() could be the Element* itself, passed by requesting it
// to s_factory->GetElement();
void GraphInfoProcessor::AddToMeshInfo(const ElementInfo& 
                                        ein, const int& rTag) {
    short int type_dim = ein.type;
    type_dim = (type_dim == 3) ? 2 : ((type_dim > 3) ? 3 : type_dim);
    if (type_dim == s_meshDim-1) {
        if (rTag > 0)
            /*add boundary element*/
            s_meshInfo->AddBoundaryElement(ein, rTag);
    } else if (type_dim == s_meshDim) {
        /*add interface element*/
        for (auto& prim : ein.prims) {
            if (s_tmpInt.count(prim->GetTag())) {
                auto it = s_tmpInt.find(prim->GetTag());
                it->second.push_back(std::make_pair(ein.type, ein.tag));
                s_meshInfo->AddInterfaceElement(prim->GetTag(),
                            it->second[0],
                            it->second[1]);
            }
            else
                s_tmpInt.emplace(prim->GetTag(), std::vector<
                                    std::pair<ElementType, size_t>>{
                                    std::make_pair(ein.type, ein.tag)});
        }
        /*add interior element*/
        s_meshInfo->AddInteriorElement(ein, rTag);
    }
}

} // end of CHONS namespace