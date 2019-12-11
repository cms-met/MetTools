#ifndef HEPHECommonTools_metPhiCorrInfoWriter_H
#define HEPHECommonTools_metPhiCorrInfoWriter_H
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Utilities/interface/EDGetToken.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/InputTag.h"
// #include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"                                                                               
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/PatCandidates/interface/Muon.h"

#include <string>
#include <vector>
#include <TProfile.h>
#include <TH2F.h>
#include <TLorentzVector.h>

class metPhiCorrInfoWriter : public edm::EDAnalyzer {
public:
  metPhiCorrInfoWriter( const edm::ParameterSet & );

private:
  edm::InputTag vertices_;
  edm::EDGetTokenT< std::vector<reco::Vertex> >     verticesToken_;
  edm::InputTag muons_;
  edm::EDGetTokenT<std::vector< pat::Muon> > muonsToken_;
  edm::InputTag pflow_;
  edm::EDGetTokenT< edm::View<reco::Candidate> >    pflowToken_;

  void analyze( const edm::Event& , const edm::EventSetup& );
  std::string moduleLabel_;
  std::vector<edm::ParameterSet> cfgCorrParameters_;
  std::vector<TProfile* > profile_x_ , profile_y_;
  std::vector<TH2F* > occupancy_ , energy_, pt_;
  std::vector<TH1F* > variable_;

  std::vector<double> etaMin_, etaMax_, MEx_, MEy_, sumPt_;
  std::vector<int> type_, varType_, nbins_, counts_, etaNBins_;

  static int translateTypeToAbsPdgId( reco::PFCandidate::ParticleType type );

  bool passSelection(TLorentzVector firstMuon, TLorentzVector secondMuon);

};

#endif


