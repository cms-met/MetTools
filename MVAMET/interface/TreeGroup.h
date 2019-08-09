#pragma once

#include <string>

namespace ROOT {
    class TreeWrapper;
    class Leaf;

    class TreeGroup {
        friend class TreeWrapper;

        public:
            Leaf& operator[](const std::string& name);
            TreeGroup group(const std::string& prefix) const;

            TreeGroup(const TreeGroup& o);
            TreeGroup(TreeGroup&& o);

        private:

            TreeGroup(const std::string prefix, TreeWrapper& wrapper):
                m_prefix(prefix),
                m_wrapper(wrapper) {

                }

        private:
            std::string m_prefix;
            TreeWrapper& m_wrapper;
    };
}
