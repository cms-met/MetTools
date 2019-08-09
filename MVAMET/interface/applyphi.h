#ifndef APPLYTRAINING
#define APPLYTRAINING

#include "TMath.h"
#include "TLorentzVector.h"
#include "TFile.h"
#include "TTree.h"
#include "TTreeFormula.h"
#include "GBRTrainer.h"
#include "CondFormats/EgammaObjects/interface/GBRForest.h"
//#include "Cintex/Cintex.h"
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <cassert>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>



class applyTraining {
  public:
    applyTraining(std::string name, std::string apply_MVA_to, std::string weightfilename, int mode, TTree *inputTree, std::string input_filename);
    applyTraining(boost::property_tree::ptree &pt, TTree *inputTree);
    applyTraining(boost::property_tree::ptree &pt, TTree *inputTree, std::string &friendFilename, std::string &friendTreename);
    applyTraining(std::string name, std::string apply_MVA_to, std::string weightfilename, TTree *inputTree, std::string input_filename);
    void registerUpdatedFourVector();
    void registerUpdatedMET();
    void registerUpdatedCovMatrix();
    void calculateUpdatedFourVector();
    void calculateUpdatedMET();
    void calculateUpdatedMETCovMatrix();
    void getResults();
    virtual ~applyTraining();

  private:
  int _mode = 0;
  void wireInputs();
  void eventLoop();
  TLorentzVector _z;
  float _z_pT                = 0;
  float _z_Phi               = 0;
  float _mvaCov1, _mvaCov2;

  TLorentzVector _oldU;
  float _old_U;
  float _old_UPhi;

  float _mvaResponse         = 0;

  TLorentzVector _newU;
  float _new_U;
  float _new_LongZ;
  float _new_PerpZ;
  float _new_UPhi;

  TLorentzVector _MET;
  float _new_met;
  float _new_metphi;

  float _Cov11, _Cov12, _Cov21, _Cov22;

  std::string _iTrain;
  std::string _iName;
  std::string _applyMVAto;
  std::string _mvaResponseName;
  TFile *_lFForest;
  const GBRForest * _lForest;
  const std::vector<std::string> *_lVars;
  int _lN = 0;
  TTreeFormula **_lFVars;
  Float_t *_lVals;

  TFile *_lInput;
  TTree *_lTree;

  int _lNEvents = 0;

  std::string _outputFilename;
  TFile *_lOFile;
  TTree *_lOTree;
};



//int main(int argc, char* argv[] );
#endif
