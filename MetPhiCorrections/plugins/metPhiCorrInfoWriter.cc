#include "MetTools/MetPhiCorrections/plugins/metPhiCorrInfoWriter.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/Association.h"
#include <string>

#define MetBin 6
#define PhiBin 12 
#define MkTree (0)

std::string namePostFix (int varType) {

  if (varType==0) return std::string("multiplicity");
  if (varType==1) return std::string("ngoodVertices");
  if (varType==2) return std::string("sumPt");
  if (varType==3) return std::string("pfType1Met");
  if (varType==4) return std::string("ptcMet");
  if (varType==5) return std::string("ptValence");
  return std::string("unknown");
}

int metPhiCorrInfoWriter::translateTypeToAbsPdgId( reco::PFCandidate::ParticleType type ) {
  switch( type ) {
  case reco::PFCandidate::ParticleType::h: return 211; // pi+
  case reco::PFCandidate::ParticleType::e: return 11;
  case reco::PFCandidate::ParticleType::mu: return 13;
  case reco::PFCandidate::ParticleType::gamma: return 22;
  case reco::PFCandidate::ParticleType::h0: return 130; // K_L0
  case reco::PFCandidate::ParticleType::h_HF: return 1; // dummy pdg code
  case reco::PFCandidate::ParticleType::egamma_HF: return 2; // dummy pdg code
  case reco::PFCandidate::ParticleType::X:
  default: return 0;
  }
}

