#include "MeshModule/MeshInfoHolder.h"
#include <iostream>

namespace CHONS {

MeshInfoHolder* MeshInfoHolder::s_soleInstance = nullptr;

MeshInfoHolder* MeshInfoHolder::GetInstance() {
    if (!s_soleInstance)
        s_soleInstance = new MeshInfoHolder;
    return s_soleInstance;
}

void MeshInfoHolder::AddBoundaryElement(const ElementInfo& ein, 
                                        const int& rTag) {
    s_tmpBndElements[ein.type].emplace(rTag, ein.tag);
}

void MeshInfoHolder::AddInterfaceElement(const size_t& tag, 
                                const std::pair<ElementType, size_t>& ele1, 
                                const std::pair<ElementType, size_t>& ele2) {
    s_tmpBndToSharings.emplace(tag, std::vector<
                        std::pair<ElementType, size_t>>{ele1, ele2});
}

void MeshInfoHolder::AddInteriorElement(const ElementInfo& ein, 
                                            const int& rTag) {
        s_tmpIntElements[ein.type].emplace(rTag, ein.tag);
}

void MeshInfoHolder::Consolidate() {
    ElementInfo einfo;
    for (int i = 0; i != MAX_NUMBER_OF_TYPES; ++i) {
        if (!s_tmpBndElements[i].empty()) {
            einfo.type = static_cast<ElementType>(i);
            for (auto bcnt = 0; bcnt != s_tmpBndElements[i].bucket_count();
                        ++bcnt)
                for (auto bckt_it = s_tmpBndElements[i].begin(bcnt);
                            bckt_it != s_tmpBndElements[i].end(bcnt); bckt_it++) {
                    einfo.tag = bckt_it->second;
                    s_boundaryElements.emplace(bckt_it->first,
                                        s_factory->GetElement(einfo));
                }
            s_tmpBndElements[i].clear();
        }

        if (!s_tmpIntElements[i].empty()) {
            einfo.type = static_cast<ElementType>(i);
            for (auto bcnt = 0; bcnt != s_tmpIntElements[i].bucket_count();
                        ++bcnt)
                for (auto bckt_it = s_tmpIntElements[i].begin(bcnt);
                            bckt_it != s_tmpIntElements[i].end(bcnt); bckt_it++) {
                    einfo.tag = bckt_it->second;
                    s_interiorElements.emplace(bckt_it->first,
                                        s_factory->GetElement(einfo));
                }
            s_tmpBndElements[i].clear();
        }
    }

    for (auto& tmpInter : s_tmpBndToSharings) {
        std::array<Element*, 2> ar;
        ElementInfo einfo;
        einfo.type = tmpInter.second[0].first;
        einfo.tag = tmpInter.second[0].second;
        ar[0] = s_factory->GetElement(einfo);
        ar[1] = nullptr; // Create a Ghost element to hold boundary cond. info?
        if (tmpInter.second.size() > 1) {
            einfo.type = tmpInter.second[1].first;
            einfo.tag = tmpInter.second[1].second;
            ar[1] = s_factory->GetElement(einfo);
        }
        s_interfaceElements.emplace(tmpInter.first, ar);
    }
    s_tmpBndToSharings.clear();
}

const std::array<Element*, 2>& MeshInfoHolder::GetSharingElements(
                                                const size_t& inter_tag) {
    try {
        return s_interfaceElements.at(inter_tag);
    } catch(...) {
        std::cerr << "Unable to fetch interface element with tag " << inter_tag
                    << ".\n";
        std::abort();
    }
}

MeshInfoHolder::element_iterator MeshInfoHolder::ElementBegin(
                                                        const short& reg = -1) {
    auto bkt = s_interiorElements.bucket(reg);
    auto ele = s_interiorElements.begin(bkt);

    return element_iterator(ele->second, reg);
}

// Definition of Helper Class Functions (ElementIterator)
MeshInfoHolder::ElementIterator::ElementIterator(
                Element* ele, 
                const short& reg
                ) : elem(ele), region(reg) {
    // TODO
    // set bkt_beg / bkt_end
}

// Move constructor
MeshInfoHolder::ElementIterator::ElementIterator(
                                const ElementIterator&& rr_init) {

}

// Move assignment
MeshInfoHolder::ElementIterator& MeshInfoHolder::ElementIterator::operator=(
                                            const ElementIterator&& rr_rhs) {
    elem = rr_rhs.elem;
    region = rr_rhs.region;
    bkt_beg = rr_rhs.bkt_beg;
    bkt_end = rr_rhs.bkt_end;

    return *this;
}

MeshInfoHolder::ElementIterator& MeshInfoHolder::ElementIterator::operator++() {

}

} // end of namespace CHONS