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
  tv2_ptcMet = new TVector2();

  h1_met=fs->make<TH1F>(std::string(moduleLabel_).append("_").append("pfType1Met").c_str(),"pfType1Met", 100, 0, 300);
  h2_met_nvtx_MEtX=fs->make<TH2F>(std::string(moduleLabel_).append("_").append("metVsNvtx_MetX").c_str(),"met_nvtx_MEtX", 50, 0, 300,35,0,35 );
  h2_met_nvtx_MEtY=fs->make<TH2F>(std::string(moduleLabel_).append("_").append("metVsNvtx_MetX").c_str(),"met_nvtx_MEtX", 50, 0, 300,35,0,35 );
  profile_MET_x=fs->make<TProfile>(std::string(moduleLabel_).append("_").append("pfMet").append("_Px").c_str(),"pfMetVsPx", 100, 0, 300, -300,300);
  profile_MET_y=fs->make<TProfile>(std::string(moduleLabel_).append("_").append("pfMet").append("_Py").c_str(),"pfMetVsPy", 100, 0, 300, -300,300);
  profile_nvtx_x=fs->make<TProfile>(std::string(moduleLabel_).append("_").append("nvtx").append("_Px").c_str(),"nvtxVsPx", 50, 0, 50, -300,300);
  profile_nvtx_y=fs->make<TProfile>(std::string(moduleLabel_).append("_").append("nvtx").append("_Py").c_str(),"nvtxVsPy", 50, 0, 50, -300,300);

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
    char buffer[20];
    //for( int iMet(0);iMet<MetBin; iMet++)
    //{
    //  sprintf(buffer, "_metBin%d",iMet);
    //  profileMetBin_x_.push_back(fs->make<TProfile>(std::string(moduleLabel_).append("_").append(namePostFix(varType)).append("_").append(v->getParameter<std::string>("name")).append(buffer).append("_Px").c_str(),"Px", nbins, nMin, nMax, -300,300));
    //  profileMetBin_y_.push_back(fs->make<TProfile>(std::string(moduleLabel_).append("_").append(namePostFix(varType)).append("_").append(v->getParameter<std::string>("name")).append(buffer).append("_Py").c_str(),"Py", nbins, nMin, nMax, -300,300));
    //}

    profile_x_.push_back(fs->make<TProfile>(std::string(moduleLabel_).append("_").append(namePostFix(varType)).append("_").append(v->getParameter<std::string>("name")).append("_Px").c_str(),"Px", nbins, nMin, nMax, -300,300));
    profile_y_.push_back(fs->make<TProfile>(std::string(moduleLabel_).append("_").append(namePostFix(varType)).append("_").append(v->getParameter<std::string>("name")).append("_Py").c_str(),"Py", nbins, nMin, nMax, -300,300));

    profile_phi.push_back(fs->make<TProfile>(std::string(moduleLabel_).append("_").append(namePostFix(varType)).append("_").append(v->getParameter<std::string>("name")).c_str(),"phiVsPt", 30, -M_PI, M_PI, -300,300));

    profile_phi_valence.push_back(fs->make<TProfile>(std::string(moduleLabel_).append("_").append(namePostFix(varType)).append("_").append(v->getParameter<std::string>("name")).append("_valence").c_str(),"phiVsPt", 30, 0, M_PI, -300,300));

    h2_met_varType_x.push_back(fs->make<TH2F>(std::string("h2_met").append("_").append(namePostFix(varType)).append("_").append(v->getParameter<std::string>("name")).append("_MET_x").c_str(),std::string("h2_met_").append(namePostFix(varType)).append("_MET_x").c_str(),  100, 0, 300, nbins, nMin, nMax));
    h2_met_varType_y.push_back(fs->make<TH2F>(std::string("h2_met").append("_").append(namePostFix(varType)).append("_").append(v->getParameter<std::string>("name")).append("_MET_y").c_str(),std::string("h2_met_").append(namePostFix(varType)).append("_MET_y").c_str(),  100, 0, 300, nbins, nMin, nMax));


    occupancy_.push_back(fs->make<TH2F>(std::string(moduleLabel_).append("_").append(namePostFix(varType)).append("_").append(v->getParameter<std::string>("name")).append("_occupancy").c_str(),"occupancy",  etaNBins, etaMin, etaMax, phiNBins, phiMin, phiMax));
    energy_.push_back(fs->make<TH2F>(std::string(moduleLabel_).append("_").append(namePostFix(varType)).append("_").append(v->getParameter<std::string>("name")).append("_energy").c_str(),"energy",           etaNBins, etaMin, etaMax, phiNBins, phiMin, phiMax));
    pt_.push_back(fs->make<TH2F>(std::string(moduleLabel_).append("_").append(namePostFix(varType)).append("_").append(v->getParameter<std::string>("name")).append("_pt").c_str(),"pt",                       etaNBins, etaMin, etaMax, phiNBins, phiMin, phiMax));
    variable_.push_back(fs->make<TH1F>(std::string(moduleLabel_).append("_").append(namePostFix(varType)).append("_").append(v->getParameter<std::string>("name")).append("_variable").c_str(),"variable", nbins, nMin, nMax));
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

  h1_met->Fill(pfMet_pt);
  h2_met_nvtx_MEtX->Fill(pfMet_pt, ngoodVertices, pfMet_px);
  h2_met_nvtx_MEtY->Fill(pfMet_pt, ngoodVertices, pfMet_py);
  profile_MET_x->Fill(pfMet_pt, pfMet_px);
  profile_MET_y->Fill(pfMet_pt, pfMet_py);
  profile_nvtx_x->Fill(ngoodVertices, pfMet_px);
  profile_nvtx_y->Fill(ngoodVertices, pfMet_py);

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

      //std::cout<<"tv2_ptcMet phi:"<<tv2_ptcMet->Phi_mpi_pi(tv2_ptcMet->Phi())<<std::endl;
      ptclMetPhi=tv2_ptcMet->Phi_mpi_pi(tv2_ptcMet->Phi());
      ptclMetPt = tv2_ptcMet->Mod();
      //std::cout<<"c phi:"<<c.phi()<<std::endl;
      if(ptclMetPhi < 0){
        usedPhi = M_PI + ptclMetPhi;
        usedPt = -1*ptclMetPt;
      }else {
        usedPhi = ptclMetPhi;
        usedPt = ptclMetPt;
      }
      //std::cout<<"ptcl phi: "<<ptclPhi<<" usedPhi: "<<usedPhi<<" M_PI"<<M_PI<<std::endl;

      profile_phi[j]->Fill(ptclMetPhi, ptclMetPt);
      profile_phi_valence[j]->Fill(usedPhi, usedPt);
      profile_x_[j]->Fill(ptclMetPt, MEx_[j]);
      profile_y_[j]->Fill(ptclMetPt, MEy_[j]);


      variable_[j]->Fill(ptclMetPt);
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
      //      profileMetBin_x_[j*MetBin + k ]->Fill(ngoodVertices, MEx_[j]);
      //      profileMetBin_y_[j*MetBin + k ]->Fill(ngoodVertices, MEy_[j]);
      //    }
      //  }else{
      //    if( pfMet_pt > k*10 && pfMet_pt < (k+1)*10 )
      //    {
      //      profileMetBin_x_[j*MetBin + k ]->Fill(ngoodVertices, MEx_[j]);
      //      profileMetBin_y_[j*MetBin + k ]->Fill(ngoodVertices, MEy_[j]);
      //    }
      //  }
      //}
      profile_x_[j]->Fill(ngoodVertices, MEx_[j]);
      profile_y_[j]->Fill(ngoodVertices, MEy_[j]);
      variable_[j]->Fill(ngoodVertices);
      h2_met_varType_x[j]->Fill(pfMet_pt,ngoodVertices, MEx_[j]);
      h2_met_varType_y[j]->Fill(pfMet_pt,ngoodVertices, MEy_[j]);

#if(MkTree)
      nVtx_catagory.push_back(j);
      nVtx_nVtx.push_back(ngoodVertices);
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

