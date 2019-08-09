
#include "../interface/TreeGroup.h"
#include "../interface/TreeWrapper.h"

namespace ROOT {
    Leaf& TreeGroup::operator[](const std::string& name) {
        return m_wrapper.operator[](m_prefix + name);
    }

    TreeGroup TreeGroup::group(const std::string& prefix) const {
        return TreeGroup(prefix + this->m_prefix, this->m_wrapper);
    }

    TreeGroup::TreeGroup(const TreeGroup& o):
        m_prefix(o.m_prefix),
        m_wrapper(o.m_wrapper) {
        // Empty
    }

    TreeGroup::TreeGroup(TreeGroup&& o):
        m_prefix(o.m_prefix),
        m_wrapper(o.m_wrapper) {
        // Empty
    }
}
