#include <TH1.h>
#include <TH2.h>
#include <TString.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TFile.h>
#include <TStyle.h>
#include <TPaveStats.h>
#include <TLatex.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <TF1.h>

//Combines the corrections (as a function of nvtx) for all particle types, to have the final plots of MET corrections (in X and Y) to be applied on a given era.
//Also fits a linear function to these plots. These functions are stored in a txt file.

double fitFunction(double *x, double *par)
{
  double result = -1.;
  result = par[0]+par[1]*x[0];
  return result;
}

int combine_corrections(int year=2016){

  //Modify file names and paths if needed.
  ofstream outfile;
  outfile.open("fits_"+std::to_string(year)+".txt");
  std::vector<TFile*> files;
  if(year==2018){
    files.push_back(TFile::Open("../fullProd/histo_merged_DataDoubleMu2018A.root"));
    files.push_back(TFile::Open("../fullProd/histo_merged_DataDoubleMu2018B.root"));
    files.push_back(TFile::Open("../fullProd/histo_merged_DataDoubleMu2018C.root"));
    files.push_back(TFile::Open("../fullProd/histo_merged_DataDoubleMu2018D.root"));
    files.push_back(TFile::Open("../fullProd/histo_merged_MC2018.root"));
  }
  else if(year==2017){
    files.push_back(TFile::Open("../fullProd/histo_merged_DataDoubleMu2017B.root"));
    files.push_back(TFile::Open("../fullProd/histo_merged_DataDoubleMu2017C.root"));
    files.push_back(TFile::Open("../fullProd/histo_merged_DataDoubleMu2017D.root"));
    files.push_back(TFile::Open("../fullProd/histo_merged_DataDoubleMu2017E.root"));
    files.push_back(TFile::Open("../fullProd/histo_merged_DataDoubleMu2017F.root"));
    files.push_back(TFile::Open("../fullProd/histo_merged_MC2017.root"));
  }
  else if(year==2016){
    files.push_back(TFile::Open("../fullProd/histo_merged_DataDoubleMu2016B.root"));
    files.push_back(TFile::Open("../fullProd/histo_merged_DataDoubleMu2016C.root"));
    files.push_back(TFile::Open("../fullProd/histo_merged_DataDoubleMu2016D.root"));
    files.push_back(TFile::Open("../fullProd/histo_merged_DataDoubleMu2016E.root"));
    files.push_back(TFile::Open("../fullProd/histo_merged_DataDoubleMu2016F.root"));
    files.push_back(TFile::Open("../fullProd/histo_merged_DataDoubleMu2016G.root"));
    files.push_back(TFile::Open("../fullProd/histo_merged_DataDoubleMu2016H.root"));
    files.push_back(TFile::Open("../fullProd/histo_merged_MC2016.root"));
  }

  std::vector<TFile*> outFiles;
  if(year==2018){
    outFiles.push_back(new TFile("plots_2018/outFile_combined_DataDoubleMu2018A.root", "RECREATE"));
    outFiles.push_back(new TFile("plots_2018/outFile_combined_DataDoubleMu2018B.root", "RECREATE"));
    outFiles.push_back(new TFile("plots_2018/outFile_combined_DataDoubleMu2018C.root", "RECREATE"));
    outFiles.push_back(new TFile("plots_2018/outFile_combined_DataDoubleMu2018D.root", "RECREATE"));
    outFiles.push_back(new TFile("plots_2018/outFile_combined_MC2018.root", "RECREATE"));
  }
  else if(year==2017){
    outFiles.push_back(new TFile("plots_2017/outFile_combined_DataDoubleMu2017B.root", "RECREATE"));
    outFiles.push_back(new TFile("plots_2017/outFile_combined_DataDoubleMu2017C.root", "RECREATE"));
    outFiles.push_back(new TFile("plots_2017/outFile_combined_DataDoubleMu2017D.root", "RECREATE"));
    outFiles.push_back(new TFile("plots_2017/outFile_combined_DataDoubleMu2017E.root", "RECREATE"));
    outFiles.push_back(new TFile("plots_2017/outFile_combined_DataDoubleMu2017F.root", "RECREATE"));
    outFiles.push_back(new TFile("plots_2017/outFile_combined_MC2017.root", "RECREATE"));
  }
  else if(year==2016){
    outFiles.push_back(new TFile("plots_2016/outFile_combined_DataDoubleMu2016B.root", "RECREATE"));
    outFiles.push_back(new TFile("plots_2016/outFile_combined_DataDoubleMu2016C.root", "RECREATE"));
    outFiles.push_back(new TFile("plots_2016/outFile_combined_DataDoubleMu2016D.root", "RECREATE"));
    outFiles.push_back(new TFile("plots_2016/outFile_combined_DataDoubleMu2016E.root", "RECREATE"));
    outFiles.push_back(new TFile("plots_2016/outFile_combined_DataDoubleMu2016F.root", "RECREATE"));
    outFiles.push_back(new TFile("plots_2016/outFile_combined_DataDoubleMu2016G.root", "RECREATE"));
    outFiles.push_back(new TFile("plots_2016/outFile_combined_DataDoubleMu2016H.root", "RECREATE"));
    outFiles.push_back(new TFile("plots_2016/outFile_combined_MC2016.root", "RECREATE"));
  }


  for(unsigned int iFiles = 0 ; iFiles < files.size() ; iFiles++){
    files[iFiles]->cd();

    std::vector<TH1D*> histos_x;
    std::vector<TH1D*> histos_y;

    histos_x.push_back((TH1D*)gDirectory->Get("metPhiCorrInfoWriter/metPhiCorrInfoWriter_ngoodVertices_hEtaPlus_Px"));
    histos_x.push_back((TH1D*)gDirectory->Get("metPhiCorrInfoWriter/metPhiCorrInfoWriter_ngoodVertices_hEtaMinus_Px"));
    histos_x.push_back((TH1D*)gDirectory->Get("metPhiCorrInfoWriter/metPhiCorrInfoWriter_ngoodVertices_hHFPlus_Px"));
    histos_x.push_back((TH1D*)gDirectory->Get("metPhiCorrInfoWriter/metPhiCorrInfoWriter_ngoodVertices_hHFMinus_Px"));
    histos_x.push_back((TH1D*)gDirectory->Get("metPhiCorrInfoWriter/metPhiCorrInfoWriter_ngoodVertices_h0Barrel_Px"));
    histos_x.push_back((TH1D*)gDirectory->Get("metPhiCorrInfoWriter/metPhiCorrInfoWriter_ngoodVertices_h0EndcapPlus_Px"));
    histos_x.push_back((TH1D*)gDirectory->Get("metPhiCorrInfoWriter/metPhiCorrInfoWriter_ngoodVertices_h0EndcapMinus_Px"));
    histos_x.push_back((TH1D*)gDirectory->Get("metPhiCorrInfoWriter/metPhiCorrInfoWriter_ngoodVertices_gammaBarrel_Px"));
    histos_x.push_back((TH1D*)gDirectory->Get("metPhiCorrInfoWriter/metPhiCorrInfoWriter_ngoodVertices_gammaEndcapPlus_Px"));
    histos_x.push_back((TH1D*)gDirectory->Get("metPhiCorrInfoWriter/metPhiCorrInfoWriter_ngoodVertices_gammaEndcapMinus_Px"));
    histos_x.push_back((TH1D*)gDirectory->Get("metPhiCorrInfoWriter/metPhiCorrInfoWriter_ngoodVertices_egammaHFPlus_Px"));
    histos_x.push_back((TH1D*)gDirectory->Get("metPhiCorrInfoWriter/metPhiCorrInfoWriter_ngoodVertices_egammaHFMinus_Px"));


    histos_y.push_back((TH1D*)gDirectory->Get("metPhiCorrInfoWriter/metPhiCorrInfoWriter_ngoodVertices_hEtaPlus_Py"));
    histos_y.push_back((TH1D*)gDirectory->Get("metPhiCorrInfoWriter/metPhiCorrInfoWriter_ngoodVertices_hEtaMinus_Py"));
    histos_y.push_back((TH1D*)gDirectory->Get("metPhiCorrInfoWriter/metPhiCorrInfoWriter_ngoodVertices_hHFPlus_Py"));
    histos_y.push_back((TH1D*)gDirectory->Get("metPhiCorrInfoWriter/metPhiCorrInfoWriter_ngoodVertices_hHFMinus_Py"));
    histos_y.push_back((TH1D*)gDirectory->Get("metPhiCorrInfoWriter/metPhiCorrInfoWriter_ngoodVertices_h0Barrel_Py"));
    histos_y.push_back((TH1D*)gDirectory->Get("metPhiCorrInfoWriter/metPhiCorrInfoWriter_ngoodVertices_h0EndcapPlus_Py"));
    histos_y.push_back((TH1D*)gDirectory->Get("metPhiCorrInfoWriter/metPhiCorrInfoWriter_ngoodVertices_h0EndcapMinus_Py"));
    histos_y.push_back((TH1D*)gDirectory->Get("metPhiCorrInfoWriter/metPhiCorrInfoWriter_ngoodVertices_gammaBarrel_Py"));
    histos_y.push_back((TH1D*)gDirectory->Get("metPhiCorrInfoWriter/metPhiCorrInfoWriter_ngoodVertices_gammaEndcapPlus_Py"));
    histos_y.push_back((TH1D*)gDirectory->Get("metPhiCorrInfoWriter/metPhiCorrInfoWriter_ngoodVertices_gammaEndcapMinus_Py"));
    histos_y.push_back((TH1D*)gDirectory->Get("metPhiCorrInfoWriter/metPhiCorrInfoWriter_ngoodVertices_egammaHFPlus_Py"));
    histos_y.push_back((TH1D*)gDirectory->Get("metPhiCorrInfoWriter/metPhiCorrInfoWriter_ngoodVertices_egammaHFMinus_Py"));


    TH1D* histo_combined_x = new TH1D("histo_combined_x",";ngoodVertices;p_{x}(GeV)",100,0,100);
    TH1D* histo_combined_y = new TH1D("histo_combined_y",";ngoodVertices;p_{y}(GeV)",100,0,100);
    std::vector<double> binValues_x (101,0);
    std::vector<double> squaredBinErrors_x (101,0);
    std::vector<double> binValues_y (101,0);
    std::vector<double> squaredBinErrors_y (101,0);

    for (unsigned int iHist = 0 ; iHist < histos_x.size() ; iHist++){
      for (int iBin = 0 ; iBin <= histos_x[iHist]->GetNbinsX() ; iBin++){
        binValues_x[iBin] += histos_x[iHist]->GetBinContent(iBin);
        binValues_y[iBin] += histos_y[iHist]->GetBinContent(iBin);
        squaredBinErrors_x[iBin] += histos_x[iHist]->GetBinError(iBin)*histos_x[iHist]->GetBinError(iBin);
        squaredBinErrors_y[iBin] += histos_y[iHist]->GetBinError(iBin)*histos_y[iHist]->GetBinError(iBin);
      }
    }
    for (int iBin = 0 ; iBin <= histo_combined_x->GetNbinsX() ; iBin++){
      histo_combined_x->SetBinContent(iBin, binValues_x[iBin]);
      histo_combined_x->SetBinError(iBin, sqrt(squaredBinErrors_x[iBin]));
      histo_combined_y->SetBinContent(iBin, binValues_y[iBin]);
      histo_combined_y->SetBinError(iBin, sqrt(squaredBinErrors_y[iBin]));
    }

    outFiles[iFiles]->cd();
    histo_combined_x->Write();
    histo_combined_y->Write();
    outfile << outFiles[iFiles]->GetName() << " : fit in x is ";
    TF1* fit_x = new TF1("fit_x", fitFunction, 0, histo_combined_x->GetNbinsX()+1,2);
    histo_combined_x->Fit("fit_x");
    double par0_x = fit_x->GetParameter(0);
    double par1_x = fit_x->GetParameter(1);
    if(par1_x >= 0.) outfile << par0_x << "+" << par1_x << "*x\n";
    else outfile << par0_x << par1_x << "*x\n";
    outfile << outFiles[iFiles]->GetName() << " : fit in y is ";
    TF1* fit_y = new TF1("fit_y", fitFunction, 0, histo_combined_y->GetNbinsX()+1,2);
    histo_combined_y->Fit("fit_y");
    double par0_y = fit_y->GetParameter(0);
    double par1_y = fit_y->GetParameter(1);
    if(par1_y >= 0.) outfile << par0_y << "+" << par1_y << "*x\n";
    else outfile << par0_y << par1_y << "*x\n";

  }

  outfile.close();
  return 0;
}

