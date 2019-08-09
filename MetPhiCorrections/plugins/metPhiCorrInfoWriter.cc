#include "MetTools/MetPhiCorrections/plugins/metPhiCorrInfoWriter.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/Association.h"
#include <string>
#include <TLorentzVector.h>

std::string namePostFix (int varType) {

  if (varType==0) return std::string("multiplicity");
  if (varType==1) return std::string("ngoodVertices");
  if (varType==2) return std::string("sumPt");
  return std::string("unknown");
}

double delta_phi (float phi1, float phi2)
{
  double dPhi = phi1 - phi2; 
  if (dPhi > 3.1416) dPhi -= 2*3.1416;
  if (dPhi <= -3.1416) dPhi += 2*3.1416; //So that dPhi is always between -pi and +pi.
  return dPhi;
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

bool metPhiCorrInfoWriter::passSelection(TLorentzVector firstMuon, TLorentzVector secondMuon) {
  if((firstMuon + secondMuon).Pt() > 20) return false;
  if((firstMuon + secondMuon).M() > 110 or (firstMuon + secondMuon).M() < 70) return false;
  if(firstMuon.Pt()/secondMuon.Pt() > 1.2 or firstMuon.Pt()/secondMuon.Pt() < 0.8) return false;
  if(fabs(delta_phi(firstMuon.Phi(),secondMuon.Phi()))<2.8) return false;
  return true;
}

metPhiCorrInfoWriter::metPhiCorrInfoWriter( const edm::ParameterSet & cfg ): 
  vertices_ ( cfg.getUntrackedParameter< edm::InputTag >("vertexCollection") ),
  verticesToken_ ( consumes< reco::VertexCollection >(vertices_) ),
  muons_ ( cfg.getUntrackedParameter< edm::InputTag >("muonsCollection") ),
  muonsToken_(consumes< std::vector< pat::Muon> >(muons_)),
  pflow_ ( cfg.getUntrackedParameter< edm::InputTag >("srcPFlow") ),
  pflowToken_ ( consumes< edm::View<reco::Candidate> >(pflow_) ),
  moduleLabel_(cfg.getParameter<std::string>("@module_label"))
{
  edm::Service<TFileService> fs;

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
    profile_x_.push_back(fs->make<TProfile>(std::string(moduleLabel_).append("_").append(namePostFix(varType)).append("_").append(v->getParameter<std::string>("name")).append("_Px").c_str(),"Px", nbins, nMin, nMax, -300,300));
    profile_y_.push_back(fs->make<TProfile>(std::string(moduleLabel_).append("_").append(namePostFix(varType)).append("_").append(v->getParameter<std::string>("name")).append("_Py").c_str(),"Py", nbins, nMin, nMax, -300,300));

    occupancy_.push_back(fs->make<TH2F>(std::string(moduleLabel_).append("_").append(namePostFix(varType)).append("_").append(v->getParameter<std::string>("name")).append("_occupancy").c_str(),"occupancy",  etaNBins, etaMin, etaMax, phiNBins, phiMin, phiMax));
    energy_.push_back(fs->make<TH2F>(std::string(moduleLabel_).append("_").append(namePostFix(varType)).append("_").append(v->getParameter<std::string>("name")).append("_energy").c_str(),"energy",           etaNBins, etaMin, etaMax, phiNBins, phiMin, phiMax));
    pt_.push_back(fs->make<TH2F>(std::string(moduleLabel_).append("_").append(namePostFix(varType)).append("_").append(v->getParameter<std::string>("name")).append("_pt").c_str(),"pt",                       etaNBins, etaMin, etaMax, phiNBins, phiMin, phiMax));

    variable_.push_back(fs->make<TH1F>(std::string(moduleLabel_).append("_").append(namePostFix(varType)).append("_").append(v->getParameter<std::string>("name")).append("_variable").c_str(),"variable", nbins, nMin, nMax));
  }
}

void metPhiCorrInfoWriter::analyze( const edm::Event& evt, const edm::EventSetup& setup) {

  //Select muons compatible with a Z
  edm::Handle< std::vector< pat::Muon> > theMuons;
  evt.getByToken(muonsToken_,theMuons );

  std::vector<TLorentzVector> goodMuons;
  std::vector<int> muonCharges;
  bool isGoodEvent = false;
  for(std::vector<pat::Muon>::const_iterator muon = (*theMuons).begin(); muon != (*theMuons).end(); muon++) {
    if(!(&*muon)->passed(reco::Muon::CutBasedIdMedium)) continue;
    if(!(&*muon)->passed(reco::Muon::PFIsoMedium)) continue;
    TLorentzVector thisMuon;
    thisMuon.SetPtEtaPhiE((&*muon)->pt(),(&*muon)->eta(),(&*muon)->phi(),(&*muon)->energy());
    goodMuons.push_back(thisMuon);
    muonCharges.push_back((&*muon)->charge());
  }
//  std::cout << "======================================================================================================" << std::endl;
//  std::cout << "There are " << goodMuons.size() << " good muons." << std::endl;
  if(goodMuons.size()>1){
//    std::cout << "First muon has Pt = " << goodMuons[0].Pt() << ", eta = " << goodMuons[0].Eta() << ", phi = " << goodMuons[0].Phi() << ", E = " << goodMuons[0].E() << ", charge = " << muonCharges[0] << std::endl;
//    std::cout << "Second muon has Pt = " << goodMuons[1].Pt() << ", eta = " << goodMuons[1].Eta() << ", phi = " << goodMuons[1].Phi() << ", E = " << goodMuons[1].E() << ", charge = " << muonCharges[1] << std::endl;
    for(unsigned int first = 0 ; first < goodMuons.size()-1 ; first++){
      for(unsigned int second = 1 ; second < goodMuons.size() ; second++){
        if(passSelection(goodMuons[first],goodMuons[second]) and (muonCharges[first]+muonCharges[second] == 0)) isGoodEvent = true;
      }
    }
  }

  if(isGoodEvent){
//    std::cout << "This is a good event." << std::endl;
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
    int ngoodVertices = goodVertices.size();

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
    for (std::vector<edm::ParameterSet>::const_iterator v = cfgCorrParameters_.begin(); v!=cfgCorrParameters_.end(); v++) {
      unsigned j=v-cfgCorrParameters_.begin();
//    std::cout<<"j "<<j<<" "<<v->getParameter<std::string>("name")<<" varType "<<varType_[j]<<" counts "<<counts_[j]<<" sumPt "<<sumPt_[j]<<" nvtx "<<ngoodVertices<<" "<<MEx_[j]<<" "<<MEy_[j]<<std::endl;
      if (varType_[j]==0) {
        profile_x_[j]->Fill(counts_[j], MEx_[j]);
        profile_y_[j]->Fill(counts_[j], MEy_[j]);
        variable_[j]->Fill(counts_[j]);
      } 
      if (varType_[j]==1) {
        profile_x_[j]->Fill(ngoodVertices, MEx_[j]);
        profile_y_[j]->Fill(ngoodVertices, MEy_[j]);
        variable_[j]->Fill(ngoodVertices);
      } 
      if (varType_[j]==2) {
        profile_x_[j]->Fill(sumPt_[j], MEx_[j]);
        profile_y_[j]->Fill(sumPt_[j], MEy_[j]);
        variable_[j]->Fill(sumPt_[j]);
      }
    }
  }
}

//define this as a plug-in
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(metPhiCorrInfoWriter);

