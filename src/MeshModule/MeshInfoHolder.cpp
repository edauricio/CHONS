#include "MeshModule/MeshInfoHolder.h"
#include <iostream>

namespace CHONS {

MeshInfoHolder* MeshInfoHolder::s_soleInstance = nullptr;

MeshInfoHolder* MeshInfoHolder::GetInstance() {
    if (!s_soleInstance)
        s_soleInstance = new MeshInfoHolder;
    return s_soleInstance;
}

void MeshInfoHolder::AddInterfaceElement(const ElementInfo& ein, 
                                        const int& rTag) {
    s_tmpInterElements[ein.type].emplace(rTag, ein.tag);
}

void MeshInfoHolder::AddInteriorElement(const ElementInfo& ein, 
                                            const int& rTag) {
        s_tmpIntElements[ein.type].emplace(rTag, ein.tag);
}

void MeshInfoHolder::Consolidate() {
    ElementInfo einfo;
    for (int i = 0; i != MAX_NUMBER_OF_TYPES; ++i) {
        if (!s_tmpInterElements[i].empty()) {
            einfo.type = static_cast<ElementType>(i);
            for (auto bcnt = 0; bcnt != s_tmpInterElements[i].bucket_count();
                        ++bcnt)
                for (auto bckt_it = s_tmpInterElements[i].begin(bcnt);
                            bckt_it != s_tmpInterElements[i].end(bcnt); bckt_it++) {
                    einfo.tag = bckt_it->second;
                    s_interfaceElements.emplace(bckt_it->first,
                                        s_factory->GetElement(einfo));
                }
            s_tmpInterElements[i].clear();
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
            s_tmpIntElements[i].clear();
        }
    }

}

MeshInfoHolder::iterator MeshInfoHolder::ElementsBegin(
                                                        const short& reg) {
    auto bkt = s_interiorElements.bucket(reg);
    return {s_interiorElements.begin(bkt), s_interiorElements.end(bkt)};
}

MeshInfoHolder::iterator MeshInfoHolder::ElementsEnd(
                                                        const short& reg) {
    auto bkt = s_interiorElements.bucket(reg);
    return {s_interiorElements.end(bkt), s_interiorElements.end(bkt)};
}

MeshInfoHolder::iterator MeshInfoHolder::BoundaryBegin(
                                                        const short& reg) {
    auto bkt = s_interfaceElements.bucket(reg);
    return {s_interfaceElements.begin(bkt), s_interfaceElements.end(bkt)};
}

MeshInfoHolder::iterator MeshInfoHolder::BoundaryEnd(
                                                        const short& reg) {
    auto bkt = s_interfaceElements.bucket(reg);
    return {s_interfaceElements.end(bkt), s_interfaceElements.end(bkt)};
}

MeshInfoHolder::iterator MeshInfoHolder::InterfaceBegin() {
    auto bkt = s_interfaceElements.bucket(-1);
    return {s_interfaceElements.begin(bkt), s_interfaceElements.end(bkt)};
}

MeshInfoHolder::iterator MeshInfoHolder::InterfaceEnd() {
    auto bkt = s_interfaceElements.bucket(-1);
    return {s_interfaceElements.end(bkt), s_interfaceElements.end(bkt)};
}

// Definition of ElementIterator helper class to iterate through the elements

MeshInfoHolder::ElementIterator::ElementIterator(bucket_iterator me,
                                            bucket_iterator e) : it(me),
                                                end(e) {
    if (it != end)
        elem = it->second;
}

MeshInfoHolder::ElementIterator::ElementIterator(const ElementIterator& eit) :
                                                it(eit.it), end(eit.end) {
    if (it != end)
        elem = it->second;
}

MeshInfoHolder::ElementIterator& MeshInfoHolder::ElementIterator::operator=(
                                                    const ElementIterator& eit){
    it = eit.it;
    end = eit.end;
    if (it != end)
        elem = eit.elem;
    else
        elem = nullptr;
    return *this;
}

MeshInfoHolder::ElementIterator& MeshInfoHolder::ElementIterator::operator++() {
    it++;
    if (it != end)
        elem = it->second;
    else
        elem = nullptr;
    return *this;
}

MeshInfoHolder::ElementIterator MeshInfoHolder::ElementIterator::operator++(int) {
    bucket_iterator it_old = it++;
    if (it != end)
        elem = it->second;
    else
        elem = nullptr;
    return ElementIterator(it_old, end);
}

bool MeshInfoHolder::ElementIterator::operator==(const ElementIterator& rhs) {
    return (it == rhs.it);
}

bool MeshInfoHolder::ElementIterator::operator!=(const ElementIterator& rhs) {
    return !(*this == rhs);
}

} // end of namespace CHONS