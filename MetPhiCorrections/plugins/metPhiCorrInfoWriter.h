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
#include "DataFormats/PatCandidates/interface/Muon.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TTree.h"
#include <string>
#include <vector>
#include <TProfile.h>
#include <TProfile2D.h>
#include <TH2F.h>
#include <TVector2.h>

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
  edm::EDGetTokenT<pat::MuonCollection> MuonLabel_;

  std::string moduleLabel_;
  std::vector<edm::ParameterSet> cfgCorrParameters_;
  //std::vector<TProfile* > profileMetBin_x_ , profileMetBin_y_;
  std::vector<TProfile* > profile_x_ , profile_y_;
  std::vector<TProfile* > profile_phiPtcMet;
  std::vector<TProfile* > 
    profile_phiPtcMetVal_ptcMet1,  profile_phiPtcMetVal_ptcMet2, profile_phiPtcMetVal_ptcMet3,
    profile_phiPtcMetVal_ptcMet4,  profile_phiPtcMetVal_ptcMet5, profile_phiPtcMetVal_ptcMet6,
    profile_phiPtcMetVal_ptcMet7,  profile_phiPtcMetVal_ptcMet8, profile_phiPtcMetVal_ptcMet9,
    profile_phiPtcMetVal_ptcMet10, profile_phiPtcMetVal_ptcMet11;
  std::vector<TProfile* > 
    profile_phiPtcMetVal_nvtx1,  profile_phiPtcMetVal_nvtx2, profile_phiPtcMetVal_nvtx3,
    profile_phiPtcMetVal_nvtx4,  profile_phiPtcMetVal_nvtx5, profile_phiPtcMetVal_nvtx6,
    profile_phiPtcMetVal_nvtx7,  profile_phiPtcMetVal_nvtx8, profile_phiPtcMetVal_nvtx9,
    profile_phiPtcMetVal_nvtx10, profile_phiPtcMetVal_nvtx11;

  std::vector<TProfile* > 
    profile_phiPtcMet_ptcMet1,  profile_phiPtcMet_ptcMet2, profile_phiPtcMet_ptcMet3,
    profile_phiPtcMet_ptcMet4,  profile_phiPtcMet_ptcMet5, profile_phiPtcMet_ptcMet6,
    profile_phiPtcMet_ptcMet7,  profile_phiPtcMet_ptcMet8, profile_phiPtcMet_ptcMet9,
    profile_phiPtcMet_ptcMet10, profile_phiPtcMet_ptcMet11;
  std::vector<TProfile* > 
    profile_phiPtcMet_nvtx1,  profile_phiPtcMet_nvtx2, profile_phiPtcMet_nvtx3,
    profile_phiPtcMet_nvtx4,  profile_phiPtcMet_nvtx5, profile_phiPtcMet_nvtx6,
    profile_phiPtcMet_nvtx7,  profile_phiPtcMet_nvtx8, profile_phiPtcMet_nvtx9,
    profile_phiPtcMet_nvtx10, profile_phiPtcMet_nvtx11;

  std::vector<TProfile* >  prof_MetX_Nvtx, prof_MetY_Nvtx;
  std::vector<TProfile* >  prof_MetX_Pt1Nvtx, prof_MetY_Pt1Nvtx;
  std::vector<TProfile* >  prof_MetX_Pt2Nvtx, prof_MetY_Pt2Nvtx;
  std::vector<TProfile* >  prof_MetX_Pt3Nvtx, prof_MetY_Pt3Nvtx;
  std::vector<TProfile* >  prof_MetX_Pt4Nvtx, prof_MetY_Pt4Nvtx;
  std::vector<TProfile* >  prof_MetX_Pt5Nvtx, prof_MetY_Pt5Nvtx;
  std::vector<TProfile* >  prof_MetX_Pt6Nvtx, prof_MetY_Pt6Nvtx;
  std::vector<TProfile* >  prof_MetX_Pt7Nvtx, prof_MetY_Pt7Nvtx;

  TProfile *profile_MET_x , *profile_MET_y, *profile_pfMet_phi;
  TProfile 
    *profile_pfMet_phi_vtx1, *profile_pfMet_phi_vtx2, *profile_pfMet_phi_vtx3, *profile_pfMet_phi_vtx4,
    *profile_pfMet_phi_vtx5, *profile_pfMet_phi_vtx6, *profile_pfMet_phi_vtx7, *profile_pfMet_phi_vtx8;

  TProfile *profile_nvtx_x , *profile_nvtx_y;
  std::vector<TH2F* > occupancy_ , energy_, pt_, h2_met_varType_x, h2_met_varType_y;
  std::vector<TProfile2D* > prof_ptcMetVsNvtx_MEtX, prof_ptcMetVsNvtx_MEtY;
  std::vector<TH1F* > variable_;
  TH2F* h2_met_nvtx_MEtX, *h2_met_nvtx_MEtY;
  TH1F* h1_met, *h1_nvtx;

  std::vector<double> etaMin_, etaMax_, MEx_, MEy_, sumPt_;
  std::vector<int> type_, varType_, nbins_, counts_, etaNBins_;

  std::vector<int> Count_counts, Count_catagory;
  std::vector<double> Count_MetX, Count_MetY, Count_pfMetT, Count_pfMetX, Count_pfMetY;
  std::vector<int> nVtx_catagory, nVtx_nVtx;
  std::vector<double> nVtx_MetX, nVtx_MetY, nVtx_pfMetT, nVtx_pfMetX, nVtx_pfMetY;
  std::vector<int> sumPt_catagory;
  std::vector<double> sumPt_sumPt, sumPt_MetX, sumPt_MetY, sumPt_pfMetT, sumPt_pfMetX, sumPt_pfMetY;

  static int translateTypeToAbsPdgId( reco::PFCandidate::ParticleType type );

  double pfMet_px, pfMet_py, pfMet_pt, pfMet_phi;
  double ptcMetPhi, usedPhi, ptcMet, usedPt, modUsedPt;
  TVector2 *tv2_ptcMet;

};

#endif


