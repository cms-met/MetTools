#pragma once

#include <string>

#include <TTree.h>

namespace ROOT {

    namespace utils {

        /* Set the branch status to 1 and activate the branch <branch>.
         * @branch the branch to activate. Must not be null.
         *
         * Set the branch status to 1 and activate the branch <branch>. This will cause the branch to be red by a call to <TreeWrapper::next>.
         * If this branch has any sub-branches, they will also be activated (useful if <branch> points to a complex object and the branch has a split-mode greater than 0).
         */
        void activateBranch(TBranch* branch);
    }
}

struct Brancher {
    public:
        virtual void operator()(const std::string&, TTree* tree) = 0;
};

template <typename T>
struct BranchCreaterT: Brancher {
    public:
        BranchCreaterT(T& data)
            : m_data(data) {
            }

        virtual void operator()(const std::string& name, TTree* tree) {
            tree->Branch<T>(name.c_str(), &m_data);
        }

    private:
        T& m_data;
};

template <typename T>
struct BranchReaderT: Brancher {
    public:
        BranchReaderT(T* data, TBranch** branch)
            : m_data(data), m_branch(branch) {
            }

        BranchReaderT(T** data, TBranch** branch)
            : m_data_ptr(data), m_branch(branch) {
            }

        virtual void operator()(const std::string& name, TTree* tree) {
            *m_branch = tree->GetBranch(name.c_str());
            if (! *m_branch) {
                std::cout << "Warning: branch '" << name << "' not found in tree" << std::endl;
                return;
            }

            if (m_data)
                tree->SetBranchAddress<T>(name.c_str(), m_data, m_branch);
            else
                tree->SetBranchAddress<T>(name.c_str(), m_data_ptr, m_branch);

            ROOT::utils::activateBranch((*m_branch));
        }

    private:
        T* m_data = nullptr;
        T** m_data_ptr = nullptr;
        TBranch** m_branch;
};
