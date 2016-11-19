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
#include "DataFormats/PatCandidates/interface/MET.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TTree.h"
#include <string>
#include <vector>
#include <TProfile.h>
#include <TH2F.h>

class metPhiCorrInfoWriter : public edm::EDAnalyzer {
public:
  metPhiCorrInfoWriter( const edm::ParameterSet & );

private:
  edm::Service<TFileService> fs;
  TTree* OutTree;
  virtual void beginJob() override;
  void analyze( const edm::Event& , const edm::EventSetup& );

  typedef std::vector<pat::MET> patMETCollection;
  edm::InputTag vertices_;
  edm::EDGetTokenT< std::vector<reco::Vertex> >     verticesToken_;
  edm::InputTag pflow_;
  edm::EDGetTokenT< edm::View<reco::Candidate> >    pflowToken_;
  edm::EDGetTokenT<patMETCollection> metToken_;

  std::string moduleLabel_;
  std::vector<edm::ParameterSet> cfgCorrParameters_;
  std::vector<TProfile* > profile_x_ , profile_y_;
  std::vector<TH2F* > occupancy_ , energy_, pt_;
  std::vector<TH1F* > variable_;

  std::vector<double> etaMin_, etaMax_, MEx_, MEy_, sumPt_;
  std::vector<int> type_, varType_, nbins_, counts_, etaNBins_;

  std::vector<int> Count_counts, Count_catagory;
  std::vector<double> Count_MetX, Count_MetY, Count_pfMetT, Count_pfMetX, Count_pfMetY;
  std::vector<int> nVtx_catagory, nVtx_nVtx;
  std::vector<double> nVtx_MetX, nVtx_MetY, nVtx_pfMetT, nVtx_pfMetX, nVtx_pfMetY;
  std::vector<int> sumPt_catagory;
  std::vector<double> sumPt_sumPt, sumPt_MetX, sumPt_MetY, sumPt_pfMetT, sumPt_pfMetX, sumPt_pfMetY;

  static int translateTypeToAbsPdgId( reco::PFCandidate::ParticleType type );

  double pfMet_px, pfMet_py, pfMet_pt;

};

#endif


