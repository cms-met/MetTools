#include "../interface/applyphi.h"
applyTraining::applyTraining(std::string name, std::string apply_MVA_to, std::string weightfilename, TTree *inputTree, std::string input_filename) :
  _mode(0),
  _applyMVAto(apply_MVA_to),
  _mvaResponseName(name),
  // Forest
  //inputs
  _lTree(inputTree),
  _lNEvents(_lTree->GetEntries()),
  //outputs
  _outputFilename(_mvaResponseName + ".root"),
  _lOFile(new TFile( _outputFilename.c_str(),"RECREATE"))
{
  std::cout << "new constructur" << std::endl;
  _lOTree = _lTree->CloneTree(0);
}
applyTraining::applyTraining(std::string name, std::string apply_MVA_to, std::string weightfilename, int mode, TTree *inputTree, std::string input_filename) :
  _mode(mode),
  _iTrain(weightfilename),
  _applyMVAto(apply_MVA_to),
  _mvaResponseName(name),
  // Forest
  _lFForest(new TFile(_iTrain.c_str())),
  _lForest( (GBRForest*) _lFForest->Get(_mvaResponseName.c_str()) ),
  _lVars( (std::vector<std::string>*) _lFForest->Get((_mvaResponseName + "varlist").c_str()) ),
  _lN(_lVars->size()),
  _lFVars(new TTreeFormula*[_lN]),
  _lVals(new Float_t[_lN]),
  //inputs
  _lTree(inputTree),
  _lNEvents(_lTree->GetEntries()),
  //outputs
  _outputFilename(_mvaResponseName + ".root"),
  _lOFile(new TFile( _outputFilename.c_str(),"RECREATE")),
  _lOTree(_lTree)
{
  std::cout << "new constructur" << std::endl;
  _lOTree = _lTree->CloneTree(0);
}
/*
applyTraining::applyTraining(boost::property_tree::ptree &pt, TTree *inputTree) :
  _mode(pt.get<int>("mode")),
  _iTrain((_mode>0) ? pt.get<std::string>("weightfilename") : "" ),
  _applyMVAto(pt.get<std::string>("apply_MVA_to")),
  _mvaResponseName(pt.get<std::string>("name")),
  // Forest
  _lFForest((_mode>0) ? new TFile(_iTrain.c_str()) : NULL),
  _lForest((_mode>0)  ? (GBRForest*)_lFForest->Get(_mvaResponseName.c_str()) : NULL),
  _lVars((_mode>0) ? (std::vector<std::string>*)_lFForest->Get((_mvaResponseName + "varlist").c_str()): NULL ),
  _lN((_mode>0) ? _lVars->size() : 0),
  _lFVars(new TTreeFormula*[_lN]),
  _lVals(new Float_t[_lN]),
  //inputs
  _lTree(inputTree),
  _lNEvents(_lTree->GetEntries()),
  //outputs
  _outputFilename(_mvaResponseName + ".root"),
  _lOFile(new TFile( _outputFilename.c_str(),"RECREATE")),
  _lOTree(new TTree(_mvaResponseName.c_str(), _outputFilename.c_str()))
{
  BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt.get_child("friends"))
  {
    assert(v.first.empty());
    std::string friendName = v.second.data();
    inputTree->AddFriend(friendName.c_str(), (friendName + ".root").c_str());
  }

}

*/

applyTraining::applyTraining(boost::property_tree::ptree &pt, TTree *inputTree, std::string &friendFilename, std::string &friendTreename):
  _mode(pt.get<int>("mode")),
  _iTrain((_mode>0) ? pt.get<std::string>("weightfilename") : "" ),
  _applyMVAto(pt.get<std::string>("apply_MVA_to")),
  _mvaResponseName(pt.get<std::string>("name")),
  // Forest
  _lFForest((_mode>0) ? new TFile(_iTrain.c_str()) : NULL),
  _lForest((_mode>0)  ? (GBRForest*)_lFForest->Get(_mvaResponseName.c_str()) : NULL),
  _lVars((_mode>0) ? (std::vector<std::string>*)_lFForest->Get((_mvaResponseName +"varlist").c_str()): NULL ),
  _lN((_mode>0) ? _lVars->size() : 0),
  _lFVars(new TTreeFormula*[_lN]),
  _lVals(new Float_t[_lN]),
  //inputs
  _lTree(inputTree),
  _lNEvents(_lTree->GetEntries()),
  //outputs
  _outputFilename(_mvaResponseName + ".root"),
  _lOFile(new TFile( _outputFilename.c_str(),"RECREATE")),
  _lOTree(new TTree(_mvaResponseName.c_str(), _outputFilename.c_str()))
{
  friendFilename = _outputFilename;
  friendTreename = _mvaResponseName;
}


void applyTraining::getResults()
{
  wireInputs();
  if(_mode != -1 && _mode!= 3) registerUpdatedFourVector();
  if(_mode != -1 && _mode!= 3) registerUpdatedMET();
  if(_mode == -1)  registerUpdatedCovMatrix();
  eventLoop();
}

