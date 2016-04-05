#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "../interface/Analyzer.h"

#include <TTree.h>

#include <limits>

JME::Analyzer::Analyzer(const edm::ParameterSet& iConfig)
    : moduleLabel_(iConfig.getParameter<std::string>("@module_label")) {

        if (iConfig.existsAs<std::string>("treeName"))
            treeName_ = iConfig.getParameter<std::string>("treeName");
        else
            treeName_ = "t";
    }

JME::Analyzer::~Analyzer() {
    // Empty
}

void JME::Analyzer::beginJob()
{
    edm::Service<TFileService> fs;
    if (!fs)
        throw edm::Exception(edm::errors::Configuration, "TFileService missing from configuration!");

    TTree* tree_ = fs->make<TTree>(treeName_.c_str(), treeName_.c_str());

    // Disable autosave
    tree_->SetAutoSave(std::numeric_limits<Long64_t>::max());

    // Debug auto flush is 30 MB. In full mode we have about 32 trees in the output fill, so 960 MB only for buffering.
    // Reduce to 1 MB per tree to reduce memory footprint
    tree_->SetAutoFlush(-1 * 1000 * 1000);

    tree.init(tree_);
}
