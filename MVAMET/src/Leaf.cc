#include "../interface/Leaf.h"

namespace ROOT {
    Leaf::Leaf(const std::string& name, const TreeWrapperAccessor& tree):
        m_name(name),
        m_tree(tree) {

        }
};