void applyTraining::wireInputs()
{
  for(int i0 = 0; i0 < _lN; i0++) { 
    TString pExpress = _lVars->at(i0); 
    _lFVars[i0] = new TTreeFormula(pExpress,pExpress,_lTree);
  }

  for (int ivar = 0; ivar<_lN; ++ivar) {
    printf("%i: %s\n",ivar, _lVars->at(ivar).c_str());
  }
  _lTree->SetBranchAddress( (_applyMVAto + "_Pt").c_str()      ,&_old_U);
  _lTree->SetBranchAddress( (_applyMVAto + "_Phi").c_str()   ,&_old_UPhi);

  _lTree->SetBranchAddress( "Boson_Pt"                ,&_z_pT);
  _lTree->SetBranchAddress( "Boson_Phi"               ,&_z_Phi);

  if(_mode==-1)
  { 
    _lTree->SetBranchAddress( "CovU1"           ,&_mvaCov1);
    _lTree->SetBranchAddress( "CovU2"           ,&_mvaCov2);
  }
  _lOTree->Branch(_mvaResponseName.c_str(), &_mvaResponse, (_mvaResponseName + "/F").c_str());
}


void applyTraining::eventLoop()
{

  for (Long64_t i0=0; i0<_lNEvents;i0++)
    {
      if (i0 % 200000 == 0) std::cout << "--- ... Processing event: " << double(i0)/double(_lNEvents) << std::endl;
      _lTree->GetEntry(i0);
    if(_mode!=0)
    {
      for(int i1 = 0; i1 < _lN; i1++)
      { 
        _lVals[i1] = _lFVars[i1]->EvalInstance();
      }
    }
    _mvaResponse = (_mode>0) ? float(_lForest->GetResponse(_lVals)) : 0;
  //do additional calculations
    _z.SetPtEtaPhiM(_z_pT, 0, _z_Phi,0);
    if(_mode >  -1) calculateUpdatedFourVector();
    if(_mode >  -1) calculateUpdatedMET();
    if(_mode == -1) calculateUpdatedMETCovMatrix();
    _lOTree->Fill();
  }
}

void applyTraining::registerUpdatedFourVector()
{
    _lOTree->Branch((_mvaResponseName+"_Pt").c_str(),      &_new_U,     (_mvaResponseName+"_Pt/F").c_str());
    _lOTree->Branch((_mvaResponseName+"_LongZ").c_str(),   &_new_LongZ,  (_mvaResponseName+"_LongZ/F").c_str());
    _lOTree->Branch((_mvaResponseName+"_PerpZ").c_str(),  &_new_PerpZ, (_mvaResponseName+"_PerpZ/F").c_str());
    _lOTree->Branch((_mvaResponseName+"_Phi").c_str(),   &_new_UPhi,  (_mvaResponseName+"_Phi/F").c_str());
}

void applyTraining::registerUpdatedCovMatrix()
{
    _lOTree->Branch((_mvaResponseName+"_Cov11").c_str(),      &_Cov11,     (_mvaResponseName+"_U/F").c_str());
    _lOTree->Branch((_mvaResponseName+"_Cov12").c_str(),      &_Cov12,     (_mvaResponseName+"_U/F").c_str());
    _lOTree->Branch((_mvaResponseName+"_Cov21").c_str(),      &_Cov21,     (_mvaResponseName+"_U/F").c_str());
    _lOTree->Branch((_mvaResponseName+"_Cov22").c_str(),      &_Cov22,     (_mvaResponseName+"_U/F").c_str());
}

void applyTraining::calculateUpdatedFourVector()
{
    if(_mode==0)
    {
      _newU.SetPtEtaPhiM(_old_U, 0, _old_UPhi,0);
    } 
    else if(_mode==1)//phi Corrections
    {
      _newU.SetPtEtaPhiM(_old_U, 0, _old_UPhi + _mvaResponse,0);
    }
    else if(_mode==2)// U Corrections
    {
      _newU.SetPtEtaPhiM(fabs(_mvaResponse * _old_U), 0, _old_UPhi,0);
      if(_mvaResponse < 0)
        _newU.RotateZ(TMath::Pi());
    }

    _new_U = float(_newU.Pt());
    _new_UPhi = _newU.Phi();

    TLorentzVector parU = _newU;
    parU.RotateZ(-_z.Phi());
    _new_LongZ = float(parU.Px());
    _new_PerpZ = float(parU.Py());
}

void applyTraining::registerUpdatedMET()
{
    _lOTree->Branch((_mvaResponseName+"_MET").c_str(), &_new_met, (_mvaResponseName+"_MET/F").c_str());
    _lOTree->Branch((_mvaResponseName+"_METPhi").c_str(), &_new_metphi, (_mvaResponseName+"_METPhi/F").c_str());
}

void applyTraining::calculateUpdatedMET()
{
      _MET = _newU - _z;
      _new_met = float(_MET.Pt());
      _new_metphi = float(_MET.Phi());
}

void applyTraining::calculateUpdatedMETCovMatrix()
{
    double cosPhi = cos(_old_UPhi);
    double sinPhi = sin(_old_UPhi);
    _Cov11 =  _old_U*( _mvaCov1*cosPhi*cosPhi + _mvaCov2*sinPhi*sinPhi);
    _Cov12 =  _old_U*(-_mvaCov1*sinPhi*cosPhi + _mvaCov2*sinPhi*cosPhi);
    _Cov21 =  _Cov12;
    _Cov22 =  _old_U*( _mvaCov1*sinPhi*sinPhi + _mvaCov2*cosPhi*cosPhi);
}

applyTraining::~applyTraining() {
  TObjArray *listOfBranches = _lTree->GetListOfBranches();
  for(int i=0; i < listOfBranches->GetEntries(); ++i)
  {
    _lTree->ResetBranchAddress((TBranch*)listOfBranches->At(i));
  }
  _lOTree->Write();
  _lOFile->Close();
}
  
  

