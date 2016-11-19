#include "MetTools/MetPhiCorrections/plugins/metPhiCorrInfoWriter.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/Association.h"
#include <string>

std::string namePostFix (int varType) {

  if (varType==0) return std::string("multiplicity");
  if (varType==1) return std::string("ngoodVertices");
  if (varType==2) return std::string("sumPt");
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
  moduleLabel_(cfg.getParameter<std::string>("@module_label"))
{

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
void
metPhiCorrInfoWriter::beginJob(){
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

}
void metPhiCorrInfoWriter::analyze( const edm::Event& evt, const edm::EventSetup& setup) {

  edm::Handle<patMETCollection> srcPatMETCollection;
  evt.getByToken(metToken_, srcPatMETCollection);
  const pat::MET& srcMET = (*srcPatMETCollection)[0];
  pfMet_px = srcMET.px();
  pfMet_py = srcMET.py();
  pfMet_pt = srcMET.pt();
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
    //std::cout<<"j "<<j<<" "<<v->getParameter<std::string>("name")<<" varType "<<varType_[j]<<" counts "<<counts_[j]<<" sumPt "<<sumPt_[j]<<" nvtx "<<ngoodVertices<<" "<<MEx_[j]<<" "<<MEy_[j]<<std::endl;

    if (varType_[j]==0) {
      profile_x_[j]->Fill(counts_[j], MEx_[j]);
      profile_y_[j]->Fill(counts_[j], MEy_[j]);
      variable_[j]->Fill(counts_[j]);
      Count_catagory.push_back(j);
      Count_counts.push_back(counts_[j]);
      Count_MetX.push_back(MEx_[j]);
      Count_MetY.push_back(MEy_[j]);
      Count_pfMetT.push_back(pfMet_pt);
      Count_pfMetX.push_back(pfMet_px);
      Count_pfMetY.push_back(pfMet_py);
    } 
    if (varType_[j]==1) {
      profile_x_[j]->Fill(ngoodVertices, MEx_[j]);
      profile_y_[j]->Fill(ngoodVertices, MEy_[j]);
      variable_[j]->Fill(ngoodVertices);
      nVtx_catagory.push_back(j);
      nVtx_nVtx.push_back(ngoodVertices);
      nVtx_MetX.push_back(MEx_[j]);
      nVtx_MetY.push_back(MEy_[j]);
      nVtx_pfMetT.push_back(pfMet_pt);
      nVtx_pfMetX.push_back(pfMet_px);
      nVtx_pfMetY.push_back(pfMet_py);
    } 
    if (varType_[j]==2) {
      profile_x_[j]->Fill(sumPt_[j], MEx_[j]);
      profile_y_[j]->Fill(sumPt_[j], MEy_[j]);
      variable_[j]->Fill(sumPt_[j]);

      sumPt_catagory.push_back(j);
      sumPt_sumPt.push_back(sumPt_[j]);
      sumPt_MetX.push_back(MEx_[j]);
      sumPt_MetY.push_back(MEy_[j]);
      sumPt_pfMetT.push_back(pfMet_pt);
      sumPt_pfMetX.push_back(pfMet_px);
      sumPt_pfMetY.push_back(pfMet_py);
    }
  }
  OutTree->Fill();
}

//define this as a plug-in
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(metPhiCorrInfoWriter);

