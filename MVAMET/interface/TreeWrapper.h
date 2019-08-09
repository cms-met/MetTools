#pragma once

#include <memory>
#include <unordered_map>

#include "Leaf.h"
#include "TreeGroup.h"

class TTree;
class TChain;

namespace ROOT {

    /* Using the TreeWrapper library
     *
     * 
     * */
    class TreeWrapper {

        friend ROOT::TreeWrapperAccessor;

        public:
            /* Create a new instance of TreeWrapper.
             * @tree The tree to wrap. Must not be null.
             *
             * Create a new instance of TreeWrapper. If you use this constructor, each branches accessed with <operator[]> will be created / red immediately.
             *
             * If any branches are present in the tree, their default status will be put to 0. Only branches registered with <operator[]> will have their status put to 1. See [here](https://root.cern.ch/root/html/TTree.html#TTree:SetBranchStatus) for more details about the meaning of branch status.
             */
            TreeWrapper(TTree* tree);

            /* Create a new instance of TreeWrapper.
             *
             * Create a new instance of TreeWrapper. You must call <init> when your tree is ready to be wrapped.
             */
            TreeWrapper();

            /* Copy constructor */
            TreeWrapper(const TreeWrapper& o);

            /* Move constructor */
            TreeWrapper(TreeWrapper&& o);

            /* Wrap the tree.
             * @tree The tree to wrap. Must not be null.
             *
             * Registered all branches already accessed with <operator[]> in the tree. Subsequent call to <operator[]> will cause the branch to be registered immediately into <tree>.
             *
             * If any branches are present in the tree, their default status will be put to 0. Only branches registered with <operator[]> will have their status put to 1. See [here](https://root.cern.ch/root/html/TTree.html#TTree:SetBranchStatus) for more details about the meaning of branch status.
             * */
            void init(TTree* tree);

            /* Read the next entry of the tree.
             *
             * @return True if the entry has been red correctly, false otherwise
             */
            bool next();

            bool getEntry(uint64_t entry);

            // Rewind to the beginning of the tree.
            void rewind() {
                m_entry = -1;
            }

            /* Fill the tree.
             * @reset If true, automatically reset all the branches to their default value after filling the tree
             *
             * Fill the tree. If <reset> is true, all the branches will be resetted to their default value. See <ResetterT> for more details about the reset procedure.
             */
            void fill(bool reset = true) {
                m_tree->Fill();
                if (reset)
                    this->reset();
            }

            /* Get the number of entries in the tree
             *
             * @return the number of entries in the tree
             */
            inline uint64_t getEntries() {
                return m_tree->GetEntries();
            }

            /* Reset all the branches to their default value.
             *
             * Reset all the branches to their default value. See <ResetterT> for more details about the reset procedure.
             */
            inline void reset() {
                for (auto& leaf: m_leafs)
                    leaf.second->reset();
            }

            /* Register a new branch into the tree.
             * @name The branch name
             *
             * Register a new branch named <name> into the tree. The read or write mode is chosen by the user with a call to <Leaf::read> or <Leaf::write> methods.
             *
             * The type of the branch is automatically derived from the template type used when calling <Leaf::read> or <Leaf::write>. For example, calling
             * ```
             * Leaf::write<float>()
             * ```
             * will create a new branch holding a float. No restriction of type is applied.
             *
             * @return A reference to the newly registered branch. Use one of the method <Leaf::read> or <Leaf::write> to set the type of data hold by this branch, and to choose the read or write access mode.
             */
            Leaf& operator[](const std::string& name);

            /* Create a new group inside the tree.
             * @prefix A string which will be automatically prefixed to all the branches created in this group
             *
             * Create a new group inside the tree. A group is a collection of branches prefixed by the string ``prefix``.
             *
             * @return A new instance of TreeWrapper representing the current group of branches.
             */
            TreeGroup group(const std::string& prefix);

        private:
            TTree* m_tree;
            TChain* m_chain; // In case of the tree is in reality a TChain, this stores m_tree casted to TChain
            int64_t m_entry;
            bool m_cleaned = false;

            std::unordered_map<std::string, std::shared_ptr<Leaf>> m_leafs;
    };
};