metPhiCorrInfoWriter::metPhiCorrInfoWriter( const edm::ParameterSet & cfg ): 
  vertices_ ( cfg.getUntrackedParameter< edm::InputTag >("vertexCollection") ),
  verticesToken_ ( consumes< reco::VertexCollection >(vertices_) ),
  pflow_ ( cfg.getUntrackedParameter< edm::InputTag >("srcPFlow") ),
  pflowToken_ ( consumes< edm::View<reco::Candidate> >(pflow_) ),
  metToken_ ( consumes<patMETCollection>(cfg.getParameter<edm::InputTag>("srcMet"))),
  MuonLabel_(consumes<pat::MuonCollection>(cfg.getParameter<edm::InputTag>("MuonLabel"))),
  moduleLabel_(cfg.getParameter<std::string>("@module_label"))
{
  tv2_ptcMet = new TVector2();

  h1_met =fs->make<TH1F>(std::string(moduleLabel_).append("_").append("pfType1Met").c_str(),"pfType1Met", 100, 0, 300);
  h1_nvtx=fs->make<TH1F>(std::string("nvtx").c_str(),"nvtx", 50, 0, 50);
  h2_met_nvtx_MEtX=fs->make<TH2F>(std::string(moduleLabel_).append("_").append("metVsNvtx_MetX").c_str(),"met_nvtx_MEtX", 50, 0, 300,35,0,35 );
  h2_met_nvtx_MEtY=fs->make<TH2F>(std::string(moduleLabel_).append("_").append("metVsNvtx_MetX").c_str(),"met_nvtx_MEtX", 50, 0, 300,35,0,35 );
  profile_MET_x=fs->make<TProfile>(std::string(moduleLabel_).append("_").append("pfMet").append("_Px").c_str(),"pfMetVsPx", 100, 0, 300, -300,300);
  profile_MET_y=fs->make<TProfile>(std::string(moduleLabel_).append("_").append("pfMet").append("_Py").c_str(),"pfMetVsPy", 100, 0, 300, -300,300);
  profile_nvtx_x=fs->make<TProfile>(std::string(moduleLabel_).append("_").append("nvtx").append("_Px").c_str(),"nvtxVsPx", 50, 0, 50, -300,300);
  profile_nvtx_y=fs->make<TProfile>(std::string(moduleLabel_).append("_").append("nvtx").append("_Py").c_str(),"nvtxVsPy", 50, 0, 50, -300,300);
  profile_pfMet_phi=fs->make<TProfile>(std::string("pfMet").append("_phi").c_str(),"pfMet_Phi", 50, -M_PI, M_PI, -300,300);

  profile_pfMet_phi_vtx1=fs->make<TProfile>(std::string("pfMet").append("_phi_vtx1").c_str(),"pfMet_Phi", 50, -M_PI, M_PI, -300,300);
  profile_pfMet_phi_vtx2=fs->make<TProfile>(std::string("pfMet").append("_phi_vtx2").c_str(),"pfMet_Phi", 50, -M_PI, M_PI, -300,300);
  profile_pfMet_phi_vtx3=fs->make<TProfile>(std::string("pfMet").append("_phi_vtx3").c_str(),"pfMet_Phi", 50, -M_PI, M_PI, -300,300);
  profile_pfMet_phi_vtx4=fs->make<TProfile>(std::string("pfMet").append("_phi_vtx4").c_str(),"pfMet_Phi", 50, -M_PI, M_PI, -300,300);
  profile_pfMet_phi_vtx5=fs->make<TProfile>(std::string("pfMet").append("_phi_vtx5").c_str(),"pfMet_Phi", 50, -M_PI, M_PI, -300,300);
  profile_pfMet_phi_vtx6=fs->make<TProfile>(std::string("pfMet").append("_phi_vtx6").c_str(),"pfMet_Phi", 50, -M_PI, M_PI, -300,300);
  profile_pfMet_phi_vtx7=fs->make<TProfile>(std::string("pfMet").append("_phi_vtx7").c_str(),"pfMet_Phi", 50, -M_PI, M_PI, -300,300);
  profile_pfMet_phi_vtx8=fs->make<TProfile>(std::string("pfMet").append("_phi_vtx8").c_str(),"pfMet_Phi", 50, -M_PI, M_PI, -300,300);

  cfgCorrParameters_ = cfg.getParameter<std::vector<edm::ParameterSet> >("parameters");
//  etaNBins_.clear();
  etaMin_.clear();
  etaMax_.clear();
  type_.clear();
  varType_.clear();
  nbins_.clear();
  counts_.clear();
  sumPt_.clear();
  MEx_.clear();
  MEy_.clear();
  for (std::vector<edm::ParameterSet>::const_iterator v = cfgCorrParameters_.begin(); v!=cfgCorrParameters_.end(); v++) {
    int etaNBins = v->getParameter<int>("etaNBins");
    double etaMin = v->getParameter<double>("etaMin");
    double etaMax = v->getParameter<double>("etaMax");
    int phiNBins = v->getParameter<int>("phiNBins");
    double phiMin = v->getParameter<double>("phiMin");
    double phiMax = v->getParameter<double>("phiMax");
    int nMin = v->getParameter<int>("nMin");
    int nMax = v->getParameter<int>("nMax");
    int nbins = v->getParameter<double>("nbins");
//    etaNBins_.push_back(etaNBins);
    etaMin_.push_back(etaMin);
    etaMax_.push_back(etaMax);
    nbins_.push_back(nbins);
    int varType(v->getParameter<int>("varType"));
    varType_.push_back(varType);
    type_.push_back(v->getParameter<int>("type"));
    counts_.push_back(0);
    sumPt_.push_back(0.);
    MEx_.push_back(0.);
    MEy_.push_back(0.);
//    std::cout<<" n/min/max "<<nbins<<" "<<etaMin<<" "<<etaMax<<std::endl;
    //char buffer[20];
    //for( int iMet(0);iMet<MetBin; iMet++)
    //{
    //  sprintf(buffer, "_metBin%d",iMet);
    //  profileMetBin_x_.push_back(fs->make<TProfile>(std::string(moduleLabel_).append("_").append(namePostFix(varType)).append("_").append(v->getParameter<std::string>("name")).append(buffer).append("_Px").c_str(),"Px", nbins, nMin, nMax, -300,300));
    //  profileMetBin_y_.push_back(fs->make<TProfile>(std::string(moduleLabel_).append("_").append(namePostFix(varType)).append("_").append(v->getParameter<std::string>("name")).append(buffer).append("_Py").c_str(),"Py", nbins, nMin, nMax, -300,300));
    //}

    profile_x_.push_back(fs->make<TProfile>(std::string(moduleLabel_).append("_").append(namePostFix(varType)).append("_").append(v->getParameter<std::string>("name")).append("_Px").c_str(),"Px", nbins, nMin, nMax, -300,300));
    profile_y_.push_back(fs->make<TProfile>(std::string(moduleLabel_).append("_").append(namePostFix(varType)).append("_").append(v->getParameter<std::string>("name")).append("_Py").c_str(),"Py", nbins, nMin, nMax, -300,300));

    profile_phiPtcMet.push_back(fs->make<TProfile>((v->getParameter<std::string>("name")).append("_phiPtcMet").c_str(),"ptcMet", 30, -M_PI, M_PI, -300,300));

    profile_phiPtcMetVal_ptcMet1.push_back(fs->make<TProfile>((v->getParameter<std::string>("name")).append("_phiPtcMetVal_ptcMet1").c_str(),"ptcMetValence", 30, 0, M_PI, -300,300));
    profile_phiPtcMetVal_ptcMet2.push_back(fs->make<TProfile>((v->getParameter<std::string>("name")).append("_phiPtcMetVal_ptcMet2").c_str(),"ptcMetValence", 30, 0, M_PI, -300,300));
    profile_phiPtcMetVal_ptcMet3.push_back(fs->make<TProfile>((v->getParameter<std::string>("name")).append("_phiPtcMetVal_ptcMet3").c_str(),"ptcMetValence", 30, 0, M_PI, -300,300));
    profile_phiPtcMetVal_ptcMet4.push_back(fs->make<TProfile>((v->getParameter<std::string>("name")).append("_phiPtcMetVal_ptcMet4").c_str(),"ptcMetValence", 30, 0, M_PI, -300,300));
    profile_phiPtcMetVal_ptcMet5.push_back(fs->make<TProfile>((v->getParameter<std::string>("name")).append("_phiPtcMetVal_ptcMet5").c_str(),"ptcMetValence", 30, 0, M_PI, -300,300));
    profile_phiPtcMetVal_ptcMet6.push_back(fs->make<TProfile>((v->getParameter<std::string>("name")).append("_phiPtcMetVal_ptcMet6").c_str(),"ptcMetValence", 30, 0, M_PI, -300,300));
    profile_phiPtcMetVal_ptcMet7.push_back(fs->make<TProfile>((v->getParameter<std::string>("name")).append("_phiPtcMetVal_ptcMet7").c_str(),"ptcMetValence", 30, 0, M_PI, -300,300));
    profile_phiPtcMetVal_ptcMet8.push_back(fs->make<TProfile>((v->getParameter<std::string>("name")).append("_phiPtcMetVal_ptcMet8").c_str(),"ptcMetValence", 30, 0, M_PI, -300,300));
    profile_phiPtcMetVal_ptcMet9.push_back(fs->make<TProfile>((v->getParameter<std::string>("name")).append("_phiPtcMetVal_ptcMet9").c_str(),"ptcMetValence", 30, 0, M_PI, -300,300));
    profile_phiPtcMetVal_ptcMet10.push_back(fs->make<TProfile>((v->getParameter<std::string>("name")).append("_phiPtcMetVal_ptcMet10").c_str(),"ptcMetValence", 30, 0, M_PI, -300,300));
    profile_phiPtcMetVal_ptcMet11.push_back(fs->make<TProfile>((v->getParameter<std::string>("name")).append("_phiPtcMetVal_ptcMet11").c_str(),"ptcMetValence", 30, 0, M_PI, -300,300));


    profile_phiPtcMetVal_nvtx1.push_back(fs->make<TProfile>((v->getParameter<std::string>("name")).append("_phiPtcMetVal_nvtx1").c_str(),"ptcMetValence", 30, 0, M_PI, -300,300));
    profile_phiPtcMetVal_nvtx2.push_back(fs->make<TProfile>((v->getParameter<std::string>("name")).append("_phiPtcMetVal_nvtx2").c_str(),"ptcMetValence", 30, 0, M_PI, -300,300));
    profile_phiPtcMetVal_nvtx3.push_back(fs->make<TProfile>((v->getParameter<std::string>("name")).append("_phiPtcMetVal_nvtx3").c_str(),"ptcMetValence", 30, 0, M_PI, -300,300));
    profile_phiPtcMetVal_nvtx4.push_back(fs->make<TProfile>((v->getParameter<std::string>("name")).append("_phiPtcMetVal_nvtx4").c_str(),"ptcMetValence", 30, 0, M_PI, -300,300));
    profile_phiPtcMetVal_nvtx5.push_back(fs->make<TProfile>((v->getParameter<std::string>("name")).append("_phiPtcMetVal_nvtx5").c_str(),"ptcMetValence", 30, 0, M_PI, -300,300));
    profile_phiPtcMetVal_nvtx6.push_back(fs->make<TProfile>((v->getParameter<std::string>("name")).append("_phiPtcMetVal_nvtx6").c_str(),"ptcMetValence", 30, 0, M_PI, -300,300));
    profile_phiPtcMetVal_nvtx7.push_back(fs->make<TProfile>((v->getParameter<std::string>("name")).append("_phiPtcMetVal_nvtx7").c_str(),"ptcMetValence", 30, 0, M_PI, -300,300));
    profile_phiPtcMetVal_nvtx8.push_back(fs->make<TProfile>((v->getParameter<std::string>("name")).append("_phiPtcMetVal_nvtx8").c_str(),"ptcMetValence", 30, 0, M_PI, -300,300));
    profile_phiPtcMetVal_nvtx9.push_back(fs->make<TProfile>((v->getParameter<std::string>("name")).append("_phiPtcMetVal_nvtx9").c_str(),"ptcMetValence", 30, 0, M_PI, -300,300));
    profile_phiPtcMetVal_nvtx10.push_back(fs->make<TProfile>((v->getParameter<std::string>("name")).append("_phiPtcMetVal_nvtx10").c_str(),"ptcMetValence", 30, 0, M_PI, -300,300));
    profile_phiPtcMetVal_nvtx11.push_back(fs->make<TProfile>((v->getParameter<std::string>("name")).append("_phiPtcMetVal_nvtx11").c_str(),"ptcMetValence", 30, 0, M_PI, -300,300));


    profile_phiPtcMet_ptcMet1.push_back(fs->make<TProfile>((v->getParameter<std::string>("name")).append("_phiPtcMet_ptcMet1").c_str(),"ptcMet", 30, -M_PI, M_PI, -300,300));
    profile_phiPtcMet_ptcMet2.push_back(fs->make<TProfile>((v->getParameter<std::string>("name")).append("_phiPtcMet_ptcMet2").c_str(),"ptcMet", 30, -M_PI, M_PI, -300,300));
    profile_phiPtcMet_ptcMet3.push_back(fs->make<TProfile>((v->getParameter<std::string>("name")).append("_phiPtcMet_ptcMet3").c_str(),"ptcMet", 30, -M_PI, M_PI, -300,300));
    profile_phiPtcMet_ptcMet4.push_back(fs->make<TProfile>((v->getParameter<std::string>("name")).append("_phiPtcMet_ptcMet4").c_str(),"ptcMet", 30, -M_PI, M_PI, -300,300));
    profile_phiPtcMet_ptcMet5.push_back(fs->make<TProfile>((v->getParameter<std::string>("name")).append("_phiPtcMet_ptcMet5").c_str(),"ptcMet", 30, -M_PI, M_PI, -300,300));
    profile_phiPtcMet_ptcMet6.push_back(fs->make<TProfile>((v->getParameter<std::string>("name")).append("_phiPtcMet_ptcMet6").c_str(),"ptcMet", 30, -M_PI, M_PI, -300,300));
    profile_phiPtcMet_ptcMet7.push_back(fs->make<TProfile>((v->getParameter<std::string>("name")).append("_phiPtcMet_ptcMet7").c_str(),"ptcMet", 30, -M_PI, M_PI, -300,300));
    profile_phiPtcMet_ptcMet8.push_back(fs->make<TProfile>((v->getParameter<std::string>("name")).append("_phiPtcMet_ptcMet8").c_str(),"ptcMet", 30, -M_PI, M_PI, -300,300));
    profile_phiPtcMet_ptcMet9.push_back(fs->make<TProfile>((v->getParameter<std::string>("name")).append("_phiPtcMet_ptcMet9").c_str(),"ptcMet", 30, -M_PI, M_PI, -300,300));
    profile_phiPtcMet_ptcMet10.push_back(fs->make<TProfile>((v->getParameter<std::string>("name")).append("_phiPtcMet_ptcMet10").c_str(),"ptcMet", 30, -M_PI, M_PI, -300,300));
    profile_phiPtcMet_ptcMet11.push_back(fs->make<TProfile>((v->getParameter<std::string>("name")).append("_phiPtcMet_ptcMet11").c_str(),"ptcMet", 30, -M_PI, M_PI, -300,300));


    profile_phiPtcMet_nvtx1.push_back(fs->make<TProfile>((v->getParameter<std::string>("name")).append("_phiPtcMet_nvtx1").c_str(),"ptcMet", 30, -M_PI, M_PI, -300,300));
    profile_phiPtcMet_nvtx2.push_back(fs->make<TProfile>((v->getParameter<std::string>("name")).append("_phiPtcMet_nvtx2").c_str(),"ptcMet", 30, -M_PI, M_PI, -300,300));
    profile_phiPtcMet_nvtx3.push_back(fs->make<TProfile>((v->getParameter<std::string>("name")).append("_phiPtcMet_nvtx3").c_str(),"ptcMet", 30, -M_PI, M_PI, -300,300));
    profile_phiPtcMet_nvtx4.push_back(fs->make<TProfile>((v->getParameter<std::string>("name")).append("_phiPtcMet_nvtx4").c_str(),"ptcMet", 30, -M_PI, M_PI, -300,300));
    profile_phiPtcMet_nvtx5.push_back(fs->make<TProfile>((v->getParameter<std::string>("name")).append("_phiPtcMet_nvtx5").c_str(),"ptcMet", 30, -M_PI, M_PI, -300,300));
    profile_phiPtcMet_nvtx6.push_back(fs->make<TProfile>((v->getParameter<std::string>("name")).append("_phiPtcMet_nvtx6").c_str(),"ptcMet", 30, -M_PI, M_PI, -300,300));
    profile_phiPtcMet_nvtx7.push_back(fs->make<TProfile>((v->getParameter<std::string>("name")).append("_phiPtcMet_nvtx7").c_str(),"ptcMet", 30, -M_PI, M_PI, -300,300));
    profile_phiPtcMet_nvtx8.push_back(fs->make<TProfile>((v->getParameter<std::string>("name")).append("_phiPtcMet_nvtx8").c_str(),"ptcMet", 30, -M_PI, M_PI, -300,300));
    profile_phiPtcMet_nvtx9.push_back(fs->make<TProfile>((v->getParameter<std::string>("name")).append("_phiPtcMet_nvtx9").c_str(),"ptcMet", 30, -M_PI, M_PI, -300,300));
    profile_phiPtcMet_nvtx10.push_back(fs->make<TProfile>((v->getParameter<std::string>("name")).append("_phiPtcMet_nvtx10").c_str(),"ptcMet", 30, -M_PI, M_PI, -300,300));
    profile_phiPtcMet_nvtx11.push_back(fs->make<TProfile>((v->getParameter<std::string>("name")).append("_phiPtcMet_nvtx11").c_str(),"ptcMet", 30, -M_PI, M_PI, -300,300));



    h2_met_varType_x.push_back(fs->make<TH2F>(std::string("h2_met").append("_").append(namePostFix(varType)).append("_").append(v->getParameter<std::string>("name")).append("_MET_x").c_str(),std::string("h2_met_").append(namePostFix(varType)).append("_MET_x").c_str(),  100, 0, 300, nbins, nMin, nMax));
    h2_met_varType_y.push_back(fs->make<TH2F>(std::string("h2_met").append("_").append(namePostFix(varType)).append("_").append(v->getParameter<std::string>("name")).append("_MET_y").c_str(),std::string("h2_met_").append(namePostFix(varType)).append("_MET_y").c_str(),  100, 0, 300, nbins, nMin, nMax));


    occupancy_.push_back(fs->make<TH2F>(std::string(moduleLabel_).append("_").append(namePostFix(varType)).append("_").append(v->getParameter<std::string>("name")).append("_occupancy").c_str(),"occupancy",  etaNBins, etaMin, etaMax, phiNBins, phiMin, phiMax));
    energy_.push_back(fs->make<TH2F>(std::string(moduleLabel_).append("_").append(namePostFix(varType)).append("_").append(v->getParameter<std::string>("name")).append("_energy").c_str(),"energy",           etaNBins, etaMin, etaMax, phiNBins, phiMin, phiMax));
    pt_.push_back(fs->make<TH2F>(std::string(moduleLabel_).append("_").append(namePostFix(varType)).append("_").append(v->getParameter<std::string>("name")).append("_pt").c_str(),"pt",                       etaNBins, etaMin, etaMax, phiNBins, phiMin, phiMax));
    variable_.push_back(fs->make<TH1F>(std::string(moduleLabel_).append("_").append(namePostFix(varType)).append("_").append(v->getParameter<std::string>("name")).append("_variable").c_str(),"variable", nbins, nMin, nMax));

    prof_ptcMetVsNvtx_MEtX.push_back(fs->make<TProfile2D>(std::string(moduleLabel_).append("_").append(namePostFix(varType)).append("_").append(v->getParameter<std::string>("name")).append("_ptcMetVsNvtx_MEtX").c_str(),"ptcMetVsNvtx_MEtX",  30, 0, 100, 15, 0, 30));
    prof_ptcMetVsNvtx_MEtY.push_back(fs->make<TProfile2D>(std::string(moduleLabel_).append("_").append(namePostFix(varType)).append("_").append(v->getParameter<std::string>("name")).append("_ptcMetVsNvtx_MEtY").c_str(),"ptcMetVsNvtx_MEtY",  30, 0, 100, 15, 0, 30));

  }


  for (unsigned i(0); i<36; i++) {
    prof_MetX_Nvtx.push_back(fs->make<TProfile>(std::string("prof_MetX_Nvtx").append(std::to_string(i)).c_str(),"MetX_Nvtx",50,0,150));
    prof_MetY_Nvtx.push_back(fs->make<TProfile>(std::string("prof_MetY_Nvtx").append(std::to_string(i)).c_str(),"MetY_Nvtx",50,0,150));

    prof_MetX_Pt1Nvtx.push_back(fs->make<TProfile>(std::string("prof_MetX_Pt1Nvtx").append(std::to_string(i)).c_str(),"MetX_Nvtx",50,0,150));
    prof_MetY_Pt1Nvtx.push_back(fs->make<TProfile>(std::string("prof_MetY_Pt1Nvtx").append(std::to_string(i)).c_str(),"MetY_Nvtx",50,0,150));
    prof_MetX_Pt2Nvtx.push_back(fs->make<TProfile>(std::string("prof_MetX_Pt2Nvtx").append(std::to_string(i)).c_str(),"MetX_Nvtx",50,0,150));
    prof_MetY_Pt2Nvtx.push_back(fs->make<TProfile>(std::string("prof_MetY_Pt2Nvtx").append(std::to_string(i)).c_str(),"MetY_Nvtx",50,0,150));
    prof_MetX_Pt3Nvtx.push_back(fs->make<TProfile>(std::string("prof_MetX_Pt3Nvtx").append(std::to_string(i)).c_str(),"MetX_Nvtx",50,0,150));
    prof_MetY_Pt3Nvtx.push_back(fs->make<TProfile>(std::string("prof_MetY_Pt3Nvtx").append(std::to_string(i)).c_str(),"MetY_Nvtx",50,0,150));
    prof_MetX_Pt4Nvtx.push_back(fs->make<TProfile>(std::string("prof_MetX_Pt4Nvtx").append(std::to_string(i)).c_str(),"MetX_Nvtx",50,0,150));
    prof_MetY_Pt4Nvtx.push_back(fs->make<TProfile>(std::string("prof_MetY_Pt4Nvtx").append(std::to_string(i)).c_str(),"MetY_Nvtx",50,0,150));
    prof_MetX_Pt5Nvtx.push_back(fs->make<TProfile>(std::string("prof_MetX_Pt5Nvtx").append(std::to_string(i)).c_str(),"MetX_Nvtx",50,0,150));
    prof_MetY_Pt5Nvtx.push_back(fs->make<TProfile>(std::string("prof_MetY_Pt5Nvtx").append(std::to_string(i)).c_str(),"MetY_Nvtx",50,0,150));
    prof_MetX_Pt6Nvtx.push_back(fs->make<TProfile>(std::string("prof_MetX_Pt6Nvtx").append(std::to_string(i)).c_str(),"MetX_Nvtx",50,0,150));
    prof_MetY_Pt6Nvtx.push_back(fs->make<TProfile>(std::string("prof_MetY_Pt6Nvtx").append(std::to_string(i)).c_str(),"MetY_Nvtx",50,0,150));
    prof_MetX_Pt7Nvtx.push_back(fs->make<TProfile>(std::string("prof_MetX_Pt7Nvtx").append(std::to_string(i)).c_str(),"MetX_Nvtx",50,0,150));
    prof_MetY_Pt7Nvtx.push_back(fs->make<TProfile>(std::string("prof_MetY_Pt7Nvtx").append(std::to_string(i)).c_str(),"MetY_Nvtx",50,0,150));
  }
}
void
metPhiCorrInfoWriter::beginJob(){
#if(MkTree)
  OutTree = fs->make<TTree>("Events", "Events");
  OutTree->Branch("Count_catagory",       &Count_catagory);
  OutTree->Branch("Count_counts",       &Count_counts);
  OutTree->Branch("Count_MetX",       &Count_MetX);
  OutTree->Branch("Count_MetY",       &Count_MetY);
  OutTree->Branch("Count_pfMetT",       &Count_pfMetT);
  OutTree->Branch("Count_pfMetX",       &Count_pfMetX);
  OutTree->Branch("Count_pfMetY",       &Count_pfMetY);
  OutTree->Branch("nVtx_catagory",     &nVtx_catagory);
  OutTree->Branch("nVtx_nVtx",         &nVtx_nVtx);
  OutTree->Branch("nVtx_MetX",         &nVtx_MetX);
  OutTree->Branch("nVtx_MetY",         &nVtx_MetY);
  OutTree->Branch("nVtx_pfMetT",       &nVtx_pfMetT);
  OutTree->Branch("nVtx_pfMetX",       &nVtx_pfMetX);
  OutTree->Branch("nVtx_pfMetY",       &nVtx_pfMetY);
  OutTree->Branch("sumPt_catagory",     &sumPt_catagory);
  OutTree->Branch("sumPt_sumPt",        &sumPt_sumPt);
  OutTree->Branch("sumPt_MetX",         &sumPt_MetX);
  OutTree->Branch("sumPt_MetY",         &sumPt_MetY);
  OutTree->Branch("sumPt_pfMetT",       &sumPt_pfMetT);
  OutTree->Branch("sumPt_pfMetX",       &sumPt_pfMetX);
  OutTree->Branch("sumPt_pfMetY",       &sumPt_pfMetY);
#endif

}
void metPhiCorrInfoWriter::analyze( const edm::Event& evt, const edm::EventSetup& setup) {

  edm::Handle<patMETCollection> srcPatMETCollection;
  evt.getByToken(metToken_, srcPatMETCollection);
  edm::Handle<pat::MuonCollection> Muons;
  evt.getByToken(MuonLabel_, Muons);

  const pat::MET& srcMET = (*srcPatMETCollection)[0];
  pfMet_px = srcMET.px();
  pfMet_py = srcMET.py();
  pfMet_pt = srcMET.pt();
  pfMet_phi= srcMET.phi();
  //get primary vertices
  edm::Handle< reco::VertexCollection > hpv;
  try {
//    evt.getByLabel( vertices_, hpv );
    evt.getByToken( verticesToken_, hpv );
  } catch ( cms::Exception & e ) {
    std::cout <<"[metPhiCorrInfoWriter] error: " << e.what() << std::endl;
  }
  std::vector<reco::Vertex> goodVertices;
  for (unsigned i = 0; i < hpv->size(); i++) {
    if ( (*hpv)[i].ndof() > 4 &&
       ( fabs((*hpv)[i].z()) <= 24. ) &&
       ( fabs((*hpv)[i].position().rho()) <= 2.0 ) )
       goodVertices.push_back((*hpv)[i]);
  }
  int ngoodVtx = goodVertices.size();

  h1_met->Fill(pfMet_pt);
  h1_nvtx->Fill(ngoodVtx);
  h2_met_nvtx_MEtX->Fill(pfMet_pt, ngoodVtx, pfMet_px);
  h2_met_nvtx_MEtY->Fill(pfMet_pt, ngoodVtx, pfMet_py);
  profile_MET_x->Fill(pfMet_pt, pfMet_px);
  profile_MET_y->Fill(pfMet_pt, pfMet_py);
  profile_nvtx_x->Fill(ngoodVtx, pfMet_px);
  profile_nvtx_y->Fill(ngoodVtx, pfMet_py);
  profile_pfMet_phi->Fill(pfMet_phi, pfMet_pt);
  if(ngoodVtx > 0  && ngoodVtx <= 5  ) profile_pfMet_phi_vtx1->Fill(pfMet_phi, pfMet_pt);
  if(ngoodVtx > 5  && ngoodVtx <= 10 ) profile_pfMet_phi_vtx2->Fill(pfMet_phi, pfMet_pt);
  if(ngoodVtx > 10 && ngoodVtx <= 15 ) profile_pfMet_phi_vtx3->Fill(pfMet_phi, pfMet_pt);
  if(ngoodVtx > 15 && ngoodVtx <= 20 ) profile_pfMet_phi_vtx4->Fill(pfMet_phi, pfMet_pt);
  if(ngoodVtx > 20 && ngoodVtx <= 25 ) profile_pfMet_phi_vtx5->Fill(pfMet_phi, pfMet_pt);
  if(ngoodVtx > 25 && ngoodVtx <= 30 ) profile_pfMet_phi_vtx6->Fill(pfMet_phi, pfMet_pt);
  if(ngoodVtx > 30 && ngoodVtx <= 35 ) profile_pfMet_phi_vtx7->Fill(pfMet_phi, pfMet_pt);
  if(ngoodVtx > 35                   ) profile_pfMet_phi_vtx8->Fill(pfMet_phi, pfMet_pt);

  // Nvtx cut
  for (int i(0); i< 35 ; i++){
    if(ngoodVtx == i){
      prof_MetX_Nvtx[i]->Fill( pfMet_pt, pfMet_px);
      prof_MetY_Nvtx[i]->Fill( pfMet_pt, pfMet_py);
    }
  }
  if(ngoodVtx > 34 ){
    prof_MetX_Nvtx[35]->Fill( pfMet_pt, pfMet_px);
    prof_MetY_Nvtx[35]->Fill( pfMet_pt, pfMet_py);
  }
  // Nvtx & MuPt cut
  for(unsigned i(0);i<Muons->size();i++){
    auto& mu = Muons->at(i);
    if(!mu.isPFMuon()) continue;
    if(!(mu.isGlobalMuon() || mu.isTrackerMuon()) ) continue;
    if(fabs(mu.eta())>2.4 ) continue;
    if(mu.pt() < 10){
      for (int i(0); i< 35 ; i++){
        if(ngoodVtx == i){
          prof_MetX_Pt1Nvtx[i]->Fill( pfMet_pt, pfMet_px);
          prof_MetY_Pt1Nvtx[i]->Fill( pfMet_pt, pfMet_py);
        }
      }
      if(ngoodVtx > 34 ){
        prof_MetX_Pt1Nvtx[35]->Fill( pfMet_pt, pfMet_px);
        prof_MetY_Pt1Nvtx[35]->Fill( pfMet_pt, pfMet_py);
      }
    }
    if(mu.pt() >= 10 && mu.pt() < 20){
      for (int i(0); i< 35 ; i++){
        if(ngoodVtx == i){
          prof_MetX_Pt2Nvtx[i]->Fill( pfMet_pt, pfMet_px);
          prof_MetY_Pt2Nvtx[i]->Fill( pfMet_pt, pfMet_py);
        }
      }
      if(ngoodVtx > 34 ){
        prof_MetX_Pt2Nvtx[35]->Fill( pfMet_pt, pfMet_px);
        prof_MetY_Pt2Nvtx[35]->Fill( pfMet_pt, pfMet_py);
      }
    }
    if(mu.pt() >= 20 && mu.pt() < 30){
      for (int i(0); i< 35 ; i++){
        if(ngoodVtx == i){
          prof_MetX_Pt3Nvtx[i]->Fill( pfMet_pt, pfMet_px);
          prof_MetY_Pt3Nvtx[i]->Fill( pfMet_pt, pfMet_py);
        }
      }
      if(ngoodVtx > 34 ){
        prof_MetX_Pt3Nvtx[35]->Fill( pfMet_pt, pfMet_px);
        prof_MetY_Pt3Nvtx[35]->Fill( pfMet_pt, pfMet_py);
      }
    }
    if(mu.pt() >= 30 && mu.pt() < 40){
      for (int i(0); i< 35 ; i++){
        if(ngoodVtx == i){
          prof_MetX_Pt4Nvtx[i]->Fill( pfMet_pt, pfMet_px);
          prof_MetY_Pt4Nvtx[i]->Fill( pfMet_pt, pfMet_py);
        }
      }
      if(ngoodVtx > 34 ){
        prof_MetX_Pt4Nvtx[35]->Fill( pfMet_pt, pfMet_px);
        prof_MetY_Pt4Nvtx[35]->Fill( pfMet_pt, pfMet_py);
      }
    }
    if(mu.pt() >= 40 && mu.pt() < 50){
      for (int i(0); i< 35 ; i++){
        if(ngoodVtx == i){
          prof_MetX_Pt5Nvtx[i]->Fill( pfMet_pt, pfMet_px);
          prof_MetY_Pt5Nvtx[i]->Fill( pfMet_pt, pfMet_py);
        }
      }
      if(ngoodVtx > 34 ){
        prof_MetX_Pt5Nvtx[35]->Fill( pfMet_pt, pfMet_px);
        prof_MetY_Pt5Nvtx[35]->Fill( pfMet_pt, pfMet_py);
      }
    }
    if(mu.pt() >= 50 && mu.pt() < 60){
      for (int i(0); i< 35 ; i++){
        if(ngoodVtx == i){
          prof_MetX_Pt6Nvtx[i]->Fill( pfMet_pt, pfMet_px);
          prof_MetY_Pt6Nvtx[i]->Fill( pfMet_pt, pfMet_py);
        }
      }
      if(ngoodVtx > 34 ){
        prof_MetX_Pt6Nvtx[35]->Fill( pfMet_pt, pfMet_px);
        prof_MetY_Pt6Nvtx[35]->Fill( pfMet_pt, pfMet_py);
      }
    }
    if(mu.pt() >= 60 ){
      for (int i(0); i< 35 ; i++){
        if(ngoodVtx == i){
          prof_MetX_Pt7Nvtx[i]->Fill( pfMet_pt, pfMet_px);
          prof_MetY_Pt7Nvtx[i]->Fill( pfMet_pt, pfMet_py);
        }
      }
      if(ngoodVtx > 34 ){
        prof_MetX_Pt7Nvtx[35]->Fill( pfMet_pt, pfMet_px);
        prof_MetY_Pt7Nvtx[35]->Fill( pfMet_pt, pfMet_py);
      }
    }
  }


  if(ngoodVtx > 0  && ngoodVtx <= 1  ) profile_pfMet_phi_vtx1->Fill(pfMet_phi, pfMet_pt);
  if(ngoodVtx > 5  && ngoodVtx <= 10 ) profile_pfMet_phi_vtx2->Fill(pfMet_phi, pfMet_pt);
  if(ngoodVtx > 10 && ngoodVtx <= 15 ) profile_pfMet_phi_vtx3->Fill(pfMet_phi, pfMet_pt);
  if(ngoodVtx > 15 && ngoodVtx <= 20 ) profile_pfMet_phi_vtx4->Fill(pfMet_phi, pfMet_pt);
  if(ngoodVtx > 20 && ngoodVtx <= 25 ) profile_pfMet_phi_vtx5->Fill(pfMet_phi, pfMet_pt);
  if(ngoodVtx > 25 && ngoodVtx <= 30 ) profile_pfMet_phi_vtx6->Fill(pfMet_phi, pfMet_pt);
  if(ngoodVtx > 30 && ngoodVtx <= 35 ) profile_pfMet_phi_vtx7->Fill(pfMet_phi, pfMet_pt);
  if(ngoodVtx > 35                   ) profile_pfMet_phi_vtx8->Fill(pfMet_phi, pfMet_pt);

  for (unsigned i=0;i<counts_.size();i++) {
    counts_[i]=0;
    sumPt_[i]=0;
    MEx_[i]=0.;
    MEy_[i]=0.;
  } 

  edm::Handle< edm::View<reco::Candidate> > particleFlow;
  evt.getByToken( pflowToken_, particleFlow );

  for (unsigned i = 0; i < particleFlow->size(); ++i) {
    const reco::Candidate& c = particleFlow->at(i);
    for (unsigned j=0; j<type_.size(); j++) {
//      if (abs(c.pdgId())==211) {
//        std::cout<<"cand pdgId "<<c.pdgId()<<" testing type:"<<type_[j]<<" translated to pdg:"<<translateTypeToAbsPdgId(reco::PFCandidate::ParticleType(type_[j]))<<std::endl;
//      }
      if (abs(c.pdgId())== translateTypeToAbsPdgId(reco::PFCandidate::ParticleType(type_[j]))) {
        if ((c.eta()>etaMin_[j]) and (c.eta()<etaMax_[j])) {
          counts_[j]+=1;
          sumPt_[j]+=c.pt();
          MEx_[j]-=c.px();
          MEy_[j]-=c.py();

          pt_[j]->Fill(c.eta(), c.phi(), c.pt()); 
          energy_[j]->Fill(c.eta(), c.phi(), c.energy()); 
          occupancy_[j]->Fill(c.eta(), c.phi()); 

        }
      }
    }
  }
  for (unsigned j=0; j<type_.size(); j++) {
    if(varType_[j]==4) {
      tv2_ptcMet->Set(MEx_[j], MEy_[j]);

      ptcMet = tv2_ptcMet->Mod();
      if(ptcMet == 0 ) continue;
      //std::cout<<"tv2_ptcMet phi:"<<tv2_ptcMet->Phi_mpi_pi(tv2_ptcMet->Phi())<<std::endl;
      ptcMetPhi=tv2_ptcMet->Phi_mpi_pi(tv2_ptcMet->Phi());
      //std::cout<<"c phi:"<<c.phi()<<std::endl;
      if(ptcMetPhi < 0){
        usedPhi = M_PI + ptcMetPhi;
        usedPt = -1*ptcMet;
      }else {
        usedPhi = ptcMetPhi;
        usedPt = ptcMet;
      }
      //std::cout<<"ptc phi: "<<ptcPhi<<" usedPhi: "<<usedPhi<<" M_PI"<<M_PI<<std::endl;

      profile_phiPtcMet[j]->Fill(ptcMetPhi, ptcMet);
      if     ( ptcMet > 0  && ptcMet <= 5 ){profile_phiPtcMetVal_ptcMet1[j]->Fill(usedPhi, usedPt);profile_phiPtcMet_ptcMet1[j]->Fill(ptcMetPhi,ptcMet);}
      else if( ptcMet > 5  && ptcMet <= 10){profile_phiPtcMetVal_ptcMet2[j]->Fill(usedPhi, usedPt);profile_phiPtcMet_ptcMet2[j]->Fill(ptcMetPhi,ptcMet);}
      else if( ptcMet > 10 && ptcMet <= 15){profile_phiPtcMetVal_ptcMet3[j]->Fill(usedPhi, usedPt);profile_phiPtcMet_ptcMet3[j]->Fill(ptcMetPhi,ptcMet);}
      else if( ptcMet > 15 && ptcMet <= 20){profile_phiPtcMetVal_ptcMet4[j]->Fill(usedPhi, usedPt);profile_phiPtcMet_ptcMet4[j]->Fill(ptcMetPhi,ptcMet);}
      else if( ptcMet > 20 && ptcMet <= 25){profile_phiPtcMetVal_ptcMet5[j]->Fill(usedPhi, usedPt);profile_phiPtcMet_ptcMet5[j]->Fill(ptcMetPhi,ptcMet);}
      else if( ptcMet > 25 && ptcMet <= 30){profile_phiPtcMetVal_ptcMet6[j]->Fill(usedPhi, usedPt);profile_phiPtcMet_ptcMet6[j]->Fill(ptcMetPhi,ptcMet);}
      else if( ptcMet > 30 && ptcMet <= 35){profile_phiPtcMetVal_ptcMet7[j]->Fill(usedPhi, usedPt);profile_phiPtcMet_ptcMet7[j]->Fill(ptcMetPhi,ptcMet);}
      else if( ptcMet > 35 && ptcMet <= 40){profile_phiPtcMetVal_ptcMet8[j]->Fill(usedPhi, usedPt);profile_phiPtcMet_ptcMet8[j]->Fill(ptcMetPhi,ptcMet);}
      else if( ptcMet > 40 && ptcMet <= 45){profile_phiPtcMetVal_ptcMet9[j]->Fill(usedPhi, usedPt);profile_phiPtcMet_ptcMet9[j]->Fill(ptcMetPhi,ptcMet);}
      else if( ptcMet > 45 && ptcMet <= 50){profile_phiPtcMetVal_ptcMet10[j]->Fill(usedPhi, usedPt);profile_phiPtcMet_ptcMet10[j]->Fill(ptcMetPhi,ptcMet);}
      else if( ptcMet > 50 && ptcMet <= 55){profile_phiPtcMetVal_ptcMet11[j]->Fill(usedPhi, usedPt);profile_phiPtcMet_ptcMet11[j]->Fill(ptcMetPhi,ptcMet);}


      if     ( ngoodVtx > 0  && ngoodVtx <= 3) {profile_phiPtcMetVal_nvtx1[j]->Fill(usedPhi, usedPt); profile_phiPtcMet_nvtx1[j]->Fill(ptcMetPhi,ptcMet);}
      else if( ngoodVtx > 3  && ngoodVtx <= 6 ){profile_phiPtcMetVal_nvtx2[j]->Fill(usedPhi, usedPt); profile_phiPtcMet_nvtx2[j]->Fill(ptcMetPhi,ptcMet);}
      else if( ngoodVtx > 6  && ngoodVtx <= 9 ){profile_phiPtcMetVal_nvtx3[j]->Fill(usedPhi, usedPt); profile_phiPtcMet_nvtx3[j]->Fill(ptcMetPhi,ptcMet);}
      else if( ngoodVtx > 9  && ngoodVtx <= 12){profile_phiPtcMetVal_nvtx4[j]->Fill(usedPhi, usedPt); profile_phiPtcMet_nvtx4[j]->Fill(ptcMetPhi,ptcMet);}
      else if( ngoodVtx > 12 && ngoodVtx <= 15){profile_phiPtcMetVal_nvtx5[j]->Fill(usedPhi, usedPt); profile_phiPtcMet_nvtx5[j]->Fill(ptcMetPhi,ptcMet);}
      else if( ngoodVtx > 15 && ngoodVtx <= 18){profile_phiPtcMetVal_nvtx6[j]->Fill(usedPhi, usedPt); profile_phiPtcMet_nvtx6[j]->Fill(ptcMetPhi,ptcMet);}
      else if( ngoodVtx > 18 && ngoodVtx <= 21){profile_phiPtcMetVal_nvtx7[j]->Fill(usedPhi, usedPt); profile_phiPtcMet_nvtx7[j]->Fill(ptcMetPhi,ptcMet);}
      else if( ngoodVtx > 21 && ngoodVtx <= 24){profile_phiPtcMetVal_nvtx8[j]->Fill(usedPhi, usedPt); profile_phiPtcMet_nvtx8[j]->Fill(ptcMetPhi,ptcMet);}
      else if( ngoodVtx > 24 && ngoodVtx <= 27){profile_phiPtcMetVal_nvtx9[j]->Fill(usedPhi, usedPt); profile_phiPtcMet_nvtx9[j]->Fill(ptcMetPhi,ptcMet);}
      else if( ngoodVtx > 27 && ngoodVtx <= 30){profile_phiPtcMetVal_nvtx10[j]->Fill(usedPhi, usedPt); profile_phiPtcMet_nvtx10[j]->Fill(ptcMetPhi,ptcMet);}
      else if( ngoodVtx > 30 && ngoodVtx <= 33){profile_phiPtcMetVal_nvtx11[j]->Fill(usedPhi, usedPt); profile_phiPtcMet_nvtx11[j]->Fill(ptcMetPhi,ptcMet);}

      //profile_phi_ptValence[j]->Fill(usedPhi, usedPt);
      prof_ptcMetVsNvtx_MEtX[j]->Fill(ptcMet,ngoodVtx, MEx_[j]);
      prof_ptcMetVsNvtx_MEtY[j]->Fill(ptcMet,ngoodVtx, MEy_[j]);

      profile_x_[j]->Fill(ptcMet, MEx_[j]);
      profile_y_[j]->Fill(ptcMet, MEy_[j]);


      variable_[j]->Fill(ptcMet);
    }
  }


  Count_catagory.clear();
  Count_counts.clear();
  Count_MetX.clear();
  Count_MetY.clear();
  Count_pfMetT.clear();
  Count_pfMetX.clear();
  Count_pfMetY.clear();

  nVtx_catagory.clear();
  nVtx_nVtx.clear();
  nVtx_MetX.clear();
  nVtx_MetY.clear();
  nVtx_pfMetT.clear();
  nVtx_pfMetX.clear();
  nVtx_pfMetY.clear();

  sumPt_catagory.clear();
  sumPt_sumPt.clear();
  sumPt_MetX.clear();
  sumPt_MetY.clear();
  sumPt_pfMetT.clear();
  sumPt_pfMetX.clear();
  sumPt_pfMetY.clear();

  for (std::vector<edm::ParameterSet>::const_iterator v = cfgCorrParameters_.begin(); v!=cfgCorrParameters_.end(); v++) {
    unsigned j=v-cfgCorrParameters_.begin();
    //std::cout<<"j "<<j<<" "<<v->getParameter<std::string>("name")<<" varType "<<varType_[j]<<" counts "<<counts_[j]<<" sumPt "<<sumPt_[j]<<" nvtx "<<ngoodVtx<<" "<<MEx_[j]<<" "<<MEy_[j]<<std::endl;

    if (varType_[j]==0) {

      //for( int k(0); k<MetBin;k++)
      //{
      //  if(k==MetBin-1){
      //    if( pfMet_pt > k*10 )
      //    {
      //      profileMetBin_x_[j*MetBin + k ]->Fill(counts_[j], MEx_[j]);
      //      profileMetBin_y_[j*MetBin + k ]->Fill(counts_[j], MEy_[j]);
      //    }
      //  }else{
      //    if( pfMet_pt > k*10 && pfMet_pt < (k+1)*10 )
      //    {
      //      profileMetBin_x_[j*MetBin + k ]->Fill(counts_[j], MEx_[j]);
      //      profileMetBin_y_[j*MetBin + k ]->Fill(counts_[j], MEy_[j]);
      //    }
      //  }
      //}
      profile_x_[j]->Fill(counts_[j], MEx_[j]);
      profile_y_[j]->Fill(counts_[j], MEy_[j]);
      variable_[j]->Fill(counts_[j]);
      h2_met_varType_x[j]->Fill(pfMet_pt,counts_[j], MEx_[j]);
      h2_met_varType_y[j]->Fill(pfMet_pt,counts_[j], MEy_[j]);

#if(MkTree)
      Count_catagory.push_back(j);
      Count_counts.push_back(counts_[j]);
      Count_MetX.push_back(MEx_[j]);
      Count_MetY.push_back(MEy_[j]);
      Count_pfMetT.push_back(pfMet_pt);
      Count_pfMetX.push_back(pfMet_px);
      Count_pfMetY.push_back(pfMet_py);
#endif
    } 
    if (varType_[j]==1) {

      //for( int k(0); k<MetBin;k++)
      //{
      //  if(k==MetBin-1){
      //    if( pfMet_pt > k*10 )
      //    {
      //      profileMetBin_x_[j*MetBin + k ]->Fill(ngoodVtx, MEx_[j]);
      //      profileMetBin_y_[j*MetBin + k ]->Fill(ngoodVtx, MEy_[j]);
      //    }
      //  }else{
      //    if( pfMet_pt > k*10 && pfMet_pt < (k+1)*10 )
      //    {
      //      profileMetBin_x_[j*MetBin + k ]->Fill(ngoodVtx, MEx_[j]);
      //      profileMetBin_y_[j*MetBin + k ]->Fill(ngoodVtx, MEy_[j]);
      //    }
      //  }
      //}
      profile_x_[j]->Fill(ngoodVtx, MEx_[j]);
      profile_y_[j]->Fill(ngoodVtx, MEy_[j]);
      variable_[j]->Fill(ngoodVtx);
      h2_met_varType_x[j]->Fill(pfMet_pt,ngoodVtx, MEx_[j]);
      h2_met_varType_y[j]->Fill(pfMet_pt,ngoodVtx, MEy_[j]);

#if(MkTree)
      nVtx_catagory.push_back(j);
      nVtx_nVtx.push_back(ngoodVtx);
      nVtx_MetX.push_back(MEx_[j]);
      nVtx_MetY.push_back(MEy_[j]);
      nVtx_pfMetT.push_back(pfMet_pt);
      nVtx_pfMetX.push_back(pfMet_px);
      nVtx_pfMetY.push_back(pfMet_py);
#endif
    } 
    if (varType_[j]==2) {

      //for( int k(0); k<MetBin;k++)
      //{
      //  if(k==MetBin-1){
      //    if( pfMet_pt > k*10 )
      //    {
      //      profileMetBin_x_[j*MetBin + k ]->Fill(sumPt_[j], MEx_[j]);
      //      profileMetBin_y_[j*MetBin + k ]->Fill(sumPt_[j], MEy_[j]);
      //    }
      //  }else{
      //    if( pfMet_pt > k*10 && pfMet_pt < (k+1)*10 )
      //    {
      //      profileMetBin_x_[j*MetBin + k ]->Fill(sumPt_[j], MEx_[j]);
      //      profileMetBin_y_[j*MetBin + k ]->Fill(sumPt_[j], MEy_[j]);
      //    }
      //  }
      //}

      profile_x_[j]->Fill(sumPt_[j], MEx_[j]);
      profile_y_[j]->Fill(sumPt_[j], MEy_[j]);
      variable_[j]->Fill(sumPt_[j]);
      h2_met_varType_x[j]->Fill(pfMet_pt,sumPt_[j], MEx_[j]);
      h2_met_varType_y[j]->Fill(pfMet_pt,sumPt_[j], MEy_[j]);

#if(MkTree)
      sumPt_catagory.push_back(j);
      sumPt_sumPt.push_back(sumPt_[j]);
      sumPt_MetX.push_back(MEx_[j]);
      sumPt_MetY.push_back(MEy_[j]);
      sumPt_pfMetT.push_back(pfMet_pt);
      sumPt_pfMetX.push_back(pfMet_px);
      sumPt_pfMetY.push_back(pfMet_py);
#endif
    }
    if (varType_[j]==3) {

      profile_x_[j]->Fill(pfMet_pt, MEx_[j]);
      profile_y_[j]->Fill(pfMet_pt, MEy_[j]);

    }
  }
#if(MkTree)
  OutTree->Fill();
#endif
}

//define this as a plug-in
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(metPhiCorrInfoWriter);

