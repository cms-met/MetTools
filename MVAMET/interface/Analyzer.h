#pragma once

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "../interface/TreeWrapper.h"

namespace JME {
  class Analyzer : public edm::EDAnalyzer {
        public:
            // construction/destruction
            explicit Analyzer(const edm::ParameterSet& iConfig);
            virtual ~Analyzer();

        protected:

            // member functions
            virtual void beginJob() override;
            virtual void analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) = 0;

        protected:

            // member data
            std::string moduleLabel_;

            // tree
            std::string treeName_;
            ROOT::TreeWrapper tree;
    };
}
