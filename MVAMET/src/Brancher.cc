
#include <TTree.h>

namespace ROOT {

    namespace utils {

        void activateBranch(TBranch* branch) {
            if (! branch)
                return;

            branch->SetStatus(1);
            TObjArray* objArray = branch->GetListOfBranches();
            for (int i = 0; i < objArray->GetEntries(); i++) {
                TBranch* b = static_cast<TBranch*> (objArray->At(i));
                if (b) {
                    activateBranch(b);
                }
            }
        }
    }
}
