#include <memory>

#include <TChain.h>
#include <TTree.h>

#include "../interface/TreeWrapper.h"

namespace ROOT {
    TreeWrapper::TreeWrapper(TTree* tree):
        m_tree(tree),
        m_chain(nullptr),
        m_entry(-1) {
            init(tree);
        }

    TreeWrapper::TreeWrapper():
        m_tree(nullptr),
        m_chain(nullptr),
        m_entry(-1) {

        }

    TreeWrapper::TreeWrapper(const TreeWrapper& o) {
        m_tree = o.m_tree;
        m_chain = o.m_chain;
        m_leafs = o.m_leafs;
    }

    TreeWrapper::TreeWrapper(TreeWrapper&& o) {
        m_tree = o.m_tree;
        m_chain = o.m_chain;
        m_leafs = std::move(o.m_leafs);
    }

    void TreeWrapper::init(TTree* tree) {
        m_tree = tree;
        m_chain = dynamic_cast<TChain*>(tree);
        if (m_chain)
            m_chain->LoadTree(0);

        for (auto& leaf: m_leafs)
            leaf.second->init(this);
    }

    /**
     * Read the next entry of the tree.
     * 
     * returns true in case of success, or false if the end of the tree is reached
     */
    bool TreeWrapper::next() {
        uint64_t entries = getEntries();

        if (m_entry + 1 > (int64_t) entries)
            return false;

        m_entry++;

        return getEntry(m_entry);
    }

    bool TreeWrapper::getEntry(uint64_t entry) {

        if (! m_cleaned) {
            for (auto it = m_leafs.begin(); it != m_leafs.end(); ) {
                if (it->second->getBranch() == nullptr) {
                    it = m_leafs.erase(it);
                } else
                    ++it;
            }

            m_cleaned = true;
        }

        uint64_t local_entry = entry;
        if (m_chain) {
            int64_t tree_index = m_chain->LoadTree(local_entry);
            if (tree_index < 0) {
                return false;
            }

            local_entry = static_cast<uint64_t>(tree_index);
        }

        for (auto& leaf: m_leafs) {
            int res = leaf.second->getBranch()->GetEntry(local_entry);
            if (res <= 0) {
                std::cout << "GetEntry failed for branch " << leaf.first << ". Return code: " << res << std::endl;
                return false;
            }
        }

        m_entry = entry;
        return true;
    }

    Leaf& TreeWrapper::operator[](const std::string& name) {

        if (m_leafs.count(name))
            return *m_leafs.at(name);

        std::shared_ptr<Leaf> leaf(new Leaf(name, this));
        m_leafs[name] = leaf;

        return *leaf;
    }

    TreeGroup TreeWrapper::group(const std::string& prefix) {
        return TreeGroup(prefix, *this);
    }
};
