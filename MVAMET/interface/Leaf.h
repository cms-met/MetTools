#pragma once

#include <boost/any.hpp>
#include <iostream>
#include <memory>

#include <TTree.h>

#include "Brancher.h"
#include "Resetter.h"
#include "TreeWrapperAccessor.h"

namespace ROOT {

    class TreeWrapper;

    /* This class holds anything related to the branch
     *
     * The only way to create a Leaf instance is from TreeWrapper <TreeWrapper::operator[]>.
     */
    class Leaf {
        private:
            Leaf(const std::string& name, const TreeWrapperAccessor& tree);

        public:
            /* Register this branch for write access
             * @T Type of data this branch holds
             * @autoReset if true, this leaf will be automatically reset to its default value. Set to false to disable this mecanism.
             *
             * Register this branch for write access. A new branch will be created in the tree properly configured to hold data of type T.
             *
             * Internally, the `TTree::Branch` method is called to create the new branch.
             *
             * @return a reference to the data hold by this branch. Change the content of this reference before calling <TreeWrapper::fill> to change the branch data.
             */
            template<typename T> T& write(bool autoReset = true) {
                return write_internal<T>(false, autoReset);
            };

            /* Register a transient branch
             * @T Type of data this branch holds
             * @autoReset if true, this leaf will be automatically reset to its default value. Set to false to disable this mecanism.
             *
             * Register a transient branch. No branch will be created into the tree, but auto-reset mechanism is still provided.
             *
             * @return a reference to the data hold by this transient branch.
             */
            template<typename T> T& transient_write(bool autoReset = true) {
                return write_internal<T>(true, autoReset);
            };

            /* Register this branch for write access
             * @T Type of data this branch holds
             * @P Variadic template for constructor arguments
             * @parameters A list of arguments which will be passed to the constructor of T
             *
             * Register this branch for write access. A new branch will be created in the tree properly configured to hold data of type T.
             *
             * Internally, the `TTree::Branch` method is called to create the new branch.
             *
             * @return a reference to the data hold by this branch. Change the content of this reference before calling <TreeWrapper::fill> to change the branch data. Since this value is initialized with some parameters, auto reset is automatically disabled.
             */
            template<typename T, typename... P> T& write_with_init(P&&... parameters) {
                return write_internal<T, P...>(false, false, std::forward<P>(parameters)...);
            };

            /* Register this branch for read access
             * @T Type of data this branch holds
             *
             * Register this branch for read access. The branch must exists in the tree and created to hold data of type T.
             *
             * Internally, the `TTree::SetBranchAddress` method is called to read the branch. The status of the branch is also set to 1.
             *
             * @return a const reference to the data hold by this branch. The content is in read-only mode, and will change each time <TreeWrapper::next> is called.
             */
            template<typename T> const T& read() {
                if (m_data.empty() && m_data_ptr == nullptr) {

                    m_store_class = TClass::GetClass(typeid(T)) != nullptr;

                    // Initialize boost::any with empty data.
                    // This allocate the necessary memory
                    
                    if (! m_store_class) {
                        m_data = boost::any(std::shared_ptr<T>(new T()));

                        T* data = boost::any_cast<std::shared_ptr<T>>(m_data).get();
                        m_resetter.reset(new ResetterT<T>(*data));

                        if (m_tree.tree()) {
                            m_branch = m_tree.tree()->GetBranch(m_name.c_str());
                            if (m_branch) {
                                m_tree.tree()->SetBranchAddress<T>(m_name.c_str(), data, &m_branch);
                                // Enable read for this branch
                                ROOT::utils::activateBranch(m_branch);
                            } else {
                                std::cout << "Warning: branch '" << m_name << "' not found in tree" << std::endl;
                            }
                        } else {
                            m_brancher.reset(new BranchReaderT<T>(data, &m_branch));
                        }
                    } else {
                        m_data_ptr = nullptr;
                        m_data_ptr_ptr = &m_data_ptr;

                        if (m_tree.tree()) {
                            m_branch = m_tree.tree()->GetBranch(m_name.c_str());
                            if (m_branch) {
                                m_tree.tree()->SetBranchAddress<T>(m_name.c_str(), reinterpret_cast<T**>(m_data_ptr_ptr), &m_branch);
                                // Enable read for this branch
                                ROOT::utils::activateBranch(m_branch);
                            } else {
                                std::cout << "Warning: branch '" << m_name << "' not found in tree" << std::endl;
                            }
                        } else {
                            m_brancher.reset(new BranchReaderT<T>(reinterpret_cast<T**>(m_data_ptr_ptr), &m_branch));
                        }

                        return const_cast<const T&>(*reinterpret_cast<T*>(m_data_ptr));
                    }


                    if (m_tree.entry() != -1) {
                        // A global GetEntry already happened in the tree
                        // Call GetEntry directly on the Branch to catch up
                        m_branch->GetEntry(m_tree.entry());
                    }
                }

                // Return a const since we read from the tree
                if (! m_store_class)
                    return const_cast<const T&>(*boost::any_cast<std::shared_ptr<T>>(m_data));
                else
                    return const_cast<const T&>(*reinterpret_cast<T*>(m_data_ptr));
            }

        private:
            void init(const TreeWrapperAccessor& tree) {
                m_tree = tree;
                if (m_brancher.get())
                    (*m_brancher)(m_name, m_tree.tree());
            }

            void reset() {
              if (m_resetter.get()) {
                  m_resetter->reset();
              }
            }

            TBranch* getBranch() {
                return m_branch;
            }

            template<typename T, typename... P> T& write_internal(bool transient, bool autoReset, P&&... parameters) {
                if (m_data.empty()) {
                    // Initialize boost::any with empty data.
                    // This allocate the necessary memory
                    if (sizeof...(parameters) != 0) {
                        autoReset = false;
                        m_data = boost::any(T(std::forward<P>(parameters)...));
                    } else {
                        m_data = boost::any(T());
                    }

                    T& data = boost::any_cast<T&>(m_data);
                    if (autoReset)
                        m_resetter.reset(new ResetterT<T>(data));

                    if (! transient) {
                        if (m_tree.tree()) {
                            // Register this Leaf in the tree
                            m_tree.tree()->Branch<T>(m_name.c_str(), &data);
                        } else {
                            m_brancher.reset(new BranchCreaterT<T>(data));
                        }
                    }
                }

                return boost::any_cast<T&>(m_data);
            }

            Leaf(const Leaf&) = delete;
            Leaf& operator=(const Leaf&) = delete;

        private:

            friend class TreeWrapper;

            boost::any m_data;

            void* m_data_ptr = nullptr;
            void** m_data_ptr_ptr = nullptr;

            TBranch* m_branch = nullptr;

            std::string m_name;
            TreeWrapperAccessor m_tree;

            std::unique_ptr<Resetter> m_resetter;
            std::unique_ptr<Brancher> m_brancher;

            bool m_store_class;
    };
};
