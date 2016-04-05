#include "TFile.h"
#include "TTree.h"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <iostream>
#include <fstream>
class distributionFlatter
{
public:
  distributionFlatter(boost::property_tree::ptree &pt1, TTree* inputTree1):
    pt(pt1),
    inputTree(inputTree1),
    weight(pt.get<std::string>("weight")),
    variable(pt.get<std::string>("flattenVariable")),
    type(pt.get<int>("type")),
    sum(0),
    outputFilename(pt.get<std::string>("name") + ".root"),
    outputFile(new TFile(outputFilename.c_str(), "RECREATE")),
    outputTree(new TTree(pt.get<std::string>("name").c_str(), pt.get<std::string>("desc").c_str()))
  {
  std::cout << "init flatter on variable " << variable <<std::endl;
    weightExpr = new TTreeFormula(weight.c_str(), weight.c_str(), inputTree);
    variableExpr = new TTreeFormula(variable.c_str(), variable.c_str(), inputTree);
  }

  ~distributionFlatter()
  {
    delete outputFile;
    delete outputTree;
    delete weightExpr;
    delete variableExpr;
  }

  void writeWeightToTree(std::string &friendFilename, std::string &friendTreename)
  {
    float weight;
    outputTree->Branch(pt.get<std::string>("name").c_str(), &weight, (pt.get<std::string>("name") + "/F").c_str());

    for(Long64_t i=0; i<inputTree->GetEntries(); i++)
    {
      inputTree->GetEntry(i);
      float variableValue = variableExpr->EvalInstance();
      weight = getWeight(variableValue);
      outputTree->Fill();
    }
    friendFilename = outputFilename; 
    friendTreename = pt.get<std::string>("name");
    outputTree->Write();
    outputFile->Close();
  }


  float getWeight(float value)
  {
    if(type == 1)
    {
      for(size_t i = 0; i<binBoundaries.size(); ++i)
      {
        if ((value >= binBoundaries[i]) &&(value < binBoundaries[i+1]))
        {
          return (binBoundaries[i+1]-binBoundaries[i]);
        }
      }
    }
    if(type == 2)
    {
      int index = (int) value;
      return binWeights[index];
    }

    return 0;
  }

  void calculateWeights()
  {
    std::vector<float> variableList;
    for(Long64_t i=0; i<inputTree->GetEntries(); i++)
    {
      inputTree->GetEntry(i);
      float weight = weightExpr->EvalInstance();
      float variableValue = variableExpr->EvalInstance();
      if (weight > 0)
      {
        sum+= variableValue;
        variableList.push_back(variableValue);
      }
    } 
    std::cout << "Sorting " << variableList.size()<<  " entries from variable "<< variable << " to flatten its distribution for the training. Sum = " << sum << std::endl;
    std::sort(variableList.begin(), variableList.end());

    //different treatment of continous and discrete variables
    if(type==1)
    {
      for(size_t i=0; i<variableList.size(); i+=pt.get<int>("eventsPerBin"))
      {
        binBoundaries.push_back(variableList[i]);
        std::cout << i << ":" << variableList[i] << std::endl;
      }
    }
    if(type==2)
    {
    float nNumbers = 0.; // number of elements
    float value = 0;
      for(auto item: variableList)
      {
        if (value == item)
          nNumbers += 1.;
        else
        {
          int index = (int) value;
          binWeights[index] = nNumbers>0 ? sum/nNumbers : 0;
          std::cout << "Bin " << index << " has " << nNumbers << " entries, weight = " << binWeights[index] << std::endl;
          nNumbers = 0;
          value = item;
        }
      }
    }
  }


private:
  boost::property_tree::ptree pt;
  TTree *inputTree; 
  std::string weight;
  std::string variable;
  TTreeFormula *weightExpr;
  TTreeFormula *variableExpr;
  std::vector<float> binBoundaries;
  std::map<int, float> binWeights;
  //ifstream boundariesFile;
  int type;
  float sum;
  std::string outputFilename;
  TFile *outputFile;
  TTree *outputTree;
};
