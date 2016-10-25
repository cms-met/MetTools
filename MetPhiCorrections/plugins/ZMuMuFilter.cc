/**\class ZMuMuFilter ZMuMuFilter.cc ZtoMuMu
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
// class declaration
class ZMuMuFilter : public edm::EDFilter {
   public:
      explicit ZMuMuFilter(const edm::ParameterSet&);
      ~ZMuMuFilter();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() override;
      virtual bool filter(edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

    edm::EDGetTokenT<pat::MuonCollection> MuonLabel_;
    edm::EDGetTokenT<pat::METCollection>  MetLabel_;
      // ----------member data ---------------------------
};
// static data member definitions
ZMuMuFilter::ZMuMuFilter(const edm::ParameterSet& iConfig) :
  MuonLabel_(consumes<pat::MuonCollection>(iConfig.getParameter<edm::InputTag>("MuonLabel"))),
  MetLabel_(consumes<pat::METCollection>(iConfig.getParameter<edm::InputTag>("MetLabel")))
{
   //now do what ever initialization is needed
}
ZMuMuFilter::~ZMuMuFilter()
{
   // do anything here that needs to be done at desctruction time
}
// ------------ method called on each new Event  ------------
bool
ZMuMuFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  Handle<pat::MuonCollection> Muons;
  iEvent.getByToken(MuonLabel_, Muons);
  int j = 0;
  double zmass=0;
  bool isZ=false;
  //for (const pat::Muon & aPatMuon : *Muons) 
  int n=Muons->size();
/////////////////////////////////////////////
  using namespace edm;
  Handle<pat::METCollection> MET;
  iEvent.getByToken(MetLabel_, MET);
  //double Met=MET->size();
    
  auto& pMET = MET->at(j);

  if( pMET.pt() >10  )
  if( n > 1 )
  for (int i=0; i<n-1; ++i)
  {
    auto& p1 = Muons->at(i);
    if(!p1.isPFMuon()) continue;
    if(!(p1.isGlobalMuon() || p1.isTrackerMuon()) ) continue;
    if(!(p1.pt()>20 && fabs(p1.eta())<2.4) ) continue;

    double pt    = p1.pt() ;

    double chIso = p1.pfIsolationR04().sumChargedHadronPt;
    double nhIso = p1.pfIsolationR04().sumNeutralHadronEt;
    double phIso = p1.pfIsolationR04().sumPhotonEt;
    double puIso = p1.pfIsolationR04().sumPUPt;
    //for deltabeta correction
    float relIso1 = ( chIso + std::max( 0.0, nhIso + phIso - 0.5 * puIso) ) / pt;
    if(relIso1>0.20) continue;
    //if(relIso1>0.12) continue;
    TLorentzVector a;
    a.SetPtEtaPhiM(p1.pt(),p1.eta(),p1.phi(),p1.mass());
    //for (const pat::Muon & aPatMuon2 : *Muons) 
    for (int j=i; j<n; ++j) 
    {
       auto& p2 = Muons->at(j);
       if(!p2.isPFMuon()) continue;
       if(!(p2.isGlobalMuon() || p2.isTrackerMuon()) ) continue;
       if(!(p2.pt()>20 && fabs(p2.eta())<2.4) ) continue;
       
       double pt2    = p1.pt() ;
       
       double chIso2 = p2.pfIsolationR04().sumChargedHadronPt;
       double nhIso2 = p2.pfIsolationR04().sumNeutralHadronEt;
       double phIso2 = p2.pfIsolationR04().sumPhotonEt;
       double puIso2 = p2.pfIsolationR04().sumPUPt;
       //for deltabeta correction
       float relIso2 = ( chIso2 + std::max( 0.0, nhIso2 + phIso2 - 0.5 * puIso2) ) / pt2;
       //if(relIso2>0.12) continue;
       if(relIso2>0.20) continue;

       TLorentzVector b;
       b.SetPtEtaPhiM(p2.pt(),p2.eta(),p2.phi(),p2.mass());
       zmass = (a+b).M();
       if(fabs(zmass-91.2)<15 && p1.charge()*p2.charge()<0 ) isZ=true;
    } 
  }
  return isZ;
}
// ------------ method called once each job just before starting event loop  ------------
void 
ZMuMuFilter::beginJob()
{
}
// ------------ method called once each job just after ending the event loop  ------------
void 
ZMuMuFilter::endJob() {
}
// ------------ method called when starting to processes a run  ------------
/*
void
ZMuMuFilter::beginRun(edm::Run const&, edm::EventSetup const&)
{ 
}
*/
// ------------ method called when ending the processing of a run  ------------
/*
void
ZMuMuFilter::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
// ------------ method called when starting to processes a luminosity block  ------------
/*
void
ZMuMuFilter::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
ZMuMuFilter::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ZMuMuFilter::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
//define this as a plug-in
DEFINE_FWK_MODULE(ZMuMuFilter);
