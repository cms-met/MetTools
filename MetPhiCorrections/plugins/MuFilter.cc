/**\class MuFilter MuFilter.cc at least isolated muon
*/
// system include files
#include <memory>
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TLorentzVector.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/MET.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

// class declaration
class MuFilter : public edm::EDFilter {
   public:
      explicit MuFilter(const edm::ParameterSet&);
      ~MuFilter();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      edm::InputTag vtxTag_;
      virtual void beginJob() override;
      virtual bool filter(edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      const bool passesVtxSel(size_t a=0) const;
      const reco::Vertex highestPtVtx()const;
      void setVertex(const edm::Handle<reco::VertexCollection> & vtxH);

      reco::VertexRefVector vtxs_;
      edm::EDGetTokenT<reco::VertexCollection> vtxHT_;

      edm::EDGetTokenT<pat::MuonCollection> MuonLabel_;
      edm::EDGetTokenT<pat::METCollection>  MetLabel_;

      // ----------member data ---------------------------
};
// static data member definitions
MuFilter::MuFilter(const edm::ParameterSet& iConfig) :
  MuonLabel_(consumes<pat::MuonCollection>(iConfig.getParameter<edm::InputTag>("MuonLabel"))),
  MetLabel_(consumes<pat::METCollection>(iConfig.getParameter<edm::InputTag>("MetLabel")))
  //vtxHT_(consumes<reco::VertexCollection>(iConfig.getParameter<edm::InputTag>("vtxTag" )))
{
   //now do what ever initialization is needed
   vtxTag_            = iConfig.getParameter<edm::InputTag>("vtxTag" );
   vtxHT_         = consumes<reco::VertexCollection>(vtxTag_);
}
MuFilter::~MuFilter()
{
   // do anything here that needs to be done at desctruction time
}

const bool MuFilter::passesVtxSel(size_t i) const {
  if (i >= vtxs_.size()) return false;
  else {
    return (vtxs_[i]->isValid() &&
	    !vtxs_[i]->isFake() &&
	    vtxs_[i]->ndof() >= 4.0 &&
	    fabs(vtxs_[i]->position().Rho()) < 2.0 &&
	    fabs(vtxs_[i]->z()) < 24.0);
  }
}

const reco::Vertex
MuFilter::highestPtVtx() const {
  if (vtxs_.size()   == 0) return reco::Vertex();
  for (size_t i=0;i<vtxs_.size();++i) {
    if (passesVtxSel(i)) {
      return *vtxs_[i];
    }
  }
  //---- if none are good ... return the default?!?
  return reco::Vertex();
}
void MuFilter::setVertex(const edm::Handle<reco::VertexCollection> & vtxH) {
  for(size_t i=0;i<vtxH->size();++i) vtxs_.push_back(reco::VertexRef(vtxH,i));
}
// ------------ method called on each new Event  ------------
bool
MuFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;

  Handle<pat::MuonCollection> Muons;
  iEvent.getByToken(MuonLabel_, Muons);

  Handle<pat::METCollection> MET;
  iEvent.getByToken(MetLabel_, MET);

  //edm::Handle<reco::VertexCollection> vtxH;
  //iEvent.getByToken(vtxHT_,vtxH);

  //setVertex(vtxH);
  //const reco::Vertex primaryVtx = highestPtVtx();

  bool isMuEvt=false;
  //for (const pat::Muon & aPatMuon : *Muons) 
  int n=Muons->size();
/////////////////////////////////////////////
  //double Met=MET->size();
    
  //auto& pMET = MET->at(j);
  //if( pMET.pt() >10  )
  //
  if( n > 0 )
  for (int i=0; i<n; ++i)
  {
    auto& p1 = Muons->at(i);
    if(!p1.isPFMuon()) continue;
    if(!(p1.isGlobalMuon() || p1.isTrackerMuon()) ) continue;
    if(!(p1.pt()>10 && fabs(p1.eta())<2.4) ) continue;

    double pt    = p1.pt() ;

    double chIso = p1.pfIsolationR04().sumChargedHadronPt;
    double nhIso = p1.pfIsolationR04().sumNeutralHadronEt;
    double phIso = p1.pfIsolationR04().sumPhotonEt;
    double puIso = p1.pfIsolationR04().sumPUPt;
    //for deltabeta correction
    float relIso1 = ( chIso + std::max( 0.0, nhIso + phIso - 0.5 * puIso) ) / pt;
    if(relIso1>0.15) continue;
    //if(p1.muonBestTrack()->dxy(primaryVtx.position())> 0.02) continue;
    //if(p1.muonBestTrack()->dz(primaryVtx.position())>0.1) continue;
    isMuEvt=true;
  }
  return isMuEvt;
}
// ------------ method called once each job just before starting event loop  ------------
void 
MuFilter::beginJob()
{
}
// ------------ method called once each job just after ending the event loop  ------------
void 
MuFilter::endJob() {
}
// ------------ method called when starting to processes a run  ------------
/*
void
MuFilter::beginRun(edm::Run const&, edm::EventSetup const&)
{ 
}
*/
// ------------ method called when ending the processing of a run  ------------
/*
void
MuFilter::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
// ------------ method called when starting to processes a luminosity block  ------------
/*
void
MuFilter::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
MuFilter::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
MuFilter::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
//define this as a plug-in
DEFINE_FWK_MODULE(MuFilter);
