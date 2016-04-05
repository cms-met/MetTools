#pragma once

#include <TLorentzVector.h>
#include <TClonesArray.h>
#include <Math/Vector4Dfwd.h>

#include <vector>
#include <utility>
#include <map>
#include <string>

struct Resetter {
    public:
        virtual void reset() = 0;
};

/* Base class for `reset` functionnality.
 * @T The type of the variable to reset
 */
 template <typename T>
struct ResetterT: Resetter {
    public:
        ResetterT(T& data)
            : m_data(data) {
            }

        virtual void reset() {
            // By default, set data to 0
            // This is not suitable for every type, so specialized templates are provided for other types
            m_data = 0;
        }

    private:
        T& m_data;
};

template <typename T>
struct ResetterT<std::vector<T>>: Resetter {
    public:
        ResetterT(std::vector<T>& data)
            : m_data(data) {
            }

        virtual void reset() {
            m_data.clear();
        }

    private:
        std::vector<T>& m_data;
};

template <typename T1, typename T2>
struct ResetterT<std::map<T1, T2>>: Resetter {
    public:
        ResetterT(std::map<T1, T2>& data)
            : m_data(data) {
            }

        virtual void reset() {
            m_data.clear();
        }

    private:
        std::map<T1, T2>& m_data;
};

template <>
struct ResetterT<std::string>: Resetter {
    public:
        ResetterT(std::string& data)
            : m_data(data) {
            }

        virtual void reset() {
            m_data.clear();
        }

    private:
        std::string& m_data;
};

template <typename T, typename U>
struct ResetterT<std::pair<T, U>>: Resetter {
    public:
        ResetterT(std::pair<T, U>& data)
            : m_data(data) {
            }

        virtual void reset() {
            m_data.first = 0;
            m_data.second = 0;
        }

    private:
        std::pair<T, U>& m_data;
};

template <>
struct ResetterT<TLorentzVector>: Resetter {
    public:
        ResetterT(TLorentzVector& data)
            : m_data(data) {
            }

        virtual void reset() {
            m_data.SetPtEtaPhiE(0., 0., 0., 0.);
        }

    private:
        TLorentzVector& m_data;
};

template <>
struct ResetterT<TClonesArray>: Resetter {
    public:
        ResetterT(TClonesArray& data)
            : m_data(data) {
            }

        virtual void reset() {
            m_data.Clear();
        }

    private:
        TClonesArray& m_data;
};

template <typename CoordinateSystem>
struct ResetterT<ROOT::Math::LorentzVector<CoordinateSystem>>: Resetter {
    public:
        ResetterT(ROOT::Math::LorentzVector<CoordinateSystem>& data)
            : m_data(data) {
            }

        virtual void reset() {
            m_data.SetCoordinates(0., 0., 0., 0.);
        }

    private:
        ROOT::Math::LorentzVector<CoordinateSystem>& m_data;
};
