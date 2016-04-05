#include "TTree.h"
#include "../interface/GBRTrainer.h"
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

#include "../interface/applyphi.h"

using namespace std;

int main(int argc, char* argv[] ) {

  std::string config = argv[1];
  std::cout << argv[1] << std::endl;
  if(strstr(argv[1], "manual") )
  {
    //mauell
    std::string filename1 = argv[2];
    TFile *inputFile1 = TFile::Open(filename1.c_str());
    TTree *inputTree1 = (TTree*)(inputFile1->Get("MAPAnalyzer/t"));
    
    applyTraining *phi = new applyTraining("PhiCorrectedRecoil", "recoilPFMet", "PhiCorection_PUPPI.root", 1, inputTree1, filename1);
    phi->getResults();
    delete phi;

    std::string filename2 = filename1 + "_PhiCorrectedRecoil.root";
    TFile *inputFile2 = TFile::Open(filename2.c_str());
    TTree *inputTree2 = (TTree*)(inputFile2->Get("t"));
    std::cout << filename2 << std::endl;
    applyTraining *recoil = new applyTraining("LongZCorrectedRecoil", "PhiCorrectedRecoil", "RecoilCorrection_PUPPI.root", 2, inputTree2, filename2);
    recoil->getResults();
    delete recoil;
    return 0;
  }


	// config file einlesen
  boost::property_tree::ptree pt;
  try 
  {
    std::ifstream in(argv[1]);
    std::stringstream ss;
    ss << in.rdbuf();      
    boost::property_tree::json_parser::read_json(ss, pt);
  } catch (std::exception const& e)
  {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  std::vector<boost::property_tree::ptree> trainingProperties;
  BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt.get_child("activeTrainings"))
  {
    assert(v.first.empty());
    trainingProperties.push_back( pt.get_child(v.second.data()));
  }

  std::string inputFilename = pt.get<std::string>("inputFile");
  std::string weightfilename = pt.get<std::string>("weightfilename");

  TFile *inputFile = TFile::Open(inputFilename.c_str());
  std::string foldername = pt.get<std::string>("Folder");
  TTree *inputTree = (TTree*)(inputFile->Get(foldername.c_str()));

  std::cout << "input tree: " << inputTree << std::endl;
  std::cout << "This many: " << trainingProperties.size() << std::endl;
  std::string lastName;
  for(size_t iTrain = 0; iTrain < trainingProperties.size(); ++iTrain)
  {
    //applyTraining *user = new applyTraining(trainingProperties[iTrain], inputTree);
    BOOST_FOREACH(boost::property_tree::ptree::value_type &v, trainingProperties[iTrain].get_child("friends"))
    {
      assert(v.first.empty());
      std::string friendName = v.second.data();
      inputTree->AddFriend("t", (friendName + ".root").c_str());
    }
    if(trainingProperties[iTrain].get<int>("mode") == 0)
    {
    if(iTrain>0)
    {
      inputFile = TFile::Open((lastName+".root").c_str());
      inputTree = (TTree*)(inputFile->Get("t"));
    }
    lastName =  trainingProperties[iTrain].get<std::string>("name");
    applyTraining *user = new applyTraining(trainingProperties[iTrain].get<std::string>("name"), 
                                            trainingProperties[iTrain].get<std::string>("apply_MVA_to"),
                                            weightfilename,
       //                                     trainingProperties[iTrain].get<int>("mode"),
                                            inputTree,
                                            inputFilename);
    user->getResults();
    delete user;
    }else 
    {
    applyTraining *user = new applyTraining(trainingProperties[iTrain].get<std::string>("name"), 
                                            trainingProperties[iTrain].get<std::string>("apply_MVA_to"),
                                            weightfilename,
                                            trainingProperties[iTrain].get<int>("mode"),
                                            inputTree,
                                            inputFilename);
    user->getResults();
    delete user;
    }
    std::cout << "Initialized." << std::endl;
  }
}

