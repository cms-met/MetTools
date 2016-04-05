
#include "../interface/MAPAnalyzer.h"
#include "FWCore/Framework/interface/MakerMacros.h"

MAPAnalyzer::MAPAnalyzer(const edm::ParameterSet& iConfig):
  JME::Analyzer(iConfig){
        edm::InputTag variableNames_;
        edm::InputTag variables_;
  if (iConfig.existsAs<edm::InputTag>("srcVariableNames"))
    srcVariableNames_ = iConfig.getParameter<edm::InputTag>("srcVariableNames");
  else throw cms::Exception("Configuration")<<"[MAPAnalyzer] srcVariableNames not given \n";

  if (iConfig.existsAs<edm::InputTag>("srcVariables"))
    srcVariables_ = iConfig.getParameter<edm::InputTag>("srcVariables");
  else throw cms::Exception("Configuration")<<"[MAPAnalyzer] srcVariables for map not given \n";

  srcGenEvent_ = iConfig.getParameter<edm::InputTag>("srcGenEvent");

  if (iConfig.existsAs<std::vector<std::string>>("variableNamesToSave"))
    variableNamesToSave_ = iConfig.getParameter<std::vector<std::string>>("variableNamesToSave");
  else throw cms::Exception("Configuration")<<"[MAPAnalyzer] variableNamesToSave not given \n";

  for(auto varName: variableNamesToSave_)
  {
    var_.insert(std::pair<std::string, std::reference_wrapper<float>>(varName, tree[varName.c_str()].write<float>()));
  }
  //manually insert event weight for nlo samples
  var_.insert(std::pair<std::string, std::reference_wrapper<float>>("weight", tree["weight"].write<float>()));

  srcVariableNamesToken_ = consumes<std::vector<std::string>>(srcVariableNames_);
  srcVariablesToken_ = consumes<std::vector<Float_t>>(srcVariables_);
  srcGenEventToken_ = consumes<GenEventInfoProduct>(srcGenEvent_);
}

MAPAnalyzer::~MAPAnalyzer(){}

void MAPAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  edm::Handle<std::vector<std::string>> VariableNamesHandle;
  iEvent.getByToken(srcVariableNamesToken_, VariableNamesHandle);
  edm::Handle<std::vector<Float_t>> VariablesHandle;
  iEvent.getByToken(srcVariablesToken_, VariablesHandle);

  std::vector<Float_t>::const_iterator variable = VariablesHandle->begin();
  std::vector<std::string>::const_iterator name = VariableNamesHandle->begin(); 

  while(variable != VariablesHandle->end())
  {
    if(var_.count((*name)) > 0)
      var_.at((*name)).get() = (*variable);
    name++;
    variable++;
  }
  edm::Handle<GenEventInfoProduct> genEvt;
  iEvent.getByToken(srcGenEventToken_, genEvt);
  var_.at("weight").get() = genEvt->weight()/abs(genEvt->weight()); 
  tree.fill();
}
DEFINE_FWK_MODULE(MAPAnalyzer);

