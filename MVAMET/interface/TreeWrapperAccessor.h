#pragma once

#include <TTree.h>

namespace ROOT {
    class TreeWrapper;

    struct TreeWrapperAccessor {
        ROOT::TreeWrapper* wrapper;

        public:
        TreeWrapperAccessor(ROOT::TreeWrapper* wrap);
        TTree* tree();
        uint64_t entry();
    };

};
