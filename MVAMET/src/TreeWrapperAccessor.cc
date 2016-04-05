#include "../interface/TreeWrapper.h"
#include "../interface/TreeWrapperAccessor.h"

namespace ROOT {
    TreeWrapperAccessor::TreeWrapperAccessor(ROOT::TreeWrapper* wrap) {
        wrapper = wrap;
    }

    TTree* TreeWrapperAccessor::tree() {
        return wrapper->m_tree;
    }

    uint64_t TreeWrapperAccessor::entry() {
        return wrapper->m_entry;
    }
}
